#include "parser.h"
#include <stdlib.h>
#include <string.h>

#include <avr/io.h>
#include <util/delay.h>

int parse_button (const char *token, enum button *btn)
{
    struct
    {
        const char *name;
        enum button value;
    } button_map[] = {
        { "A", BT_A },           { "B", BT_B },
        { "X", BT_X },           { "Y", BT_Y },
        { "L", BT_L },           { "R", BT_R },
        { "ZL", BT_ZL },         { "ZR", BT_ZR },
        { "HOME", BT_HOME },     { "PLUS", BT_PLUS },
        { "MINUS", BT_MINUS },   { "LCLICK", BT_LCLICK },
        { "RCLICK", BT_RCLICK }, { "CAPTURE", BT_CAPTURE },
    };
    size_t token_len = strlen (token);
    for (size_t i = 0; i < sizeof (button_map) / sizeof (button_map[0]); ++i)
        {
            size_t name_len = strlen (button_map[i].name);
            if (token_len == name_len
                && memcmp (token, button_map[i].name, name_len) == 0)
                {
                    *btn = button_map[i].value;
                    return 0; // Success
                }
        }
    return -1; // Unknown button
}

int parse_command (const char *line, struct command *cmd)
{
    char buffer[128];
    strncpy (buffer, line, sizeof (buffer) - 1);
    buffer[sizeof (buffer) - 1] = '\0';

    char *token = strtok (buffer, " ");
    if (!token)
        return -1;

    if (strcmp (token, "WAIT") == 0)
        {
            cmd->type = CMD_WAIT;
            token     = strtok (0, " ");

            if (!token)
                return -1;
            cmd->wait.duration = (uint16_t)atoi (token);
        }
    if (strcmp (token, "PRESS") == 0)
        {
            cmd->type = CMD_PRESS;
            token     = strtok (0, " ");
            if (!token)
                return -1;

            enum button btn = BT_NONE;
            char *btn_token = strtok (token, "|");
            while (btn_token)
                {
                    enum button single_btn;
                    if (parse_button (btn_token, &single_btn) != 0)
                        return -1; // Unknown button
                    btn |= single_btn;
                    btn_token = strtok (0, "|");
                }
            cmd->press.buttons = btn;
        }
    if (strcmp (token, "HOLD") == 0)
        {
            cmd->type = CMD_HOLD;
            token     = strtok (0, " "); // Duration
            if (!token)
                return -1;
            cmd->hold.duration = (uint16_t)atoi (token);

            token = strtok (0, " "); // Buttons
            if (!token)
                return -1;

            enum button btn = BT_NONE;
            char *btn_token = strtok (token, "|");
            while (btn_token)
                {
                    enum button single_btn;
                    if (parse_button (btn_token, &single_btn) != 0)
                        return -1; // Unknown button
                    btn |= single_btn;
                    btn_token = strtok (0, "|");
                }
            cmd->hold.buttons = btn;
        }
    if (strcmp (token, "STICK") == 0)
        {
            cmd->type = CMD_SET_STICK;
            token     = strtok (0, " "); // Stick
            if (!token)
                return -1;
            if (strcmp (token, "LEFT") == 0)
                cmd->set_stick.stick = STICK_LEFT;
            else if (strcmp (token, "RIGHT") == 0)
                cmd->set_stick.stick = STICK_RIGHT;
            else
                return -1; // Unknown stick

            token = strtok (0, " "); // X
            if (!token)
                return -1;
            cmd->set_stick.axis.x = (int8_t)atoi (token);
            token                 = strtok (0, " "); // Y
            if (!token)
                return -1;
            cmd->set_stick.axis.y = (int8_t)atoi (token);
        }
    return 0; // Success
}