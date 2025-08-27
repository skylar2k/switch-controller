#ifndef LEXER_H_
#define LEXER_H_

#include "buttons.h"

enum command_type
{
    CMD_WAIT,
    CMD_PRESS,
    CMD_HOLD
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
    };
};

int parse_command (const char *line, struct command *cmd);

#endif // LEXER_H_