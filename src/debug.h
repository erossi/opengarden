/* This file is part of OpenGarden
 * Copyright (C) 2011-2014 Enrico Rossi
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

/*! \file debug.h
 * \brief Functions to activare and display (rs232) debug.
 */

#ifndef DBG_H
#define DBG_H

#include <avr/pgmspace.h>
#include "uart.h"

/*! Macro example for create a function macro in 2 pass.
 *
 * The scope should be \#x -> f(x)
 */
#define QUOTEME_(x) #x
/*! Macro example 2nd pass. */
#define QUOTEME(x) QUOTEME_(x)

/*! Maximum number of char a line can be */
#define MAX_LINE_LENGHT 80

/*! Number of char a substring can be made of.
Substrings are used to compose the line when, for example,
conversions have to be made from int to string etc. */
#define MAX_STRING_LENGHT 20

/*! Unused. */
#define PRINT_VALUE_X_LINE 16

/*! Seconds to wait for an answer (y/n) */
#define SEC_FOR_Y 5

/*! GITREL Environment check */
#ifndef GITREL
#define GITREL "unknown"
#endif

/*! The main debug structure, it has to be allocated,
  eventually, if debug is not active, you can avoid the
  allocation of the two members char *.
  */
struct debug_t {
	/*! the line to be printed */
	char *line;
	/*! usefull pre-allocated space to use in strcat()... */
	char *string;
	/*! is debug active, shall we print the output? */
	uint8_t active;
};

void debug_get_str(char *str);
void debug_print_P(PGM_P string, struct debug_t *debug);
void debug_print(struct debug_t *debug);
void debug_version(struct debug_t *debug);
uint8_t debug_wait_for_y(struct debug_t *debug);
void debug_start(struct debug_t *debug);
void debug_stop(struct debug_t *debug);
struct debug_t *debug_init(struct debug_t *debug);
void debug_free(struct debug_t *debug);

#endif
