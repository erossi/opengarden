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

/*! \file main.c
  \brief Main.
 */

#ifndef OG_IO_OUT_H
#define OG_IO_OUT_H

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

void io_out_init(void);
void io_out_set(const uint8_t pin, const uint8_t val);
uint8_t io_out_get(const uint8_t pin);
void io_out_change_line(const uint8_t oline, const uint8_t onoff);

#endif
