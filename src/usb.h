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

/*! \file usb.h
 * \brief usb connection related functions
 */

#ifndef USB_H
#define USB_H

/*! usb port line. */
#define USB_PORT PORTD
/*! usb port DDR. */
#define USB_DDR DDRD
/*! usb pin port. */
#define USB_PORTIN PIND
/*! usb pin. */
#define USB_PIN PIND2

/*! true */
#define TRUE 1
/*! false */
#define FALSE 0

/*! Global used in interrupt.
 * Global usb connected flag.
 */
volatile uint8_t usb_connected;

uint8_t usb_is_connected(void);
void usb_init(void);
void usb_shutdown(void);

#endif
