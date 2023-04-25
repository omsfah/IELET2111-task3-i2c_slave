#include "I2C.h"

/*
 * protocol/I2C.c
 *
 * This file implements the I2C communication routines
 * specific to the fan/voltage monitoring system.
 *
 */

volatile uint8_t transmission_buffer_index = 0;
volatile uint8_t receive_buffer_index = 0;
volatile uint8_t transmission_buffer[TRANSMISSION_BUFFER_SIZE];
volatile uint8_t receive_buffer[RECEIVE_BUFFER_SIZE];


twi_receive_callback_t onReceive(uint8_t data) {
    /* Routine for every time we receive a byte on the I2C bus */

    if (receive_buffer_index == RECEIVE_BUFFER_SIZE) {
        machine_state.i2c_data.new_settings_flag = true;
    }

    // As long as there is space in the buffer, we write the byte
    // to it and increment the index.
    if (receive_buffer_index < RECEIVE_BUFFER_SIZE) {
        receive_buffer[receive_buffer_index] = data;
        receive_buffer_index++;
    }
    // If there is no space in the buffer, we discard the byte
}


twi_transmit_callback_t onTransmit(void) {
    /* Routine for every time we transmit from I2C bus */

    // We keep sending the n'th byte of the transmission buffer
    if (transmission_buffer_index < TRANSMISSION_BUFFER_SIZE) {
        return transmission_buffer[transmission_buffer_index++];
    }
    else {
        // It is the controllers responsibility to not read more bytes
        // than are available. If it still insists, we feed it with zeros.
        return 0;
    }
}

twi_stop_callback_t onStop(void) {
    /* Routine for every time we end communication on I2C bus */

    receive_buffer_index = 0;   // Reset the buffer array index
    transmission_buffer_index = 0;   // Reset the buffer array index

    if (!machine_state.i2c_data.data_was_read) {
        machine_state.i2c_data.data_ready = true;
    }
    else {
        machine_state.i2c_data.data_was_read = false;
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


void printRegister(uint8_t array[], uint8_t len) {
    /* Print the array in HEX coding, 4 bytes at a time */
    static const group_by = 4;

    for (uint8_t i = 0; i < len; i++) {
        printf("%02X", array[i]);
        if((i+1) % group_by == 0) {
            printf(" ");
        }
    }
}


void I2C_SYSTEM_update(void) {
    /* I2C housekeeping function exposed to 'main' */

    // TODO: implement

    // If the 'new_settings_flag' is set, copy the new
    // threshold values over to the 'machine_state'

    //machine_state.i2c_data.last_contact = timer_sample();
    memcpy(transmission_buffer, &machine_state, TRANSMISSION_BUFFER_SIZE);

    printf("trans: ");
    printRegister(transmission_buffer, TRANSMISSION_BUFFER_SIZE);
    printf(", ");
    printf("recv: ");
    printRegister(receive_buffer, RECEIVE_BUFFER_SIZE);
    printf(", ");

    // Update I2C address
    I2C_setAddress(machine_state.i2c_data.address);
}

