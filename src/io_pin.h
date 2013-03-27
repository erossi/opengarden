/* This file is part of OpenGarden
 * Copyright (C) 2011-2013 Enrico Rossi
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

/*! \file io_pin.h
 * \brief IO lines definitions.
 */

#ifndef OG_IO_OUT_H
#define OG_IO_OUT_H

#include "ogstruct.h"

/*! the IO line port */
#define OUT_PORT PORTA
/*! IO PIN */
#define OUT_PIN PINA
/*! IO DDR */
#define OUT_DDR DDRA

/*! IO line 0 */
#define OUT_P0 PA0
/*! IO line 1 */
#define OUT_P1 PA1
/*! IO line 2 */
#define OUT_P2 PA2
/*! IO line 3 */
#define OUT_P3 PA3
/*! IO line 4 */
#define OUT_P4 PA4
/*! IO line 5 */
#define OUT_P5 PA5
/*! IO line 6 */
#define OUT_P6 PA6
/*! IO line 7 */
#define OUT_P7 PA7

/*! Alarm port */
#define IN_PORT PORTD
/*! Alarm PIN */
#define IN_PIN PIND
/*! Alarm DDR */
#define IN_DDR DDRD
/*! Alarm line 0 */
#define IN_P0 PIND4
/*! Alarm line 1 */
#define IN_P1 PIND5

/*! Command PORT */
#define OUT_CMD_PORT PORTB
/*! Command DDR */
#define OUT_CMD_DDR DDRB
/*! Command On Off line 0 */
#define OUT_CMD_ONOFF PB0
/*! Command +- line */
#define OUT_CMD_PN PB1

/*! Macro OFF */
#define OFF 0
/*! Macro ON */
#define ON 1
/*! Macro PULSE */
#define PULSE 2
/*! PULSE msec delay */
#define PULSE_MSEC 50

void io_init(void);
void io_shut(void);
void io_set(const uint8_t oline, const uint8_t onoff, struct programs_t *progs);
uint8_t io_get(struct programs_t *progs);
void io_off(struct programs_t *progs);
uint8_t io_alarm(struct programs_t *progs);

#endif
