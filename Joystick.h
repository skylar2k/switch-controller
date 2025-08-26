#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

/* Includes: */
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <string.h>

#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Drivers/Board/Joystick.h>
#include <LUFA/Drivers/Board/LEDs.h>
#include <LUFA/Drivers/Board/Buttons.h>
#include <LUFA/Platform/Platform.h>

#include "Descriptors.h"

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

typedef struct
{
  uint16_t Button;
  uint8_t HAT;
  uint8_t LX; // Left  Stick X
  uint8_t LY; // Left  Stick Y
  uint8_t RX; // Right Stick X
  uint8_t RY; // Right Stick Y
  uint8_t VendorSpec;
} USB_JoystickReport_Data_t;

typedef enum
{
  SWITCH_Y = 0x01,
  SWITCH_B = 0x02,
  SWITCH_A = 0x04,
  SWITCH_X = 0x08,
  SWITCH_L = 0x10,
  SWITCH_R = 0x20,
  SWITCH_ZL = 0x40,
  SWITCH_ZR = 0x80,
  SWITCH_MINUS = 0x100,
  SWITCH_PLUS = 0x200,
  SWITCH_LCLICK = 0x400,
  SWITCH_RCLICK = 0x800,
  SWITCH_HOME = 0x1000,
  SWITCH_CAPTURE = 0x2000,
} Buttons_t;

typedef enum
{
  HOLD,
  PRESS,
  CLEAR
} Action;

typedef struct
{
  Action action;
  union
  {
    struct
    {
      Buttons_t button;
    } press;
    struct
    {
      Buttons_t button;
      uint16_t duration;
    } hold;
    struct
    {
      uint16_t duration;
    } clear;
  };
} command;

/* Function Prototypes: */
void SetupHardware(void);
void HID_Task(void);

void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);

void GetNextReport(USB_JoystickReport_Data_t *ReportData);

#endif
