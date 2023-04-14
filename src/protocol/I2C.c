#include "protocol/I2C.h"

/*
 * I2C.c
 *
 * This file implements the I2C communication routines
 * specific to the fan/voltage monitoring system.
 *
 */



twi_write_callback_t onWrite(uint8_t data) {
    /* Routine for every time we write to the I2C bus */
}

twi_read_callback_t onRead(void) {
    /* Routine for every time we read on I2C bus */
    uint8_t data = 0;
    return data;
}

twi_stop_callback_t onStop(void) {
    /* Routine for every time we end communication on I2C bus */
}

void I2C_init(twi_address_t I2C_address) {
    /* I2C target initialization procedure */
    TWI0_TARGET_init(I2C_address);

    TWI_TARGET_registerWriteCallback(onWrite);
    TWI_TARGET_registerReadCallback(onRead);
    TWI_TARGET_registerStopCallback(onStop);
}

void I2C_setAddress(twi_address_t I2C_address) {
    /* Update I2C address to new value */
}


