#include "init.h"

// Here we instantiate the global machine state data structure
machine_state_t machine_state;


void SYSTEMS_init(void) {
    BUTTON_BUILTIN_init();
    LED_BUILTIN_init();
    DIP4_init();
    SEVEN_SEGMENT_init();

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
}

void development_testing(void) {
    // While developing, we can test subsystems here

    uint8_t data_buffer[9];     // TODO: temporary until we use the struct
    printf("VEXT: %d, ", machine_state.sensor_data.vext);
    printf("DIP: %d \r\n", machine_state.sensor_data.dip_switch);

    // If master polls for information, send the register
    if (data_ready) {
        // (Temporary stuff for testing I2C)
        cli();
        memcpy(data_buffer, (const uint8_t *)incoming_buffer, 8);
        sei();
        data_buffer[8] = '\0';
        printf("Received: %s\n", data_buffer);
        SEVEN_SEGMENT_showNumber(data_buffer[0] - 48);
        data_ready = false;
    }

    LED_BUILTIN_toggle();
    _delay_ms(100);
}

void MACHINE_STATE_update(void) {
    // Update sensor values

    // Check external voltage
    machine_state.sensor_data.vext = ADC0_readSingle(PORT_E, 0);
    machine_state.sensor_data.vint = ADC0_readSingle(PORT_E, 1);

    // Check Temperature

    // Check I2C (save something about I2C status?)

    // Update I2C address from DIP-switch
    machine_state.sensor_data.dip_switch = DIP4_read();
    machine_state.i2c_address = machine_state.sensor_data.dip_switch + I2C_ADDRESS_OFFSET;
    I2C_setAddress(machine_state.i2c_address);

    // Check Fan RPM

    development_testing();
}

