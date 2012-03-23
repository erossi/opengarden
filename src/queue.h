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

/*! \file queue.h
  \brief Functions to handle queues
  */

#ifndef QUEUE_H
#define QUEUE_H

#include "ogstruct.h"
#include "debug.h"
#include "date.h"
#include "temperature.h"

void q_push(struct programs_t *progs, struct tm *tm_clock, const uint8_t i);
void queue_run(struct programs_t *progs, struct tm *tm_clock, struct debug_t *debug);
void queue_list(struct programs_t *progs, struct debug_t *debug);

#endif
