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

/*! \file io_pin.h
 * \brief IO lines definitions.
 */

#ifndef OG_IO_OUT_H
#define OG_IO_OUT_H

#include "ogstruct.h"
#include "led.h"

#define OUT_PORT PORTA
#define OUT_PIN PINA
#define OUT_DDR DDRA

#define OUT_P0 PA0
#define OUT_P1 PA1
#define OUT_P2 PA2
#define OUT_P3 PA3
#define OUT_P4 PA4
#define OUT_P5 PA5
#define OUT_P6 PA6
#define OUT_P7 PA7

#define IN_PORT PORTD
#define IN_PIN PIND
#define IN_DDR DDRD
#define IN_P0 PIND4
#define IN_P1 PIND5

#define OUT_CMD_PORT PORTB
#define OUT_CMD_DDR DDRB
#define OUT_CMD_ONOFF PB0
#define OUT_CMD_PN PB1

#define OFF 0
#define ON 1
#define PULSE 2
#define PULSE_MSEC 100

void io_pin_init(void);
void io_out_set(const uint8_t oline, const uint8_t onoff, const uint8_t valvetype);
uint8_t io_in_allarm(void);
uint8_t io_line_in_use(void);
void io_out_off(struct programs_t *progs);

#endif
