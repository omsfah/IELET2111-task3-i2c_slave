/*
 * hardware/init.h
 *
 * Initialization of hardware specific to the
 * I2C target fan/voltage monitoring system.
 * If subsystems grow too large, they get their
 * own file.
 *
 * board: AVR128DB48 Curiosity Nano
 *
 */

#ifndef HW_INIT_H_
#define HW_INIT_H_

#include <avr/io.h>
#include <util/delay.h>

#include "../configuration.h"

#include "../lib/twi.h"
#include "../lib/usart.h"
#include "../lib/adc.h"
#include "../lib/builtins.h"

#include "../protocol/I2C.h"
#include "../protocol/logging.h"

#include "datastructure.h"
#include "dip-switch.h"
#include "seven-segment.h"

void SYSTEMS_init(void);

void MACHINE_STATE_init(void);

void MACHINE_STATE_sensorUpdate(void);

extern machine_state_t machine_state;

#endif
