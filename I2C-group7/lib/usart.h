/*
 * usart.h
 *
 * AVR abstraction for serial communication over USART
 *
 * board: AVR128DB48 Curiosity Nano
 * project: IELET2111, oppgave 3: 'I2C-slave for monitorering'.
 * authors: Gruppe 7, BIELEKTRO NTNU, Trondheim. Våren 2023
 */


#ifndef USART_H_
#define USART_H_

// 4 MHz CPU clock speed
#define F_CPU 4000000UL

// Baud rate calculation for "USART Asynchronous" mode. From Microchip application note
// "Getting started with USART". Also supplied in avr128DB48 datasheet.
#define USART3_BAUD_RATE(BAUD_RATE)     ((float)(64 * F_CPU / (16 * (float)BAUD_RATE)) + 0.5)

#include <avr/io.h>
#include <stdio.h>
#include <string.h>

// Function prototypes
void USART3_init(void);
void USART3_sendChar(char c);
void USART3_sendString(char* str);
uint8_t USART3_printChar(char c, FILE* stream);

// Output stream to which we can point printf
extern FILE USART3_stream;

#endif
