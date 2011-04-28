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

/*! \file program.h
  \brief Functions to handle programms
  */

#ifndef PROGS_H
#define PROGS_H

#include <avr/eeprom.h>
#include "io_out.h"
#include "io_input.h"
#include "debug.h"
#include "date.h"

/*! check code to control if a valid program is in memeory.
 * Change this code any time the struct program_t has been changed
 */
#define CHECK_VALID_CODE 0x7d
/*! maximum number of programms in memory */
#define MAX_PROGS 20

/*! A single program event structure */
struct program_t {
	/*! Days of the week */
	uint8_t dow;
	/*! Output line */
	uint8_t oline;
	/*! start time hours */
	uint8_t hstart;
	/*! start time minutes */
	uint8_t mstart;
	/*! stop time hours*/
	uint8_t hstop;
	/*! stop time minutes*/
	uint8_t mstop;
};

/*! Single structure to keep all the programms */
struct programms_t {
	/*! control code, should be == CHECK_VALID_CODE */
	uint8_t check;
	/*! number of valid programms 0..MAX_PROGS */
	uint8_t number;
	/*! array of the programms 0..(MAX_PROGS - 1)*/
	struct program_t p[MAX_PROGS];
};

struct programms_t *prog_init(struct programms_t *progs);
void prog_free(struct programms_t *progs);
void prog_load(struct programms_t *progs);
void prog_save(struct programms_t *progs);
void prog_list(struct programms_t *progs, struct debug_t *debug);
void prog_clear(struct programms_t *progs);
void prog_add(struct programms_t *progs, const char *s);
uint8_t prog_del(struct programms_t *progs, const uint8_t n);
void prog_run(struct programms_t *progs, struct tm *tm_clock, struct debug_t *debug);

#endif
