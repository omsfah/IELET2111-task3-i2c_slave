/*
 * thermistor.c
 *
 * Created: 25/04/2023 09:28:42
 *  Author: NTNU
 */ 

#include "thermistor.h"

double R0 = 33000.0;
double B = 4090.0;
double T0 = 298.25;

long double ntc_conversion(double voltVal)
{
	return (1/((1/T0)+(1/B)*log((R0*3.3/voltVal-R0)/R0)))-272.15; //Due to typecasting it needs a .0 to not return an int...
}


uint16_t THERMISTOR_read(void) {
	double adc_value = ADC0_readVoltage(PORT_F, 4);
	uint16_t temperature = ntc_conversion(adc_value) * 100;
	return temperature;
}