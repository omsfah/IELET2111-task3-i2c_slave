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
}

void MACHINE_STATE_init(void) {
    // Init systems for saving machine state
    //   data structure
    //   EEPROM
    //   Reset controller?
    machine_state.machine_state_size = sizeof(machine_state);

    machine_state.error_code_has_changed = false;
}

void development_testing(void) {
    // While developing, we can test subsystems here

    /*
    printf("\n");
    printf("I2C: %d, ", machine_state.i2c_data.address);
    printf("size: %d, ", machine_state.machine_state_size);
    // Testing fan monitor library
    printf("FAN1 freq: %d, ", machine_state.sensor_data.fan1_freq);
    printf("span: %d, ", machine_state.sensor_data.fan1_span);
    printf("FAN2 freq: %d, ", machine_state.sensor_data.fan2_freq);
    printf("span: %d, ", machine_state.sensor_data.fan2_span);
    printf("\n");
    */

    //SEVEN_SEGMENT_showNumber(machine_state.sensor_data.dip_switch);

    LED_BUILTIN_toggle();
    _delay_ms(500);
}

void MACHINE_STATE_update(void) {
    /* Update 'machine_state' with new readings */

    //memset(machine_state

    // Check external voltage
    machine_state.sensor_data.vext = ADC0_readSingle(PORT_E, 0);
    machine_state.sensor_data.vint = ADC0_readSingle(PORT_E, 1);

    // Check Temperature
    machine_state.sensor_data.temp = THERMISTOR_read();

    // Check Fan RPM
    machine_state.sensor_data.fan1_freq = FAN_MONITOR_1_readFrequency();
    machine_state.sensor_data.fan2_freq = FAN_MONITOR_2_readFrequency();
    machine_state.sensor_data.fan1_span = FAN_MONITOR_1_readSpan();
    machine_state.sensor_data.fan2_span = FAN_MONITOR_2_readSpan();

    // Check I2C (save something about I2C status?)

    // Read DIP-switch
    machine_state.sensor_data.dip_switch = DIP4_read();
    machine_state.machine_state_size = sizeof(machine_state);
    //printf("\n size: %d \n", machine_state.machine_state_size);

    development_testing();
}

void ALARM_SYSTEM_update(void) {
    /* Based on the current 'machine_state', decide
     * 'error_code' and whether or not to sound the
     * buzzer alarm */

    // Decide action for the alarm state machine
    switch (machine_state.buzzer_state) {
        case BUZZER_OFF:
            // TODO: Ensure the alarm is not on
        break;
        case BUZZER_CONSTANT_ON:
            // TODO: Write a tone to the buzzer
        break;
        case BUZZER_SUMMED:
        break;
    }

    machine_state.error_code_has_changed = false;

    // Error code decision. The last code to be checked will be standing in case
    // of simultaneous errors.
    //
    // 0: First we assert there is no error
    machine_state.error_code = 0;

    // 1: checking temperature reading
    if (machine_state.sensor_data.temp > machine_state.threshold.TEMP_HIGH) {
        machine_state.error_code = ALARM_TEMPERATURE;
        machine_state.buzzer_state = BUZZER_CONSTANT_ON;
    }
    // TODO:
    // 2: ALARM_SINGLE_FAN_FAILURE
    // 3: ALARM_BOTH_FAN_FAILURE

    // 4: checking if external 12V voltage is too high
    if (machine_state.sensor_data.vext > machine_state.threshold.VEXT_HIGH) {
        machine_state.error_code = ALARM_VEXT_HIGH;
        machine_state.buzzer_state = BUZZER_CONSTANT_ON;
    }
    // 5: checking if external 12V voltage is too low
    if (machine_state.sensor_data.vext < machine_state.threshold.VEXT_LOW) {
        machine_state.error_code = ALARM_VEXT_LOW;
        machine_state.buzzer_state = BUZZER_CONSTANT_ON;
    }
    // 6: checking if internal, regulated 5V voltage is too high
    if (machine_state.sensor_data.vint > machine_state.threshold.VINT_HIGH) {
        machine_state.error_code = ALARM_VINT_HIGH;
        machine_state.buzzer_state = BUZZER_CONSTANT_ON;
    }
    // 7: checking if internal, regulated 5V voltage is too low
    if (machine_state.sensor_data.vext > machine_state.threshold.VINT_LOW) {
        machine_state.error_code = ALARM_VINT_LOW;
        machine_state.buzzer_state = BUZZER_CONSTANT_ON;
    }
    // TODO:
    // ALARM_I2C_NOCONTACT

    // If builtin button (PB2) was pushed, we sum (disable) the alarm sound
    if (machine_state.sensor_data.button_builtin == 1) {
        // FIXME: we need some more logic here or the buzzer will instantly
        //        turn back on.
        machine_state.buzzer_state = BUZZER_SUMMED;
    }

    // Write error code to the seven segment display
    //if (machine_state.error_code_has_changed)
    SEVEN_SEGMENT_showNumber(machine_state.error_code);
}

