#include "actions.h"

// SYNC

// HOLD
void hold_button(){};

void release_button(USB_JoystickReport_Data_t* ReportData, Buttons_t button) {
    ReportData->Button &= button;
}
// PRESS
void press_button(USB_JoystickReport_Data_t* ReportData, Buttons_t button) {
    ReportData->Button |= button;
}

// Center sticks
void center_sticks(USB_JoystickReport_Data_t* ReportData){
	ReportData->LX = STICK_CENTER;
	ReportData->LY = STICK_CENTER;
	ReportData->RX = STICK_CENTER;
	ReportData->RY = STICK_CENTER;
	ReportData->HAT = HAT_CENTER;
};
