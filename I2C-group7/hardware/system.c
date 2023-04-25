#include "system.h"

// Here we instantiate the global machine state data structure
machine_state_t machine_state;

// We instantiate the alarm buzzer state machine
typedef enum {
    BUZZER_OFF = 0,
    BUZZER_CONSTANT_ON = 1,
    BUZZER_SUMMED = 2
} BUZZER_STATE_T;
BUZZER_STATE_T buzzer_state;


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

    //SEVEN_SEGMENT_showNumber(machine_state.sensor_data.dip_switch);

    LED_BUILTIN_toggle();
    _delay_ms(100);
}

void MACHINE_STATE_update(void) {
    /* Update 'machine_state' with new readings */

    // Check external voltage
    machine_state.sensor_data.vext = ADC0_readSingle(PORT_E, 0);
    machine_state.sensor_data.vint = ADC0_readSingle(PORT_E, 1);

    // Check Temperature
    machine_state.sensor_data.temp = ADC0_readSingle(PORT_E, 2);

    // Check Fan RPM
    machine_state.sensor_data.fan1_freq = FAN_MONITOR_1_readFrequency();
    machine_state.sensor_data.fan2_freq = FAN_MONITOR_2_readFrequency();
    machine_state.sensor_data.fan1_span = FAN_MONITOR_1_readSpan();
    machine_state.sensor_data.fan2_span = FAN_MONITOR_2_readSpan();

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

    // Decide action for the alarm state machine
    switch (buzzer_state) {
        case BUZZER_OFF:
            // TODO: Ensure the alarm is not on
        break;
        case BUZZER_CONSTANT_ON:
            // TODO: Write a tone to the buzzer
        break;
        case BUZZER_SUMMED:
        break;
    }

    // Error code decision
    if (machine_state.sensor_data.vext > machine_state.threshold.VEXT_HIGH) {
        machine_state.error_code = ALARM_VEXT_HIGH;
        buzzer_state = BUZZER_CONSTANT_ON;
    }
    if (machine_state.sensor_data.vext < machine_state.threshold.VEXT_LOW) {
        machine_state.error_code = ALARM_VEXT_LOW;
        buzzer_state = BUZZER_CONSTANT_ON;
    }
    if (machine_state.sensor_data.vint > machine_state.threshold.VINT_HIGH) {
        machine_state.error_code = ALARM_VINT_HIGH;
        buzzer_state = BUZZER_CONSTANT_ON;
    }
    if (machine_state.sensor_data.vext > machine_state.threshold.VINT_LOW) {
        machine_state.error_code = ALARM_VINT_LOW;
        buzzer_state = BUZZER_CONSTANT_ON;
    }
    if (machine_state.sensor_data.temp > machine_state.threshold.TEMP_HIGH) {
        machine_state.error_code = ALARM_TEMPERATURE;
        buzzer_state = BUZZER_CONSTANT_ON;
    }
    // TODO:
    // ALARM_SINGLE_FAN_FAILURE
    // ALARM_BOTH_FAN_FAILURE
    // ALARM_I2C_NOCONTACT

    // If builtin button (PB2) was pushed, we sum (disable) the alarm sound
    if (machine_state.sensor_data.button_builtin == 1) {
        // FIXME: we need some more logic here or the buzzer will instantly
        //        turn back on.
        buzzer_state = BUZZER_SUMMED;
    }

    // Write error code to the seven segment display
    //if (machine_state.error_code_has_changed)
    SEVEN_SEGMENT_showNumber(machine_state.error_code);
}
