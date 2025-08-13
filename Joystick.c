#include "Joystick.h"

// Current step in the script.
uint16_t i = 0;
uint16_t duration = 0;

static const command step[] = {
	{NOTHING, 250},
	{TRIGGERS, 5},
	{NOTHING, 150},

	{X, 5},
	{NOTHING, 150}};

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

	/* Indicate endpoint configuration success or failure */
	// LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

void EVENT_USB_Device_ControlRequest(void) {}

void GetNextReport(USB_JoystickReport_Data_t *ReportData)
{
	memset(ReportData, 0, sizeof(USB_JoystickReport_Data_t));
	ReportData->LX = STICK_CENTER;
	ReportData->LY = STICK_CENTER;
	ReportData->RX = STICK_CENTER;
	ReportData->RY = STICK_CENTER;
	ReportData->HAT = HAT_CENTER;

	switch (step[i].button)
	{
	case TRIGGERS:
		ReportData->Button |= SWITCH_L | SWITCH_R;
		break;
	case A:
		ReportData->Button |= SWITCH_A;
		break;
	case X:
		ReportData->Button |= SWITCH_X;
		break;
	default:
		ReportData->LX = STICK_CENTER;
		ReportData->LY = STICK_CENTER;
		ReportData->RX = STICK_CENTER;
		ReportData->RY = STICK_CENTER;
		ReportData->HAT = HAT_CENTER;
		break;
	}
	duration++;
	if (duration > step[i].duration)
	{
		i++;
		duration = 0;
	}
	if (i > (int)(sizeof(step) / sizeof(step[0])) - 1)
	{
		i = 3;
		duration = 0;
		ReportData->LX = STICK_CENTER;
		ReportData->LY = STICK_CENTER;
		ReportData->RX = STICK_CENTER;
		ReportData->RY = STICK_CENTER;
		ReportData->HAT = HAT_CENTER;
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
		USB_JoystickReport_Data_t ReportData;
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
