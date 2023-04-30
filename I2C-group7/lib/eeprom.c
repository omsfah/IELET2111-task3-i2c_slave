#include "eeprom.h"


void eepromWrite(uint16_t addr, void* input, uint16_t len) {
    /* Write an array to EEPROM by repeated calls to the
     * avr 'eeprom_write_byte' function
     *
     * Credit: Coby Kaufer on StackOverflow */

    uint8_t* src;
    uint8_t* dst;
    src = (uint8_t*) input;
    dst = (uint8_t*) addr;

    for (uint16_t i = 0; i < len; i++) {
        eeprom_write_byte(dst++, *src++);
    }
}


void eepromRead(uint16_t addr, void* output, uint16_t len) {
    /* Read an array from EEPROM */

    uint8_t* src;
    uint8_t* dst;
    src = (uint8_t*) addr;
    dst = (uint8_t*) output;

    for (uint16_t i = 0; i < len; i++) {
        *dst++ = eeprom_read_byte(src++);
    }
}

