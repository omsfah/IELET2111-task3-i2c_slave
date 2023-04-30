/*
 * voltages.h
 *
 * Functions for reading voltages.
 * Implementation is specific to the fan/voltage monitoring system.
 *
 * board: AVR128DB48 Curiosity Nano
 */ 


#ifndef VOLTAGES_H_
#define VOLTAGES_H_

#include <avr/io.h>

#include "../lib/adc.h"


uint16_t VEXT_read(void);
uint16_t VINT_read(void);


#endif /* VOLTAGES_H_ */
