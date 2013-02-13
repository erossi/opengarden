/* This file is part of opengarden
 * Copyright (C) 2011, 2012 Enrico Rossi
 *
 * Opengarden is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Chpc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "led.h"

/*! Set led On Off or Blink.
 *
 * \param led which led.
 * \param status On Off or Blink.
 */
void led_set(const uint8_t led, const uint8_t status)
{
	switch (status) {
		case ON:
			if ((led == RED) || (led == BOTH))
				LED_PORT &= ~_BV(LED_RED);

			if ((led == GREEN) || (led == BOTH))
				LED_PORT &= ~_BV(LED_GREEN);

			break;
		case OFF:
			if ((led == RED) || (led == BOTH))
				LED_PORT |= _BV(LED_RED);

			if ((led == GREEN) || (led == BOTH))
				LED_PORT |= _BV(LED_GREEN);

			break;
		case BLINK:
			led_set(led, ON);
			LED_DELAY;
			led_set(led, OFF);
			break;
		default:
			led_set(BOTH, OFF);
	}
}

/*! Configure led IO lines. */
void led_init(void)
{
	LED_DDR |= (_BV(LED_RED) | _BV(LED_GREEN));
	led_set(BOTH, OFF);
}

/*! Deconfigure led IO lines. */
void led_shut(void)
{
	led_set(BOTH, OFF);
	LED_DDR &= ~(_BV(LED_RED) | _BV(LED_GREEN));
}
