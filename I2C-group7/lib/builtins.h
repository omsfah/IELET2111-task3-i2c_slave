/*
 *  builtins.h
 *
 *  AVR abstractions for built in LED (PB2) and push button (PB3)
 *
 * board: AVR128DB48 Curiosity Nano
 * project: IELET2111, oppgave 3: 'I2C-slave for monitorering'.
 * authors: Gruppe 7, BIELEKTRO NTNU, Trondheim. Våren 2023
 */

#ifndef BUILTINS_H_
#define BUILTINS_H_


#include <avr/io.h>


// Function prototypes
void LED_BUILTIN_init(void);
void LED_BUILTIN_toggle(void);
void LED_BUILTIN_off(void);
void LED_BUILTIN_on(void);
void BUTTON_BUILTIN_init(void);
void BUTTON_BUILTIN_awaitPress(void);


#endif
