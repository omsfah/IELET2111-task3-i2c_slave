// File structure:
// lib for functions and declarations
// hardware for initializing hardware components

#define F_CPU 4000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "configuration.h"

#include "lib/adc.h"
#include "lib/builtins.h"
#include "lib/usart.h"

#include "hardware/init.h"

#include "protocol/I2C.h"
#include "protocol/logging.h"



int main(void) {
    USART3_init();
    BUTTON_BUILTIN_init();
    LED_BUILTIN_init();
	ADC0_init(PORT_D, 0, SINGLE_CONVERSION_MODE);
    DIP4_init();
    SEVEN_SEGMENT_init();
	

    stdout = &USART3_stream;    // Move to usart.h if possible

    I2C_init(I2C_TARGET_DEFAULT_ADDRESS);   // Initialize I2C
    uint8_t data_buffer[9];     // TODO: temporary until we use the struct

    // Update I2C address from DIP-switch
    twi_address_t new_address = 4;  // TODO: Read this address from DIP-switches
    I2C_setAddress(new_address);

    // Initialize brownout detection
    // Initialize watchdog
    // Initialize fan monitoring
    // Initialize voltage monitoring

    // Startup check

    for (;;) {
        // Check voltages (own and external)
        // Check I2C
        // Update I2C address from DIP-switch
        // Check Temperature
        // Check Fan RPM
        // Write data into transmission register


        // While developing, we can test subsystems here
		uint16_t val = ADC0_readSingle(PORT_E, 0);
		uint8_t dip = DIP4_read();
        printf("ADC: %d, ",val);
        printf("DIP: %d \r\n",dip);
        SEVEN_SEGMENT_showNumber(dip);

        // If master polls for information, send the register
        if (data_ready) {
            // (Temporary stuff for testing I2C)
            cli();
            memcpy(data_buffer, (const uint8_t *)incoming_buffer, 8);
            sei();
            data_buffer[8] = '\0';
            printf("Received: %s\n", data_buffer);
            data_ready = false;
        }

        LED_BUILTIN_toggle();
        _delay_ms(100);
    }
}

