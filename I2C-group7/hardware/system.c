#include "system.h"

// Here we instantiate the global machine state data structure
machine_state_t machine_state;


void SYSTEMS_init(void) {
    BUTTON_BUILTIN_init();
    LED_BUILTIN_init();
    DIP4_init();
    SEVEN_SEGMENT_init();
    FAN_MONITOR_init();

    USART3_init();
    stdout = &USART3_stream;    // Move to usart.h if possible

    // Generic ADC initializations
    ADC0_init(PORT_E, 0, SINGLE_CONVERSION_MODE);   // VEXT (External 12V supply)
    ADC0_init(PORT_E, 1, SINGLE_CONVERSION_MODE);   // VINT (Internal 5V supply)
    ADC0_init(PORT_E, 2, SINGLE_CONVERSION_MODE);   // TEMP (TMP36)

    // Init brownout detection
    // Init watchdog
    // Init fan monitoring
    // Init voltage monitoring
}

void MACHINE_STATE_init(void) {
    // Init systems for saving machine state
    //   data structure
    //   EEPROM
    //   Reset controller?

    machine_state.error_code_has_changed = false;
}

void development_testing(void) {
    // While developing, we can test subsystems here

    uint8_t data_buffer[9];     // TODO: temporary until we use the struct
    //printf("VEXT: %d, ", machine_state.sensor_data.vext);
    //printf("DIP: %d \r\n", machine_state.sensor_data.dip_switch);

    // If master polls for information, send the register
    /*
    if (machine_state.i2c_data.data_ready) {
        // (Temporary stuff for testing I2C)
        cli();
        memcpy(data_buffer, (const uint8_t *)transmission_buffer, 8);
        sei();
        data_buffer[8] = '\0';
        printf("Received: %s\n", data_buffer);
        SEVEN_SEGMENT_showNumber(data_buffer[0] - 48);
        machine_state.i2c_data.data_ready = false;
    }
    */

    // Write directly to I2C transmission_buffer for testing
    transmission_buffer[0] = 13;
    transmission_buffer[1] = 37;

    // Testing fan monitor library
    uint16_t fan1_freq = FAN_MONITOR_1_readFrequency();
    uint16_t fan1_span = FAN_MONITOR_1_readSpan();
    //printf("FAN1: %d, stddev: %d\n", fan1_freq, fan1_span);

    uint16_t fan2_freq = FAN_MONITOR_2_readFrequency();
    uint16_t fan2_span = FAN_MONITOR_2_readSpan();
    printf("FAN2: %d, stddev: %d\n", fan2_freq, fan2_span);

    LED_BUILTIN_toggle();
    //_delay_ms(100);
}

void MACHINE_STATE_update(void) {
    /* Update 'machine_state' with new readings */

    // Check external voltage
    machine_state.sensor_data.vext = ADC0_readSingle(PORT_E, 0);
    machine_state.sensor_data.vint = ADC0_readSingle(PORT_E, 1);

    // Check Temperature
    machine_state.sensor_data.temp = ADC0_readSingle(PORT_E, 2);

    // Check Fan RPM

    // Check I2C (save something about I2C status?)

    // Update I2C address from DIP-switch
    machine_state.sensor_data.dip_switch = DIP4_read();
    machine_state.i2c_data.address = machine_state.sensor_data.dip_switch + I2C_ADDRESS_OFFSET;


    development_testing();
}

void ALARM_SYSTEM_update(void) {
    /* Based on the current 'machine_state', decide
     * 'error_code' and whether or not to sound the
     * buzzer alarm */

    // TODO: implement

    // Check if we exceed any threshold

    // If builtin button (PB2) was pushed, we sum (disable) the alarm sound

    // Update the buzzer state

    // Write error code to the seven segment display
    if (machine_state.error_code_has_changed)
        SEVEN_SEGMENT_showNumber(machine_state.error_code);
}
