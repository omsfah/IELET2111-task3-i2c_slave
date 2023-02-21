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

typedef enum {
    SINGLE_CONVERSION_MODE = 0,
    FREERUN_MODE = 1
} ADC_MODE;

void ADC0_init(uint8_t pin, ADC_MODE adc_mode);
uint16_t ADC0_readSingle(uint8_t pin);

uint16_t ADC0_read(uint8_t pin);
void ADC0_start(void);
bool ADC0_conversionDone(void);


#endif
