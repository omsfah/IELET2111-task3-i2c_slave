// File structure:
// lib for functions and declarations
// hardware for initializing hardware components

#include <avr/io.h>
#include <util/delay.h>

#include "configuration.h"
#include "hardware/init.h"

#include "protocol/I2C.h"
#include "protocol/logging.h"


int main(void) {
    SYSTEMS_init();
    MACHINE_STATE_init();
    I2C_init(I2C_TARGET_DEFAULT_ADDRESS);


    for (;;) {
        /* We loop forever, updating the machine_state
         * data structure with sensor data for every
         * loop iteration. On the I2C controllers
         * request, we transmit the data. If any level
         * exceeds a given threshold, the alarm is sound
         * and alarm code is shown on the seven segment
         * display.
         */
        MACHINE_STATE_update();
    }
}

