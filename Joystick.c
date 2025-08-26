#include "Joystick.h"
#include "actions.h"

// Current step in the script.
uint16_t i = 0;
uint16_t duration = 0;

static const command step[] = {
	{CLEAR, {.clear = {250}}},
	{PRESS, {.press = {SWITCH_L | SWITCH_R}}},
};

int main(void)
{
	SetupHardware();
	GlobalInterruptEnable();

	for (;;)
	{
		HID_Task();
		USB_USBTask();
	}
}

void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* Hardware Initialization */
	USB_Init();
}

void EVENT_USB_Device_Connect(void) {}

void EVENT_USB_Device_Disconnect(void) {}

void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	/* Setup HID Report Endpoint */
	ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);
}

void EVENT_USB_Device_ControlRequest(void) {}

// Callback cleanup functions maybe?
void GetNextReport(USB_JoystickReport_Data_t *ReportData)
{

	if (i > (int)(sizeof(step) / sizeof(step[0])) - 1)
		return;

	switch (step[i].action)
	{
	case PRESS:
		press_button(ReportData, step[i].press.button);
		if (duration > 5) {
			duration = 0;
			i++;
			release_button(ReportData, step[i].press.button);
		}
		break;
	case CLEAR:
		memset(ReportData, 0, sizeof(USB_JoystickReport_Data_t));
		center_sticks(ReportData);
		duration++;
		if(duration > step[i].clear.duration) {
			duration = 0;
			i++;
		}
		break;
	default:
		break;
	}
}

/** Function to manage HID report generation and transmission to the host. */
void HID_Task(void)
{
	/* Device must be connected and configured for the task to run */
	if (USB_DeviceState != DEVICE_STATE_Configured)
		return;

	/* Select the Joystick Report Endpoint */
	Endpoint_SelectEndpoint(JOYSTICK_EPADDR);

	/* Check to see if the host is ready for another packet */
	if (Endpoint_IsINReady())
	{
		static USB_JoystickReport_Data_t ReportData;
		GetNextReport(&ReportData);
		/* Write Joystick Report Data */
		while (Endpoint_Write_Stream_LE(&ReportData, sizeof(ReportData), NULL) != ENDPOINT_RWSTREAM_NoError)
			;

		/* Finalize the stream transfer to send the last packet */
		Endpoint_ClearIN();
		/* Clear the report data afterwards */
		// memset(&ReportData, 0, sizeof(ReportData));
	}
}
