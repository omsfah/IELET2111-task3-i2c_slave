/*
 * voltages.h
 *
 * Functions for reading voltages.
 * Implementation is specific to the fan/voltage monitoring system.
 *
 * board: AVR128DB48 Curiosity Nano
 * project: IELET2111, oppgave 3: 'I2C-slave for monitorering'.
 * authors: Gruppe 7, BIELEKTRO NTNU, Trondheim. Våren 2023
 */ 


#ifndef VOLTAGES_H_
#define VOLTAGES_H_

#include <avr/io.h>

#include "../lib/adc.h"


uint16_t VEXT_read(void);
uint16_t VINT_read(void);


#endif /* VOLTAGES_H_ */
