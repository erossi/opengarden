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

/*! \file i2c.h */

#ifndef I2C_DEF
#define I2C_DEF

/*! define the i2c START value */
#define START 1
/*! define the i2c STOP value */
#define STOP 2
/*! define the i2c SLA value */
#define SLA 3
/*! define the i2c DATA value */
#define DATA 4
/*! define the i2c ACK value */
#define ACK 5
/*! define the i2c NACK value */
#define NACK 6

/*! define WRITE value */
#define WRITE 0
/*! define READ value */
#define READ 1

uint8_t i2c_send(const uint8_t code, const uint8_t data);
void i2c_init(void);
void i2c_shut(void);
uint8_t i2c_master_send_b(const uint8_t addr, const uint8_t data);
uint8_t i2c_master_send_w(const uint8_t addr, const uint8_t msb, const uint8_t lsb);
uint8_t i2c_master_read_b(const uint8_t addr, uint8_t *byte);
uint8_t i2c_master_read_w(const uint8_t addr, uint16_t *code);

#endif
