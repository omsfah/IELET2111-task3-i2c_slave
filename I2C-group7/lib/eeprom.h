/*
 * eeprom.h
 *
 * EEPROM helper functions.
 *
 * board: AVR128DB48 Curiosity Nano
 * project: IELET2111, oppgave 3: 'I2C-slave for monitorering'.
 * authors: Gruppe 7, BIELEKTRO NTNU, Trondheim. Våren 2023
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include <avr/io.h>
#include <avr/eeprom.h>

void eepromWrite(uint16_t addr, void* input, uint16_t len);
void eepromRead(uint16_t addr, void* output, uint16_t len);


#endif

