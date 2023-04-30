/*
 * dip-switch.h
 *
 * Driver for a generic 4-bit DIP-switch.
 * Implementation is specific to the
 * fan/voltage monitoring system.
 *
 * board: AVR128DB48 Curiosity Nano
 * project: IELET2111, oppgave 3: 'I2C-slave for monitorering'.
 * authors: Gruppe 7, BIELEKTRO NTNU, Trondheim. Våren 2023
 */

#ifndef DIP_SWITCH_H_
#define DIP_SWITCH_H_

#include <avr/io.h>

void DIP4_init(void);
uint8_t DIP4_read(void);

#endif

