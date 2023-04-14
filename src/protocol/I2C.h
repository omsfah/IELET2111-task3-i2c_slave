/*
 * protocol/I2C.h
 *
 * Abstractions of I2C communication, specific
 * to the I2C target fan/voltage monitoring system.
 *
 * board: AVR128DB48 Curiosity Nano
 *
 */

#ifndef PROTOCOL_I2C_H_
#define PROTOCOL_I2C_H_

#include <avr/io.h>

#include "protocol/I2C_packet.h"

#include "configuration.h"
#include "lib/twi.h"

// Init I2C
void I2C_init(twi_address_t I2C_address);
void I2C_setAddress(twi_address_t I2C_address);

// I2C status function that returns I2C_status_t struct?

// Make transmission buffer available?
extern volatile i2c_status_packet_t incoming_buffer;
extern volatile i2c_status_packet_t outgoing_buffer;

#endif
