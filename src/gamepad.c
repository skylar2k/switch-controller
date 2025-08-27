#include "gamepad.h"
#include "descriptors.h"
#include "parser.h"

// Current step in the script.
// uint16_t i = 0;

// static const command step[] = {
//     CLEAR (250),   PRESS (SWITCH_L | SWITCH_R), SET_STICK (LSTICK, 128, 0),
//     NOTHING (250), PRESS (SWITCH_LCLICK),
// };

unsigned char scripts_test_script[]
  = { 0x57, 0x41, 0x49, 0x54, 0x20, 0x32, 0x35, 0x30, 0x0d, 0x0a,
      0x50, 0x52, 0x45, 0x53, 0x53, 0x20, 0x4c, 0x7c, 0x52 };
unsigned int scripts_test_script_len = 19;

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

    if (should_advance && counter < scripts_test_script_len)
        {
            duration       = 0;
            should_advance = false;

            char line[128];
            int i = 0;
            // Read a line until newline or end of script
            while (counter < scripts_test_script_len
                   && scripts_test_script[counter] != 0x0a
                   && i < (int)(sizeof (line) - 1))
                {
                    line[i++] = scripts_test_script[counter++];
                }
            line[i] = '\0';
            if (i > 0 && line[i - 1] == 0x0d)
                line[i - 1] = '\0';
            // Skip newline character
            if (counter < scripts_test_script_len
                && scripts_test_script[counter] == 0x0a)
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
                {

                    PORTD |= (1 << PD5);
                    _delay_ms (500);
                    PORTD &= ~(1 << PD5);
                    _delay_ms (500);
                    report->buttons &= ~cmd.press.buttons;
                }
            break;
        case CMD_HOLD:
            // Not implemented yet
            break;
        default: break;
        }
    duration++;
}

//    static uint16_t duration = 0;
//    uint16_t should_advance  = 0;
//    if (i > (int)(sizeof (step) / sizeof (step[0])) - 1)
//        return;
//
//    switch (step[i].action)
//        {
//        case PRESS:
//            should_advance
//              = press_button (ReportData, step[i].press.button,
//              &duration);
//            break;
//        case HOLD:
//            should_advance = hold_button (ReportData,
//            step[i].hold.button,
//                                          step[i].hold.duration,
//                                          &duration);
//            break;
//        case SET_STICK:
//            set_stick_direction (ReportData, step[i].set_stick.stick,
//                                 step[i].set_stick.direction);
//            should_advance = 1;
//            break;
//        case CLEAR:
//            memset (ReportData, 0, sizeof (USB_JoystickReport_Data_t));
//            center_sticks (ReportData);
//            should_advance = duration > step[i].clear.duration;
//            break;
//        case NOTHING:
//            should_advance = duration > step[i].nothing.duration;
//            break;
//        default: break;
//        }
//    duration++;
//    if (should_advance)
//        {
//            duration = 0;
//            i++;
//        }

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
