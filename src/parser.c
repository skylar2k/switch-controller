#include "parser.h"
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

int parse_button (const char *token, enum button *btn)
{
    if (strcmp (token, "A") == 0)
        *btn = BT_A;
    else if (strcmp (token, "B") == 0)
        *btn = BT_B;
    else if (strcmp (token, "X") == 0)
        *btn = BT_X;
    else if (strcmp (token, "Y") == 0)
        *btn = BT_Y;
    else if (strcmp (token, "L") == 0)
        *btn = BT_L;
    else if (strcmp (token, "R") == 0)
        *btn = BT_R;
    else if (strcmp (token, "ZL") == 0)
        *btn = BT_ZL;
    else if (strcmp (token, "ZR") == 0)
        *btn = BT_ZR;
    else if (strcmp (token, "MINUS") == 0)
        *btn = BT_MINUS;
    else if (strcmp (token, "PLUS") == 0)
        *btn = BT_PLUS;
    else if (strcmp (token, "LCLICK") == 0)
        *btn = BT_LCLICK;
    else if (strcmp (token, "RCLICK") == 0)
        *btn = BT_RCLICK;
    else if (strcmp (token, "HOME") == 0)
        *btn = BT_HOME;
    else if (strcmp (token, "CAPTURE") == 0)
        *btn = BT_CAPTURE;
    else
        return -1; // Unknown button

    return 0; // Success
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

    return 0; // Success
}

void flash (void)
{
    PORTD |= (1 << PD5);
    _delay_ms (100);
    PORTD &= ~(1 << PD5);
}