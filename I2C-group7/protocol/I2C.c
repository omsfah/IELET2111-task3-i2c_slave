#include "I2C.h"

/*
 * protocol/I2C.c
 *
 * This file implements the I2C communication routines
 * specific to the fan/voltage monitoring system.
 *
 */

//volatile i2c_status_packet_t incoming_buffer = {};
#define DATA_SIZE 8
volatile uint8_t incoming_buffer[DATA_SIZE];
volatile uint8_t buffer_index = 0;

volatile i2c_status_packet_t outgoing_buffer = {};

volatile bool was_read = false;
volatile bool data_ready = false;



twi_receive_callback_t onReceive(uint8_t data) {
    /* Routine for every time we receive to the I2C bus */

    // We choose the action depending on the register byte
    if(buffer_index < DATA_SIZE) {
        incoming_buffer[buffer_index] = data;
        buffer_index++;
    }
    //HELLO = data;
    //LOG("receive callback function\n");
}

twi_transmit_callback_t onTransmit(void) {
    /* Routine for every time we transmit from I2C bus */
    //LOG("transmit callback function\n");
    was_read = true;
    return 100;
}

twi_stop_callback_t onStop(void) {
    /* Routine for every time we end communication on I2C bus */
    buffer_index = 0;
    //LOG("Stop callback function\n");
    if(!was_read) {
        data_ready = true;
    }
    else {
        was_read = false;
    }
}

void I2C_init(twi_address_t I2C_address) {
    /* I2C target initialization procedure */
    TWI0_TARGET_init(I2C_address);

    TWI_TARGET_registerReceiveCallback(&onReceive);
    TWI_TARGET_registerTransmitCallback(&onTransmit);
    TWI_TARGET_registerStopCallback(&onStop);
}

void I2C_setAddress(twi_address_t I2C_address) {
    /* Update I2C address to provided value */
    TWI0_TARGET_updateAddress(I2C_address);
}

