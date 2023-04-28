/*
 * Thermistor.h
 *
 * Created: 25/04/2023 09:28:23
 *  Author: NTNU
 */ 


#ifndef THERMISTOR_H_
#define THERMISTOR_H_

#include <stdio.h>
#include <math.h>

#include "../lib/adc.h"

long double ntc_conversion(double voltVal);
uint16_t THERMISTOR_read(void);

#endif /* THERMISTOR_H_ */