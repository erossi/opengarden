/* This file is part of OpenGarden
 * Copyright (C) 2011 Enrico Rossi
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
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "debug.h"

void debug_print_P(PGM_P string, struct debug_t *debug)
{
	if (debug->active) {
		strcpy_P(debug->line, string);
		uart_printstr(0, debug->line);
	}
}

void debug_print(struct debug_t *debug)
{
	if (debug->active)
		uart_printstr(0, debug->line);
}

static void hello(struct debug_t *debug)
{
        debug_print_P(PSTR("\n\n\n"), debug);
        debug_print_P(PSTR("OpenGarden Rel: "), debug);
        debug_print_P(PSTR(GITREL), debug);
        debug_print_P(PSTR("\n\n"), debug);
        debug_print_P(PSTR("Nicola Galliani <1999tora@gmail.com>\n"), debug);
        debug_print_P(PSTR("Andrea Marabini <info@marabo.it>\n"), debug);
        debug_print_P(PSTR("Enrico Rossi <e.rossi@tecnobrain.com>\n"), debug);
        debug_print_P(PSTR("URL: http://tecnobrain.com/\n"), debug);
        debug_print_P(PSTR("GNU GPL v3 - use at your own risk!\n"), debug);
        debug_print_P(PSTR("\n\n"), debug);
}

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

struct debug_t *debug_init(void)
{
	struct debug_t *debug;

	uart_init(0);
	debug = malloc(sizeof(struct debug_t));
	debug->line = malloc(MAX_LINE_LENGHT);
	debug->string = malloc(MAX_STRING_LENGHT);
	debug->active = 1;
	hello(debug);
	debug_print_P(PSTR("\nActivate debug? (y/N): "), debug);

	if (!debug_wait_for_y(debug)) {
		uart_shutdown(0);
		debug->active = 0;
		free(debug->line);
		free(debug->string);
	}

	return(debug);
}

void debug_free(struct debug_t *debug)
{
	if (debug->active) {
		free(debug->string);
		free(debug->line);
	}

	free(debug);
}
