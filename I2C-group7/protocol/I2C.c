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

    // Reset the buffer array indexes
    receive_buffer_index = 0;
    transmission_buffer_index = 0;

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

uint8_t U8_FROM_RECV(void) {
    /* Cast the received value as uint8_t */
    return receive_buffer[5];
}

uint16_t U16_FROM_RECV(void) {
    /* Cast the received value as uint16_t */
    return receive_buffer[5] + receive_buffer[4]*256;
}

uint32_t U32_FROM_RECV(void) {
    /* Cast the received value as uint32_t */
    return receive_buffer[5] + receive_buffer[4]*256 + receive_buffer[3]*65536 + receive_buffer[2]*16777216;
}


void I2C_parseCommand(I2C_COMMAND command) {
    /* This function takes a command received from the
     * I2C controller and does either of:
     *
     *  A: Copy certain data to the 'transmission_buffer' to be sent
     *  B: Set certain values in 'machine_state'
     *
     *  See the I2C_COMMAND type definition in datastructure.h for a full
     *  list of available commands.
     */

    cli();  // Disable interrupts while we parse the command

    switch (command) {
        // Command series 0-9: Direct control of target
        case NO_COMMAND:
            // Do nothing
        break;

        case REBOOT:
            // TODO: not implemented
        break;

        case TEST_ALARM:
            // TODO: not implemented
        break;

        // Command series 10-19: Controller requests transmission of a data container
        case SENDCONTAINER_MACHINE_STATE:
            memcpy(transmission_buffer, &machine_state, TRANSMISSION_BUFFER_SIZE);
        break;
        case SENDCONTAINER_SENSOR_DATA:
            memcpy(transmission_buffer, &machine_state.sensor_data, sizeof(machine_state.sensor_data));
        break;
        case SENDCONTAINER_THRESHOLDS:
            memcpy(transmission_buffer, &machine_state.threshold, sizeof(machine_state.threshold));
        break;
        // Command series 20-99: Controller requests transmission of a single value
        case SEND_VEXT:
            memcpy(transmission_buffer, &machine_state.sensor_data.vext, sizeof(machine_state.sensor_data.vext));
        break;
        case SEND_VINT:
            memcpy(transmission_buffer, &machine_state.sensor_data.vint, sizeof(machine_state.sensor_data.vint));
        break;
        case SEND_TEMP:
            memcpy(transmission_buffer, &machine_state.sensor_data.temp, sizeof(machine_state.sensor_data.temp));
        break;
        case SEND_FAN1_FREQ:
            memcpy(transmission_buffer, &machine_state.sensor_data.fan1_freq, sizeof(machine_state.sensor_data.fan1_freq));
        break;
        case SEND_FAN1_SPAN:
            memcpy(transmission_buffer, &machine_state.sensor_data.fan1_span, sizeof(machine_state.sensor_data.fan1_span));
        break;
        case SEND_FAN1_OFFTIME:
            memcpy(transmission_buffer, &machine_state.sensor_data.fan1_offtime, sizeof(machine_state.sensor_data.fan1_offtime));
        break;
        case SEND_FAN2_FREQ:
            memcpy(transmission_buffer, &machine_state.sensor_data.fan2_freq, sizeof(machine_state.sensor_data.fan2_freq));
        break;
        case SEND_FAN2_SPAN:
            memcpy(transmission_buffer, &machine_state.sensor_data.fan2_span, sizeof(machine_state.sensor_data.fan2_span));
        break;
        case SEND_FAN2_OFFTIME:
            memcpy(transmission_buffer, &machine_state.sensor_data.fan2_offtime, sizeof(machine_state.sensor_data.fan2_offtime));
        break;
        case SEND_UPTIME:
            memcpy(transmission_buffer, &machine_state.sensor_data.uptime, sizeof(machine_state.sensor_data.uptime));
        break;
        case SEND_ALARM_STATE:
            memcpy(transmission_buffer, &machine_state.alarm_state, sizeof(machine_state.alarm_state));
        break;
        case SEND_ERROR_CODE:
            memcpy(transmission_buffer, &machine_state.error_code, sizeof(machine_state.error_code));
        break;

        // Command series 100-150: Controller demands a change of a threshold value
        case SET_THRESHOLD_VEXT_HIGH:
            machine_state.threshold.VEXT_HIGH = U16_FROM_RECV();
        break;
        case SET_THRESHOLD_VEXT_LOW:
            machine_state.threshold.VEXT_LOW = U16_FROM_RECV();
        break;
        case SET_THRESHOLD_VINT_HIGH:
            machine_state.threshold.VINT_HIGH = U16_FROM_RECV();
        break;
        case SET_THRESHOLD_VINT_LOW:
            machine_state.threshold.VINT_LOW = U16_FROM_RECV();
        break;
        case SET_THRESHOLD_TEMP_HIGH:
            machine_state.threshold.TEMP_HIGH = U16_FROM_RECV();
        break;
        case SET_THRESHOLD_FAN_OFFTIME:
            machine_state.threshold.FAN_OFFTIME = U16_FROM_RECV();
        break;
        case SET_THRESHOLD_I2C_LASTCOMTIME:
            machine_state.threshold.I2C_LASTCOMTIME = U16_FROM_RECV();
        break;
    }

    sei();  // Re-enable interrupts
}


void I2C_SYSTEM_update(void) {
    /* I2C housekeeping function exposed to 'main' */

    // We reset the transmission buffer with zeros
    //memset(transmission_buffer, 0, TRANSMISSION_BUFFER_SIZE);

    cli();
    memcpy(transmission_buffer, &machine_state, TRANSMISSION_BUFFER_SIZE);  // remove this
    sei();

    // Here we parse and execute the command if there is any
    if (machine_state.i2c_data.new_settings_flag) {
        //I2C_parseCommand(receive_buffer[0]);
        machine_state.i2c_data.new_settings_flag = false;
        //memset(receive_buffer, 0, RECEIVE_BUFFER_SIZE);
    }

    //machine_state.i2c_data.last_contact = timer_sample();

    printf("CMD: %d, ", receive_buffer[0]);
    printf("trans: ");
    printRegister(transmission_buffer, TRANSMISSION_BUFFER_SIZE);
    printf("recv: ");
    printRegister(receive_buffer, RECEIVE_BUFFER_SIZE);
    printf("\n");

    // Update I2C address
    machine_state.i2c_data.address = machine_state.sensor_data.dip_switch + I2C_ADDRESS_OFFSET;
    I2C_setAddress(machine_state.i2c_data.address);
}

