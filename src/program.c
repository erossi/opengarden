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
#include <string.h>
#include "program.h"

/*! global EEPROM variable */
struct programms_t EEMEM EE_progs;

void prog_load(struct programms_t *progs)
{
	eeprom_read_block(progs, &EE_progs, sizeof(struct programms_t));

	/* initialize a new programms struct */
	if (progs->check != CHECK_VALID_CODE) {
		progs->check = CHECK_VALID_CODE;
		progs->number = 0; /* 0 valid program */
	}
}

struct programms_t *prog_init(struct programms_t *progs)
{
	progs = malloc(sizeof(struct programms_t));
	prog_load(progs);
	io_in_init();
	io_out_init();
	return(progs);
}

/*! Free the allocated memory and close the IO lines */
void prog_free(struct programms_t *progs)
{
	/*! \todo shut IO down, to be implemented but useless. */
	free(progs);
}

void change_io_line(const uint8_t oline, const uint8_t onoff)
{
	/*
	   if (io_in_get(IN_P0)) {
	   io_out_set(OUT_P0, 1);
	   io_out_set(OUT_P1, 1);
	   } else {
	   io_out_set(OUT_P0, 0);
	   io_out_set(OUT_P1, 0);
	   }
	   */

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

/*! Check which program to exec.
  \param tm_clock time now.
 */
void prog_run(struct programms_t *progs, struct tm *tm_clock, struct debug_t *debug)
{
	uint8_t i;

	for (i=0; i<progs->number; i++) {
		if ((progs->p[i].hstart == tm_clock->tm_hour) && (progs->p[i].mstart == tm_clock->tm_min)) {
			sprintf_P(debug->line, PSTR(" prog %d start\n"), i);
			debug_print(debug);
			change_io_line(progs->p[i].oline, 1);
		}

		if ((progs->p[i].hstop == tm_clock->tm_hour) && (progs->p[i].mstop == tm_clock->tm_min)) {
			sprintf_P(debug->line, PSTR(" prog %d stop\n"), i);
			debug_print(debug);
			change_io_line(progs->p[i].oline, 0);
		}
	}
}

/*! list all valid programms */
void prog_list(struct programms_t *progs, struct debug_t *debug)
{
	uint8_t i;

	sprintf_P(debug->line, PSTR("Programms list [%2d]:\n"), progs->number);
	debug_print(debug);
	debug_print_P(PSTR("p<number>,<start>,<stop>,<DoW>,<out line>\n"), debug);

	for (i = 0; i < progs->number; i++) {
		sprintf_P(debug->line, PSTR("p%2d,%2d%2d,%2d%2d,%2x,%2x\n"),i ,progs->p[i].hstart, progs->p[i].mstart, progs->p[i].hstop, progs->p[i].mstop, progs->p[i].dow, progs->p[i].oline);
		debug_print(debug);
	}
}

/*! remove all programms */
void prog_clear(struct programms_t *progs)
{
	progs->number = 0;
}

/*! add a program into memory
  \param s string in the form pShSm,shsm,DD,OO
  where
  Sh Start hour in the form 0..24.
  Sm Start minutes.
  sh Stop hour.
  sm Stop minutes.
  DD Day of the week sun..sat bit for day (HEX number).
  OO output line 0..7 bit for line 0 to 7 (HEX number).
 */
void prog_add(struct programms_t *progs, const char *s)
{
	char *substr;

	if (progs->number + 1 < MAX_PROGS) {
		substr = malloc(3);
		/* get Sh, copy from s char 1..2 into substr */
		strlcpy(substr, s + 1, 3);
		progs->p[progs->number].hstart = strtoul(substr, 0, 10);
		/* get Sm, copy from s char 3..4 into substr */
		strlcpy(substr, s + 3, 3);
		progs->p[progs->number].mstart = strtoul(substr, 0, 10);
		/* get sh */
		strlcpy(substr, s + 6, 3);
		progs->p[progs->number].hstop = strtoul(substr, 0, 10);
		/* get sm */
		strlcpy(substr, s + 8, 3);
		progs->p[progs->number].mstop = strtoul(substr, 0, 10);
		/* get DD */
		strlcpy(substr, s + 11, 3);
		progs->p[progs->number].dow = strtoul(substr, 0, 16);
		/* get OO */
		strlcpy(substr, s + 14, 3);
		progs->p[progs->number].oline = strtoul(substr, 0, 16);
		free(substr);
		progs->number++;
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
	eeprom_update_block(progs, &EE_progs, sizeof(struct programms_t));
}
