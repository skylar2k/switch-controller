#ifndef LEXER_H_
#define LEXER_H_

#include "buttons.h"

enum command_type
{
    CMD_WAIT,
    CMD_PRESS,
    CMD_HOLD,
    CMD_SET_STICK
};

struct command
{
    enum command_type type;
    union
    {
        struct
        {
            uint16_t duration; // in milliseconds
        } wait;
        struct
        {
            enum button buttons; // bitmask of buttons to press
        } press;
        struct
        {
            uint16_t duration;   // in milliseconds
            enum button buttons; // bitmask of buttons to hold
        } hold;
        struct
        {
            enum button buttons; // bitmask of buttons to release
        } release;
        struct
        {
            struct stick_axis axis; // x and y axis values (0-255)
            enum stick stick;       // which stick to set
        } set_stick;
    };
};

int parse_command (const char *line, struct command *cmd);

#endif // LEXER_H_