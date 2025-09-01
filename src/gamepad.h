#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <string.h>

#include <LUFA/Drivers/Board/Buttons.h>
#include <LUFA/Drivers/Board/Joystick.h>
#include <LUFA/Drivers/Board/LEDs.h>
#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>

#include "buttons.h"

struct report_data
{
    enum button buttons : 16;
    enum hat_direction hat : 8;
    struct stick_axis lstick;
    struct stick_axis rstick;
    uint8_t _; // Vendor spec, not used
};

/* Function Prototypes: */
void SetupHardware (void);
void HID_Task (struct report_data *report);
void get_next_report (struct report_data *report);

void EVENT_USB_Device_Connect (void);
void EVENT_USB_Device_Disconnect (void);
void EVENT_USB_Device_ConfigurationChanged (void);
void EVENT_USB_Device_ControlRequest (void);

#endif
