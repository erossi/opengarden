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

/*! \file date.h
  \brief time high level functions.
  */

#ifndef DATE_H
#define DATE_H

#include "debug.h"
#include "time.h"

void date_set(struct tm *date, struct debug_t *debug);
struct tm *date_init(struct tm *tm_clock, struct debug_t *debug);
void date_free(struct tm *tm_clock);
void date(struct debug_t *debug);
void date_hwclock_start(void);
void date_hwclock_stop(void);
uint8_t date_timetorun(struct tm *tm_clock, struct debug_t *debug);

#endif
