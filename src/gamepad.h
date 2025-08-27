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

#define CLEAR(duration)                                                       \
    {                                                                         \
        CLEAR,                                                                \
        {                                                                     \
            .clear = { duration }                                             \
        }                                                                     \
    }
#define PRESS(button)                                                         \
    {                                                                         \
        PRESS,                                                                \
        {                                                                     \
            .press = { button }                                               \
        }                                                                     \
    }
#define HOLD(button, duration)                                                \
    {                                                                         \
        HOLD,                                                                 \
        {                                                                     \
            .hold = { button, duration }                                      \
        }                                                                     \
    }
#define SET_STICK(stick, x, y)                                                \
    {                                                                         \
        SET_STICK,                                                            \
        {                                                                     \
            .set_stick = { stick, { x, y } }                                  \
        }                                                                     \
    }
#define NOTHING(duration)                                                     \
    {                                                                         \
        NOTHING,                                                              \
        {                                                                     \
            .nothing = { duration }                                           \
        }                                                                     \
    }

typedef enum
{
    HOLD,
    PRESS,
    CLEAR,
    SET_STICK,
    NOTHING
} Action;

// typedef struct
//{
//     Action action;
//     union
//     {
//         struct
//         {
//             Buttons_t button;
//         } press;
//         struct
//         {
//             Buttons_t button;
//             uint16_t duration;
//         } hold;
//         struct
//         {
//             uint16_t duration;
//         } clear;
//         struct
//         {
//             StickType stick;
//             StickPoint direction;
//         } set_stick;
//         struct
//         {
//             uint16_t duration;
//         } nothing;
//     };
// } command;

struct report_data
{
    enum button buttons : 16;
    enum hat_direction hat : 8;
    uint8_t ly;
    uint8_t lx;
    uint8_t ry;
    uint8_t rx;
    // struct stick_axis lstick;
    // struct stick_axis rstick;
    uint8_t _; // Vendor spec, not used
};

/* Function Prototypes: */
void SetupHardware (void);
void HID_Task (void);
void get_next_report (struct report_data *report);

void EVENT_USB_Device_Connect (void);
void EVENT_USB_Device_Disconnect (void);
void EVENT_USB_Device_ConfigurationChanged (void);
void EVENT_USB_Device_ControlRequest (void);

#endif
