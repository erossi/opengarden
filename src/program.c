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

/*! \file program.c
  \brief Function to load/unload and exec programms
  */

#include <stdlib.h>
#include <stdio.h>
#include "program.h"

struct programms_t *prog_init(void)
{
	struct programms_t *progs;
	
	progs = malloc(sizeof(struct programms_t));
	eeprom_read_block(progs, EE_progs, sizeof(struct programms_t));

	/* initialize a new programms struct */
	if (progs->check != CHECK_VALID_CODE) {
		progs->check = CHECK_VALID_CODE;
		progs->number = 0; /* 0 valid program */
	}

	return(progs);
}

void change_io_line(const uint8_t oline, const uint8_t onoff)
{
	if (oline & _BV(0))
		io_out_set(OUT_P0, onoff);
	if (oline & _BV(1))
		io_out_set(OUT_P1, onoff);
	if (oline & _BV(2))
		io_out_set(OUT_P2, onoff);
	if (oline & _BV(3))
		io_out_set(OUT_P3, onoff);
	if (oline & _BV(4))
		io_out_set(OUT_P4, onoff);
	if (oline & _BV(5))
		io_out_set(OUT_P5, onoff);
	if (oline & _BV(6))
		io_out_set(OUT_P6, onoff);
	if (oline & _BV(7))
		io_out_set(OUT_P7, onoff);
}

/*! Check which program to execi.
  \param hh time of the program 0..48 (half hour steps)
 */
void prog_exec(struct programms_t *progs, const uint8_t hh)
{
	uint8_t i;

	for (i=0; i<progs->number; i++) {
		if (progs->p[i].hstart == hh)
			change_io_line(i, 1);

		if (progs->p[i].hstop == hh)
			change_io_line(i, 0);
	}
}

/*! list all valid programms */
void prog_list(struct programms_t *progs, struct debug_t *debug)
{
	uint8_t i;

	sprintf_P(debug->line, "\nProgramms list [%d]:\n", progs->number);
	debug_print(debug);
	debug_print_P("p<number>,<start>,<stop>,<DoW>,<out line>\n", debug);

	for (i = 0; i < progs->number; i++) {
		sprintf_P(debug->line, "p%d,%d,%d,%d\n",i ,progs->p[i].hstart, progs->p[i].hstop, progs->p[i].dow, progs->p[i].oline);
		debug_print(debug);
	}
}

/*! remove all programms */
void prog_clear(struct programms_t *progs)
{
	progs->number = 0;
}

/*! add a program into memory
  \param s string in the form pHH,SS,DD,OO
  where
  HH Start hour in the form 0..48 of 30min each.
  SS Stop hour.
  DD Day of the week sun..sat bit for day.
  OO output line 0..7 bit for line 0 to 7.
 */
void prog_add(struct programms_t *progs, const char *s)
{
	uint8_t hstart, hstop, dow, oline;

	if (progs->number + 1 < MAX_PROGS) {
		progs->number++;
		progs->p[progs->number].hstart = hstart;
		progs->p[progs->number].hstop = hstop;
		progs->p[progs->number].dow = dow;
		progs->p[progs->number].oline = oline;
	}
}

uint8_t prog_del(struct programms_t *progs, const uint8_t n)
{
	uint8_t i;

	if (n < progs->number) {
		for (i=n+1; i < progs->number; i++)
			progs->p[i-1] = progs->p[i];

		progs->number--;
		return(1);
	} else {
		return(0);
	}
}

void prog_save(struct programms_t *progs)
{
	eeprom_update_block(progs, EE_progs, sizeof(struct programms_t));
}
