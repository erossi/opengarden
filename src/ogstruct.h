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

/*! \file ogstruct.h
  \brief structures.
  */

#ifndef OGSTR_H
#define OGSTR_H

#include "date.h"

/*! \brief check code to control if a valid program is in memory.
 *
 * Change this code any time the struct program_t has been changed
 * when reprogram the micro.
 * During boot the program read from flash this code, and, if it is
 * valid, retrive from flash the stored programs.
 * If the structs are changed, we should change this code too, to avoid
 * retrival of wrong data.
 * If, during programming, you nuke the flash memory too, this check
 * code is useless.
 */
#define CHECK_VALID_CODE 0x7e
/*! \brief maximum number of programs */
#define MAX_PROGS 20
#define PROG_MAX_FACTOR 3.0
#define PROG_TOMORROW_FACTOR 2.0

/*! A single program event structure */
struct program_t {
	/*! \brief Days of the week
	 *
	 * equal to _BV(struct tm->tm_wday)
	 * 0 Sunday
	 * 1 Monday
	 * 2 Tuesday
	 * 4 Wednesday
	 * 8 Thursday
	 * 16 Friday
	 * 32 Saturday
	 */
	uint8_t dow;

	/*! \brief Output line (bitmapped)
	 * 0 line 0
	 * 1 line 1
	 * 2 line 2
	 * 4 line 3
	 * 8 line 4
	 * 16 line 5
	 * 32 line 6
	 * 64 line 7
	 * 128 line 8
	 */
	uint8_t oline;

	/*! \brief start time (hours) */
	uint8_t hstart;
	/*! start time (minutes) */
	uint8_t mstart;
	/*! duration (minutes) */
	uint16_t dmin;
};

/*! \brief the queue buffer */
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
	/*! temperature realtime */
	float tnow;
	/*! temperature media */
	float tmedia;
	/*! Drifting factor */
	float dfactor;
};

#endif
