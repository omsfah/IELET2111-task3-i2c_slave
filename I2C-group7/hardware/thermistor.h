/*
 * thermistor.h
 *
 * Read temperature from NTC thermistor.
 * Implementation is specific to the fan/voltage monitoring system.
 *
 * board: AVR128DB48 Curiosity Nano
 * project: IELET2111, oppgave 3: 'I2C-slave for monitorering'.
 * authors: Gruppe 7, BIELEKTRO NTNU, Trondheim. Våren 2023
 */ 


#ifndef THERMISTOR_H_
#define THERMISTOR_H_

#include <stdio.h>
#include <math.h>

#include "../lib/adc.h"

uint16_t THERMISTOR_read(void);

#endif /* THERMISTOR_H_ */
