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

uint8_t i2c_send_start(void);
void i2c_send_stop(void);
uint8_t i2c_send_sla_w(const uint8_t addr);
uint8_t i2c_send_sla_r(const uint8_t addr);
uint8_t i2c_send_data(const uint8_t data);
uint8_t i2c_send_ack(void);
uint8_t i2c_send_nack(void);
void i2c_init(void);
uint8_t i2c_master_send_b(const uint8_t addr, const uint8_t data);
uint8_t i2c_master_read_w(const uint8_t addr, int16_t *word);

#endif
