/* This file is part of OpenGarden
 * Copyright (C) 2011, 2012 Enrico Rossi
 *
 * OpenGarden is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenGarden is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"

/*! Read a string from the user with echo. */
void debug_get_str(char *str)
{
	uint8_t i = 0;

	*(str + i) = uart_getchar(0, 1);
	uart_putchar(0, *(str + i));

	while ((*(str + i) != '\r') && (i < MAX_LINE_LENGHT)) {
		i++;
		*(str + i) = uart_getchar(0, 1);
		uart_putchar(0, *(str + i));
	}

	i++;
	*(str + i) = 0;
}

/*! Print a flash-stored string to the terminal.
 * \param string MUST be a PSTR() string.
 */
void debug_print_P(PGM_P string, struct debug_t *debug)
{
	if (debug->active) {
		strcpy_P(debug->line, string);
		uart_printstr(0, debug->line);
	}
}

/*! Print the debug->line string. */
void debug_print(struct debug_t *debug)
{
	if (debug->active)
		uart_printstr(0, debug->line);
}

void debug_version(struct debug_t *debug)
{
	debug_print_P(PSTR("OpenGarden "), debug);
	debug_print_P(PSTR(GITREL), debug);
	debug_print_P(PSTR("\n"), debug);
}

/*! Print the greetings and release number. */
static void hello(struct debug_t *debug)
{
        debug_print_P(PSTR("\n\n\n"), debug);
	debug_version(debug);
        debug_print_P(PSTR("\nNicola Galliani <1999tora@gmail.com>\n"), debug);
        debug_print_P(PSTR("Andrea Marabini <info@marabo.it>\n"), debug);
        debug_print_P(PSTR("Enrico Rossi <e.rossi@tecnobrain.com>\n"), debug);
        debug_print_P(PSTR("URL: http://tecnobrain.com/\n"), debug);
        debug_print_P(PSTR("GNU GPL v3 - use at your own risk!\n\n"), debug);
}

/*! Prompt the user for and y/n answer */
uint8_t debug_wait_for_y(struct debug_t *debug)
{
	uint8_t i;
	char c;

	if (debug->active) {
		for (i = 0; i < SEC_FOR_Y; i++) {
			c = uart_getchar(0, 0);

			/*! "Y" is 89 and "y" is 121 */
			if ((c == 89) || (c == 121)) {
				debug_print_P(PSTR("\n"), debug);
				return(1); /*! Exit the cicle in a bad way */
			} else {
				_delay_ms(1000);
				debug_print_P(PSTR("."), debug);
			}
		}
	}

	return(0);
}

void debug_start(struct debug_t *debug)
{
	uart_init(0);
	debug->active = 1;
}

void debug_stop(struct debug_t *debug)
{
	uart_shutdown(0);
	debug->active = 0;
}

/*! Initialize the debug_t structure and ask if
 * debug is active.
 */
struct debug_t *debug_init(struct debug_t *debug)
{
	debug = malloc(sizeof(struct debug_t));
	debug->line = malloc(MAX_LINE_LENGHT);
	debug->string = malloc(MAX_STRING_LENGHT);
	debug_start(debug);
	hello(debug);

#ifndef DEBUG_ALWAYS_ACTIVE
	debug_print_P(PSTR("\nActivate debug? (y/N): "), debug);

	if (!debug_wait_for_y(debug)) {
		debug_stop(debug);
		free(debug->line);
		free(debug->string);
	}
#endif

	return(debug);
}

/*! Free the allocated structure. */
void debug_free(struct debug_t *debug)
{
	if (debug->active) {
		free(debug->string);
		free(debug->line);
	}

	free(debug);
}
