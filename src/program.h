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

/*! \file program.h
  \brief Functions to handle programs
  */

#ifndef PROGS_H
#define PROGS_H

#include <avr/eeprom.h>
#include "ogstruct.h"
#include "io_pin.h"
#include "debug.h"
#include "date.h"
#include "temperature.h"
#include "queue.h"

struct programs_t *prog_init(struct programs_t *progs);
void prog_free(struct programs_t *progs);
void prog_load(struct programs_t *progs);
void prog_save(struct programs_t *progs);
void prog_list(struct programs_t *progs, struct debug_t *debug);
void prog_clear(struct programs_t *progs);
void prog_add(struct programs_t *progs, const char *s);
uint8_t prog_del(struct programs_t *progs, const uint8_t n);
void prog_run(struct programs_t *progs, struct tm *tm_clock, struct debug_t *debug);
uint8_t prog_allarm(struct programs_t *progs);

#endif
