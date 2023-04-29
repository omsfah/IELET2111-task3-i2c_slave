 #include "voltages.h"
 
 /*
 * voltages.c
 *
 * Created: 27/04/2023 12:38:29
 *  Author: NTNU
 */ 


double scaleVoltage(double voltVal, double R1, double R2)
{
	return voltVal/(R2/(R1+R2));
}


uint16_t VEXT_read(void) {
	double R2 = 46150.0;
	double R1 = 220600.0;

	double volt_val = ADC0_readVoltage(PORT_D, 1);
	uint16_t vext = scaleVoltage(volt_val, R1, R2) * 1000;	// [millivolt]

	return vext;
} 

uint16_t VINT_read(void) {
	double R1 = 5600.0;
	double R2 = 5600.0;

	double volt_val = ADC0_readVoltage(PORT_D, 2);
	uint16_t vint = scaleVoltage(volt_val, R1, R2) * 1000;	// [millivolt]

	return vint;
}

