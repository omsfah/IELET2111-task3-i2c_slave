/*
 * protocol/I2C.h
 *
 * I2C communication routines specific to
 * the I2C target fan/voltage monitoring system.
 *
 * board: AVR128DB48 Curiosity Nano
 *
 */

#ifndef PROTOCOL_I2C_H_
#define PROTOCOL_I2C_H_

#define TRANSMISSION_BUFFER_SIZE 52
#define RECEIVE_BUFFER_SIZE 5

#include <avr/io.h>

#include "../hardware/system.h"

#include "../configuration.h"
#include "../lib/twi.h"
#include "logging.h"

void I2C_init(twi_address_t I2C_address);
void I2C_setAddress(twi_address_t I2C_address);
void I2C_SYSTEM_update();

#endif
