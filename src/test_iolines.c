/* This file is part of OpenGarden
 * Copyright (C) 2014 Enrico Rossi
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

/*! \file test_iolines.c
 * \brief Test all the IO lines in sequence.
 */

#include <stdlib.h>
#include <util/delay.h>
#include "ogstruct.h"
#include "led.h"
#include "io_pin.h"

/*! main */
int main(void)
{
	struct programs_t *progs;
	uint8_t i;

	/* anti warning for non initialized variables */
	progs = malloc(sizeof(struct programs_t));
	progs->valve = BISTABLE;
	progs->ioline = 0;

	/* Init sequence, turn on both led */
	led_init();
	led_set(BOTH, ON);
	_delay_ms(1000);
	io_init();
	led_set(BOTH, OFF);

	while (1) {
		for (i=0; i<8; i++) {
			led_set(RED, ON);
			io_set(i, ON, progs);
			_delay_ms(1000);
			io_set(i, OFF, progs);
			led_set(BOTH, OFF);
			_delay_ms(1000);
		}
	}

	return(0);
}
