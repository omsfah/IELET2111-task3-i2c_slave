/*
 * adc.h
 *
 * AVR abstractions for analog to digital conversion (ADC)
 *
 * board: AVR128DB48 Curiosity Nano
 *
 */


#ifndef ADC_H_
#define ADC_H_

#define RTC_PERIOD  511

#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>

extern uint16_t adcVal;

typedef enum{
	PORT_D = 0,
	PORT_E = 1,
	PORT_F =2
	} PORT_SELECT;

typedef enum {
    SINGLE_CONVERSION_MODE = 0,
    FREERUN_MODE = 1
} ADC_MODE;

void ADC0_init(PORT_SELECT port, uint8_t pin, ADC_MODE adc_mode);
uint16_t ADC0_readSingle(PORT_SELECT port, uint8_t pin);
double ADC0_readVoltage(PORT_SELECT port, uint8_t pin);
void ADC0_start(void);
bool ADC0_conversionDone(void);


#endif
