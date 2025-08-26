#ifndef _ACTIONS_H_
#define _ACTIONS_H_

#include "Joystick.h"

void release_button(USB_JoystickReport_Data_t* ReportData, Buttons_t button);

void press_button(USB_JoystickReport_Data_t *ReportData, Buttons_t button);

void center_sticks(USB_JoystickReport_Data_t *ReportData);

#endif