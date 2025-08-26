#include "actions.h"

#define HAT_TOP 0x00
#define HAT_TOP_RIGHT 0x01
#define HAT_RIGHT 0x02
#define HAT_BOTTOM_RIGHT 0x03
#define HAT_BOTTOM 0x04
#define HAT_BOTTOM_LEFT 0x05
#define HAT_LEFT 0x06
#define HAT_TOP_LEFT 0x07
#define HAT_CENTER 0x08

#define STICK_MIN 0
#define STICK_CENTER 128
#define STICK_MAX 255

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
}

void set_stick_direction(USB_JoystickReport_Data_t *ReportData, StickType stick, StickPoint direction)
{
	switch (stick)
	{
	case LSTICK:
		ReportData->LX = direction.x;
		ReportData->LY = direction.y;
		break;
	case RSTICK:
		ReportData->RX = direction.x;
		ReportData->RY = direction.y;
		break;
	}
}

// Center sticks
void center_sticks(USB_JoystickReport_Data_t *ReportData)
{
	ReportData->LX = STICK_CENTER;
	ReportData->LY = STICK_CENTER;
	ReportData->RX = STICK_CENTER;
	ReportData->RY = STICK_CENTER;
	ReportData->HAT = HAT_CENTER;
}