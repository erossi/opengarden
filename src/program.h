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
  \brief Functions to handle programs
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
/*! maximum number of programs in memory */
#define MAX_PROGS 20
/*! time in sec for the programs to be executed now */
#define NOW 0

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

struct queue_t {
	time_t time;
	uint8_t oline;
	uint8_t flag;
};

/*! Single structure to keep all the programs */
struct programs_t {
	/*! control code, should be == CHECK_VALID_CODE */
	uint8_t check;
	/*! number of valid programs 0..MAX_PROGS */
	uint8_t number;
	uint8_t qc;
	/*! array of the programs 0..(MAX_PROGS - 1)*/
	struct program_t p[MAX_PROGS];
	struct queue_t q[MAX_PROGS];
};

struct programs_t *prog_init(struct programs_t *progs);
void prog_free(struct programs_t *progs);
void prog_load(struct programs_t *progs);
void prog_save(struct programs_t *progs);
void prog_list(struct programs_t *progs, struct debug_t *debug);
void prog_clear(struct programs_t *progs);
void prog_add(struct programs_t *progs, const char *s);
uint8_t prog_del(struct programs_t *progs, const uint8_t n);
void prog_run(struct programs_t *progs, struct tm *tm_clock, struct debug_t *debug);

#endif
