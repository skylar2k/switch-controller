#ifndef _ACTIONS_H_
#define _ACTIONS_H_

#include "Joystick.h"

void release_button(USB_JoystickReport_Data_t *ReportData, Buttons_t button);

uint16_t press_button(USB_JoystickReport_Data_t *ReportData, Buttons_t button, uint16_t *_duration);

uint16_t hold_button(USB_JoystickReport_Data_t *ReportData, Buttons_t button, uint16_t hold_duration, uint16_t *_duration);

void center_sticks(USB_JoystickReport_Data_t *ReportData);

#endif