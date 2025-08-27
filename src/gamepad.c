#include "gamepad.h"
#include "descriptors.h"
#include "parser.h"
#include "script.h"

unsigned char script[];
unsigned int script_len;

int main (void)
{
    SetupHardware ();
    GlobalInterruptEnable ();

    // Create and zero our report struct, and pass it
    // report.lstick = (struct stick_axis){ 128, 128 };
    // report.rstick = (struct stick_axis){ 128, 128 };
    DDRD |= (1 << PD5);

    for (;;)
        {
            HID_Task ();
            USB_USBTask ();
        }
}

void SetupHardware (void)
{
    /* Disable watchdog if enabled by bootloader/fuses */
    MCUSR &= ~(1 << WDRF);
    wdt_disable ();

    /* Disable clock division */
    clock_prescale_set (clock_div_1);

    /* Hardware Initialization */
    USB_Init ();
}

void EVENT_USB_Device_Connect (void) {}

void EVENT_USB_Device_Disconnect (void) {}

void EVENT_USB_Device_ConfigurationChanged (void)
{
    bool ConfigSuccess = true;

    /* Setup HID Report Endpoint */
    ConfigSuccess &= Endpoint_ConfigureEndpoint (
      JOYSTICK_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);
}

void EVENT_USB_Device_ControlRequest (void) {}

void get_next_report (struct report_data *report)
{
    static uint16_t counter    = 0;
    static uint16_t duration   = 0;
    static bool should_advance = true;
    static struct command cmd;

    if (should_advance && counter < script_len)
        {
            duration       = 0;
            should_advance = false;

            char line[128];
            int i = 0;
            // Read a line until newline or end of script
            while (counter < script_len && script[counter] != 0x0a
                   && i < (int)(sizeof (line) - 1))
                line[i++] = script[counter++];
            line[i] = '\0';
            if (i > 0 && line[i - 1] == 0x0d)
                line[i - 1] = '\0';
            // Skip newline character
            if (counter < script_len && script[counter] == 0x0a)
                counter++;
            // Parse the command
            if (parse_command (line, &cmd) != 0)
                return; // Parsing error, skip this line
        }

    // Execute the command
    switch (cmd.type)
        {
        case CMD_WAIT: should_advance = duration > cmd.wait.duration; break;
        case CMD_PRESS:
            report->buttons |= cmd.press.buttons;
            should_advance = duration > 10; // Press for 10ms
            // Release buttons after press
            if (should_advance)
                report->buttons &= ~cmd.press.buttons;
            break;
        case CMD_HOLD:
            // Not implemented yet
            break;
        default: break;
        }
    duration++;
}

/** Function to manage HID report generation and transmission to the host. */
void HID_Task (void)
{
    /* Device must be connected and configured for the task to run */
    if (USB_DeviceState != DEVICE_STATE_Configured)
        return;

    /* Select the Joystick Report Endpoint */
    Endpoint_SelectEndpoint (JOYSTICK_EPADDR);

    /* Check to see if the host is ready for another packet */
    if (Endpoint_IsINReady ())
        {
            static struct report_data report;
            // center_sticks (&report);
            report.hat = HAT_CENTER;
            report.lx  = 128;
            report.ly  = 128;
            report.rx  = 128;
            report.ry  = 128;
            get_next_report (&report);
            /* Write Joystick Report Data */
            while (Endpoint_Write_Stream_LE (&report, sizeof (report), NULL)
                   != ENDPOINT_RWSTREAM_NoError)
                ;

            /* Finalize the stream transfer to send the last packet */
            Endpoint_ClearIN ();
            /* Clear the report data afterwards */
            // memset(&ReportData, 0, sizeof(ReportData));
        }
}
