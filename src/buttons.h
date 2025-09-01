#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <stdint.h>

enum button
{
    BT_NONE    = 0x0000,
    BT_Y       = 0x0001,
    BT_B       = 0x0002,
    BT_A       = 0x0004,
    BT_X       = 0x0008,
    BT_L       = 0x0010,
    BT_R       = 0x0020,
    BT_ZL      = 0x0040,
    BT_ZR      = 0x0080,
    BT_MINUS   = 0x0100,
    BT_PLUS    = 0x0200,
    BT_LCLICK  = 0x0400,
    BT_RCLICK  = 0x0800,
    BT_HOME    = 0x1000,
    BT_CAPTURE = 0x2000
};

enum hat_direction
{
    HAT_TOP          = 0x00,
    HAT_TOP_RIGHT    = 0x01,
    HAT_RIGHT        = 0x02,
    HAT_BOTTOM_RIGHT = 0x03,
    HAT_BOTTOM       = 0x04,
    HAT_BOTTOM_LEFT  = 0x05,
    HAT_LEFT         = 0x06,
    HAT_TOP_LEFT     = 0x07,
    HAT_CENTER       = 0x08
};

enum stick
{
    STICK_LEFT,
    STICK_RIGHT
};

struct stick_axis
{
    uint8_t x;
    uint8_t y;
};

#endif // BUTTONS_H_