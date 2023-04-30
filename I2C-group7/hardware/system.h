/*
 * hardware/system.h
 *
 * Main branchpoint for initialization and drivers
 * of components specific to the I2C target fan/voltage
 * monitoring system. If subsystems grow too large, they
 * get their own file in the 'hardware' folder.
 * The global 'machine_state' data container is provided
 * with this file.
 *
 * board: AVR128DB48 Curiosity Nano
 * project: IELET2111, oppgave 3: 'I2C-slave for monitorering'.
 * authors: Gruppe 7, BIELEKTRO NTNU, Trondheim. Våren 2023
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <avr/io.h>
#include <util/delay.h>

#include "../configuration.h"

#include "../lib/twi.h"
#include "../lib/usart.h"
#include "../lib/adc.h"
#include "../lib/builtins.h"
#include "../lib/eeprom.h"
#include "../lib/rtc.h"

#include "../protocol/I2C.h"
#include "../protocol/logging.h"

#include "datastructure.h"
#include "dip-switch.h"
#include "seven-segment.h"
#include "thermistor.h"
#include "voltages.h"

void SYSTEMS_init(void);

void MACHINE_STATE_init(void);

void MACHINE_STATE_update(void);

void ALARM_SYSTEM_update(void);

extern machine_state_t machine_state;

#endif
