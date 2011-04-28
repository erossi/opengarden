/* Copyright (C) 2011 Enrico Rossi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include "i2c.h"

uint8_t i2c_send_start(void)
{
	/* send start condition */
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
	/* wait for transmission */
	loop_until_bit_is_set(TWCR, TWINT);
	return(TW_STATUS);
}

void i2c_send_stop(void)
{
	/* send stop condition */
	TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
}

uint8_t i2c_send_sla_w(const uint8_t addr)
{
	TWDR = addr;
	/* clear interrupt to start transmission */
	TWCR = _BV(TWINT) | _BV(TWEN); 
	/* wait for transmission */
	loop_until_bit_is_set(TWCR, TWINT);
	return(TW_STATUS);
}

uint8_t i2c_send_sla_r(const uint8_t addr)
{
	TWDR = addr;
	/* clear interrupt to start transmission */
	TWCR = _BV(TWINT) | _BV(TWEN); 
	/* wait for transmission */
	loop_until_bit_is_set(TWCR, TWINT);
	return(TW_STATUS);
}

uint8_t i2c_send_data(const uint8_t data)
{
	TWDR = data;
	/* clear interrupt to start transmission */
	TWCR = _BV(TWINT) | _BV(TWEN); 
	/* wait for transmission */
	loop_until_bit_is_set(TWCR, TWINT);
	return(TW_STATUS);
}

uint8_t i2c_send_ack(void)
{
	TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
	/* wait for transmission */
	loop_until_bit_is_set(TWCR, TWINT);
	return(TW_STATUS);
}

uint8_t i2c_send_nack(void)
{
	TWCR = _BV(TWINT) | _BV(TWEN);
	/* wait for transmission */
	loop_until_bit_is_set(TWCR, TWINT);
	return(TW_STATUS);
}

/*! Initialize the i2c bus */
void i2c_init(void)
{
	TWSR = 3;
	TWBR = 32;
}

/*! Send a byte to the i2c slave.
 * \param addr address of the slave.
 * \param data byte to send.
 */
uint8_t i2c_master_send_b(const uint8_t addr, const uint8_t data)
{
	uint8_t err;

	/* Send ADDR_W reg. 0 req. */
	err = i2c_send_start();

	if (err == TW_START)
		err = i2c_send_sla_w(addr);
	else
		return(err);

	if (err == TW_MT_SLA_ACK)
		err = i2c_send_data(0);
	else
		return(err);

	if (err == TW_MT_DATA_ACK)
		return(0);
	else
		return(err);
}

/*! Read a word (2 byte) from the slave.
 * \param addr address of the slave.
 * \param pre-allocated word.
 * \return 1 - value OK, 0 - Error.
 */
uint8_t i2c_master_read_w(const uint8_t addr, int16_t *word)
{
	uint8_t err;

	err = i2c_send_start();

	if (err == TW_REP_START)
		err = i2c_send_sla_r(addr);

	if (err == TW_MR_SLA_ACK)
		err = i2c_send_ack();

	if (err == TW_MR_DATA_ACK) {
		*word = TWDR << 8;
		err = i2c_send_nack();
	}

	if (err == TW_MR_DATA_NACK) {
		*word |= TWDR;
		err = 0;
	}

	i2c_send_stop();
	return(err);
}
