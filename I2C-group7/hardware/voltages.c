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