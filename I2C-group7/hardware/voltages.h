/*
 * voltages.h
 *
 * Created: 27/04/2023 12:38:47
 *  Author: NTNU
 */ 


#ifndef VOLTAGES_H_
#define VOLTAGES_H_

#include <avr/io.h>

#include "../lib/adc.h"

double scaleVoltage(double voltVal, double R1, double R2);
uint16_t VEXT_read(void);
uint16_t VINT_read(void);


#endif /* VOLTAGES_H_ */
