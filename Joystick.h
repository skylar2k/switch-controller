/*
       LUFA Library
   Copyright (C) Dean Camera, 2021.

  dean [at] fourwalledcubicle [dot] com
       www.lufa-lib.org
*/

/*
  Copyright 2021  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Header file for Joystick.c.
 */

#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

/* Includes: */
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <string.h>

#include "Descriptors.h"

#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Drivers/Board/Joystick.h>
#include <LUFA/Drivers/Board/LEDs.h>
#include <LUFA/Drivers/Board/Buttons.h>
#include <LUFA/Platform/Platform.h>

/* Macros: */
/** LED mask for the library LED driver, to indicate that the USB interface is not ready. */
// #define LEDMASK_USB_NOTREADY LEDS_LED1

/** LED mask for the library LED driver, to indicate that the USB interface is enumerating. */
// #define LEDMASK_USB_ENUMERATING (LEDS_LED2 | LEDS_LED3)

/** LED mask for the library LED driver, to indicate that the USB interface is ready. */
// #define LEDMASK_USB_READY (LEDS_LED2 | LEDS_LED4)

/** LED mask for the library LED driver, to indicate that an error has occurred in the USB interface. */
// #define LEDMASK_USB_ERROR (LEDS_LED1 | LEDS_LED3)

/* Type Defines: */
/** Type define for the joystick HID report structure, for creating and sending HID reports to the host PC.
 *  This mirrors the layout described to the host in the HID report descriptor, in Descriptors.c.
 */

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
  UP,
  DOWN,
  LEFT,
  RIGHT,
  X,
  Y,
  A,
  B,
  L,
  R,
  HOME,
  NOTHING,
  TRIGGERS
} Action;

typedef struct
{
  Action button;
	uint16_t duration;
} command;

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
} JoystickButtons_t;

/* Function Prototypes: */
void SetupHardware(void);
void HID_Task(void);

void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);

void GetNextReport(USB_JoystickReport_Data_t* ReportData);

#endif
