// File structure:
// lib:

#define F_CPU 4000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "configuration.h"

#include "lib/adc.h"
#include "lib/builtins.h"
#include "lib/usart.h"

#include "hardware/init.h"
#include "hardware/read.h"


#include "protocol/I2C.h"



int main(void) {
    USART3_init();
    BUTTON_BUILTIN_init();
    LED_BUILTIN_init();

    stdout = &USART3_stream;    // Move to usart.h if possible

    // Instantiate transmission register

    // Initialize I2C
    I2C_init(I2C_TARGET_DEFAULT_ADDRESS);

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

        // If master polls for information, send the register


        printf("Hello, world!\n");
        LED_BUILTIN_toggle();
        _delay_ms(100);
    }
}

