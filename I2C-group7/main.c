/*
 * main.c
 *
 * Main entry point for I2C target fan/voltage monitor.
 *
 * Project for the subject IELET2111 Mikrokontrollersystemer.
 *  - Olaf Andreas Hafsmo
 *  - Gunnar Myhre
 *  - Daniel Thauland
 * Spring 2023. NTNU, Trondheim
 *
 * board: AVR128DB48 Curiosity Nano
 * project: IELET2111, oppgave 3: 'I2C-slave for monitorering'.
 * authors: Gruppe 7, BIELEKTRO NTNU, Trondheim. Våren 2023
 *
 */

#include "configuration.h"
#include "hardware/system.h"

#include "protocol/I2C.h"
#include "protocol/logging.h"


int main(void) {
    SYSTEMS_init();
    MACHINE_STATE_init();
    I2C_init(I2C_TARGET_DEFAULT_ADDRESS);


    for (;;) {
        /* We loop forever, updating the 'machine_state'
         * data structure with sensor data for every
         * loop iteration. On the I2C controllers
         * request, we transmit the data.
         */
        MACHINE_STATE_update();

        /* If any level exceeds a given threshold, the
         * alarm is sound and alarm code is shown on the
         * seven segment display.
         */
        ALARM_SYSTEM_update();

        /* If the I2C controller has commanded a change of
         * the system settings, we do so.
         */
        I2C_SYSTEM_update();
    }
}

