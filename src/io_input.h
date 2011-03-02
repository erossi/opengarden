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

#ifndef OG_IOIN_H
#define OG_IOIN_H

#define IN_PORT PORTD
#define IN_PIN PIND
#define IN_DDR DDRD
#define IN_P0 PIND4
#define IN_P1 PIND5
#define IN_P2 PIND6
#define IN_P3 PIND7

void io_in_init(void);
uint8_t io_in_get(const uint8_t port);

#endif
