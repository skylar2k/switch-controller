#include "actions.h"

// HOLD

void release_button(USB_JoystickReport_Data_t *ReportData, Buttons_t button)
{
	ReportData->Button &= button ^ 0xffff;
}
// Return 0 if button should still be pressed; 1 if not.
uint16_t press_button(USB_JoystickReport_Data_t *ReportData, Buttons_t button, uint16_t *_duration)
{
	if (*_duration > 10)
	{
		release_button(ReportData, button);
		return 1; // Tell loop to advance.
	}
	ReportData->Button |= button;
	return 0;
}

uint16_t hold_button(USB_JoystickReport_Data_t *ReportData, Buttons_t button, uint16_t hold_duration, uint16_t *_duration)
{
	if (*_duration > hold_duration)
	{
		release_button(ReportData, button);
		return 1;
	}
	ReportData->Button |= button;
	return 0;
};

// Center sticks
void center_sticks(USB_JoystickReport_Data_t *ReportData)
{
	ReportData->LX = STICK_CENTER;
	ReportData->LY = STICK_CENTER;
	ReportData->RX = STICK_CENTER;
	ReportData->RY = STICK_CENTER;
	ReportData->HAT = HAT_CENTER;
};
