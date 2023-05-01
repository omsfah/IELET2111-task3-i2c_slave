/*
 * protocol/I2C.h
 *
 * I2C communication routines specific to
 * the I2C target fan/voltage monitoring system.
 *
 * board: AVR128DB48 Curiosity Nano
 * project: IELET2111, oppgave 3: 'I2C-slave for monitorering'.
 * authors: Gruppe 7, BIELEKTRO NTNU, Trondheim. Våren 2023
 */

#ifndef PROTOCOL_I2C_H_
#define PROTOCOL_I2C_H_

#include <avr/io.h>

#include "../hardware/system.h"

#include "../configuration.h"
#include "../lib/twi.h"
#include "logging.h"

static inline void I2C_parseCommand(void);
void I2C_init(twi_address_t I2C_address);
void I2C_setAddress(twi_address_t I2C_address);
void I2C_SYSTEM_update(void);

#endif
