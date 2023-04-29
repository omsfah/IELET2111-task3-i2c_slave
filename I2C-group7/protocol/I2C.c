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

volatile uint32_t last_i2c_contact_time = 0;


twi_receive_callback_t onReceive(uint8_t data) {
    /* Routine for every time we receive a byte on the I2C bus */

    if (receive_buffer_index == RECEIVE_BUFFER_SIZE-1) {
        // When the receive buffer is full, we call the command
        // parser to decide what to do with the data
        I2C_parseCommand(receive_buffer[0]);

        // After parsing the received command, we empty the receive buffer
        memset(&receive_buffer, 0, RECEIVE_BUFFER_SIZE);
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

    //memset(&receive_buffer, 0, RECEIVE_BUFFER_SIZE);
    last_i2c_contact_time = RTC_UPTIME_COUNTER_read();
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


void printBothBuffers(void) {
    // Development printing
    printf("\n");
    printf("trans: ");
    printRegister(transmission_buffer, TRANSMISSION_BUFFER_SIZE);
    printf(" recv: ");
    printRegister(receive_buffer, RECEIVE_BUFFER_SIZE);
    printf(" ");
    printf("size: %d", machine_state.machine_state_size);
}


uint8_t U8_FROM_RECV(void) {
    /* Cast the received value as uint8_t */
    return receive_buffer[4];
}

uint16_t U16_FROM_RECV(void) {
    /* Cast the received value as uint16_t */
    return receive_buffer[4] + (receive_buffer[3]<<8);
}

uint32_t U32_FROM_RECV(void) {
    /* Cast the received value as uint32_t */
    return receive_buffer[4] + (receive_buffer[3]<<8) + (receive_buffer[2]<<16) + (receive_buffer[1]<<24);
}


/* I2C transmission buffer overwrite/multiplex macro.
 *  1. sets transmission_buffer to zero and, 2. overwrites it with the data 'x'
 * 
 * This macro is used internally by the 'I2C_parseCommand' function to reset
 * and overwrite the transmission buffer during receive interrupts.
 */
#define I2C_TRANSBUF_MUX(x) {\
    memcpy(transmission_buffer, &x, sizeof(x)); \
}

//memset(transmission_buffer, 0, TRANSMISSION_BUFFER_SIZE); \
    

void I2C_parseCommand(I2C_COMMAND command) {
    /* This function takes a command received from the
     * I2C controller and does either of:
     *
     *  A: Copy certain data to the 'transmission_buffer' to be sent
     *  B: Set certain values in 'machine_state'
     *
     *  See the I2C_COMMAND type definition in datastructure.h for a full
     *  list of available commands.
     *
     *  This function is called by the receive ISR when the receive buffer
     *  is full.
     */

    cli();  // Disable interrupts while we parse the command

    switch (command) {
        // Command series 0-99: Direct control of target
        case NO_COMMAND:
            // Do nothing
        break;

        case SOFTWARE_RESET:
            RSTCTRL.SWRR = RSTCTRL_SWRST_bm;
        break;

        case USART_DEBUG_PRINT_ONCE:
            printBothBuffers();
        break;

        case TEST_ALARM:
            machine_state.buzzer_state = BUZZER_CONSTANT_ON;
        break;

        case SUM_ALARM:
            machine_state.buzzer_state = BUZZER_SUMMED;
            // We also clear the reset flag register
            RSTCTRL.RSTFR = RSTCTRL_PORF_bm | RSTCTRL_BORF_bm | RSTCTRL_EXTRF_bm
                          | RSTCTRL_SWRF_bm | RSTCTRL_UPDIRF_bm;
        break;
        
        case SAVE_THRESHOLDS_TO_EEPROM:
            eepromWrite(THRESHOLDS_EEPROM_ADDR, &machine_state.threshold, sizeof(machine_state.threshold));
        break;

        case LOAD_THRESHOLDS_FROM_EEPROM:
            eepromRead(THRESHOLDS_EEPROM_ADDR, &machine_state.threshold, sizeof(machine_state.threshold));
        break;

        case FACTORY_RESET:
            machine_state.threshold.VEXT_HIGH = FACTORY_VEXT_HIGH;
            machine_state.threshold.VEXT_LOW = FACTORY_VEXT_LOW;
            machine_state.threshold.VINT_HIGH = FACTORY_VINT_HIGH;
            machine_state.threshold.VINT_LOW = FACTORY_VINT_LOW;
            machine_state.threshold.TEMP_HIGH = FACTORY_TEMP_HIGH;
            machine_state.threshold.FAN_OFFTIME = FACTORY_FAN_OFFTIME;
            machine_state.threshold.I2C_LASTCOMTIME = FACTORY_I2C_LASTCOMTIME;
        break;

        // Command series 100-119: Controller requests transmission of a data container
        case SENDCONTAINER_MACHINE_STATE:
            I2C_TRANSBUF_MUX(machine_state);
        break;
        case SENDCONTAINER_SENSOR_DATA:
            I2C_TRANSBUF_MUX(machine_state.sensor_data);
        break;
        case SENDCONTAINER_THRESHOLDS:
            I2C_TRANSBUF_MUX(machine_state.threshold);
        break;

        // Command series 120-199: Controller requests transmission of a single value
        case SEND_VEXT:
            I2C_TRANSBUF_MUX(machine_state.sensor_data.vext);
        break;
        case SEND_VINT:
            I2C_TRANSBUF_MUX(machine_state.sensor_data.vint);
        break;
        case SEND_TEMP:
            I2C_TRANSBUF_MUX(machine_state.sensor_data.temp);
        break;
        case SEND_FAN1_FREQ:
            I2C_TRANSBUF_MUX(machine_state.sensor_data.fan1_freq);
        break;
        case SEND_FAN1_SPAN:
            I2C_TRANSBUF_MUX(machine_state.sensor_data.fan1_span);
        break;
        case SEND_FAN1_OFFTIME:
            I2C_TRANSBUF_MUX(machine_state.sensor_data.fan1_offtime);
        break;
        case SEND_FAN2_FREQ:
            I2C_TRANSBUF_MUX(machine_state.sensor_data.fan2_freq);
        break;
        case SEND_FAN2_SPAN:
            I2C_TRANSBUF_MUX(machine_state.sensor_data.fan2_span);
        break;
        case SEND_FAN2_OFFTIME:
            I2C_TRANSBUF_MUX(machine_state.sensor_data.fan2_offtime);
        break;
        case SEND_UPTIME:
            I2C_TRANSBUF_MUX(machine_state.sensor_data.uptime);
        break;
        case SEND_RESET_FLAG_REGISTER:
            I2C_TRANSBUF_MUX(machine_state.reset_flag_register);
        break;
        case SEND_ERROR_CODE:
            I2C_TRANSBUF_MUX(machine_state.error_code);
        break;

        // Command series 200-255: Controller demands a change of a threshold value
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

    machine_state.i2c_data.last_contact = RTC_UPTIME_COUNTER_read() - last_i2c_contact_time;

    // Update I2C address
    machine_state.i2c_data.address = machine_state.sensor_data.dip_switch + I2C_ADDRESS_OFFSET;
    I2C_setAddress(machine_state.i2c_data.address);
}

