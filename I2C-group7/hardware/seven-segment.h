/*
 * hardware/seven-segment.h
 *
 * Initialization of generic seven segment display.
 * Implementation is specific to the fan/voltage
 * monitoring system.
 *
 * board: AVR128DB48 Curiosity Nano
 * project: IELET2111, oppgave 3: 'I2C-slave for monitorering'.
 * authors: Gruppe 7, BIELEKTRO NTNU, Trondheim. Våren 2023
 */

#ifndef SEVEN_SEGMENT_H_
#define SEVEN_SEGMENT_H_

#include <avr/io.h>

void SEVEN_SEGMENT_init(void);
void SEVEN_SEGMENT_showNumber(int8_t number);

#endif
