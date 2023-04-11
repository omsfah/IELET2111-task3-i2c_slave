#include "twi.h"


static void TWI0_wait(void) {
    /* Wait forever while the TWI bus is not ready or there is an error */
    while (!(TWI0_IS_CLOCKHELD() || TWI0_IS_BUSERR() || TWI0_IS_ARBLOST() || TWI0_IS_BUSBUSY())) {
        ;
    }
}


static void TWI0_busInit(void) {
    /* Initialize TWI bus */

    // Enable TWI pins
    PORTA.DIRSET = PIN2_bm;     // SDA
    PORTA.DIRSET = PIN3_bm;     // SCL
    
    // Simultaneously enable internal pull-up for PA2 and PA3
    PORTA.PINCONFIG = PORT_PULLUPEN_bm;
    PORTA.PINCTRLUPD = PIN2_bm | PIN3_bm;
    
    // Configure SDA
    TWI0.CTRLA = TWI_SDASETUP_4CYC_gc | TWI_SDAHOLD_300NS_gc;
    
    TWI0.CTRLA &= ~TWI_FMPEN_bm;    // Disable fast mode
    TWI0.MBAUD = (uint8_t) 15;      // Ideally we should calculate baud rate from F_CPU and other
                                    // timing parameters, but 15 works.
}


static void TWI0_TARGET_init(twi_address_t target_address) {
    /* Initialize device as TWI target */

    TWI0_busInit();

    // Register target address
    TWI0.SADDR = target_address << 1;

    // Instantiate callback functions with NULL pointers
    twi_write_callback_t TWI0_TARGET_onWrite = NULL;
    twi_read_callback_t TWI0_TARGET_onRead = NULL;
    twi_stop_callback_t TWI0_TARGET_onStop = NULL;

    // Set up target control register
    TWI0.SCTRLA |= TWI_ENABLE_bm;           // Mark device as TWI target
    TWI0.SCTRLA |= TWI_DIEN_bm;             // Enable data interrupts
    TWI0.SCTRLA |= TWI_APIEN_bm;            // Enable Address/stop interrupts
    TWI0.SCTRLA |= TWI_PIEN_bm;             // Enable ?????

    //Configure status register
    TWI0.SSTATUS = TWI_BUSSTATE_IDLE_gc;    // Force TWI bus to IDLE state
}


void TWI_TARGET_registerWriteCallback(twi_write_callback_t function) {
    /* Register write callback function for TWI TARGET */
    TWI0_TARGET_onWrite = function;
}


void TWI_TARGET_registerReadCallback(twi_read_callback_t function) {
    /* Register read callback function for TWI TARGET */
    TWI0_TARGET_onRead = function;
}


void TWI_TARGET_registerStopCallback(twi_stop_callback_t function) {
    /* Register stop callback function for TWI TARGET */
    TWI0_TARGET_onStop = function;
}


ISR(TWI0_TWIS_vect) {
    /* General ISR for TWI target */

    // TODO: is it possible to break this up into more atomic ISRs?

    // Check if data interrupt flag is high
    if (TWI0.SSTATUS & TWI_DIF_bm) {

        // Prepare a data buffer
        uint8_t data = 0x00;

        // Scenario A: Write data (Controller -> Target)
        if (((TWI0.SSTATUS & TWI_DIR_bm) >> TWI_DIR_bp) == TWI_WRITE) {
            data = TWI0.SDATA;

            // If write callback function exists, run it on the data
            if (TWI0_TARGET_onWrite) {
                TWI0_TARGET_onWrite(data);
            }
        }

        // Scenario B: Read data (Controller <- Target)
        else { // TODO: add proper if clause instead of else
            // If write callback function exists, run it on the data
            if (TWI0_TARGET_onRead) {
                data = TWI0_TARGET_onRead();
            }

            TWI0.SDATA = data;
        }

        TWI0.SCTRLB |= TWI_ACKACT_ACK_gc;
        TWI0.SCTRLB |= TWI_SCMD_RESPONSE_gc;
    }

    // Check if address detection flag is high
    if (TWI0.SSTATUS & TWI_AP_ADR_gc) {

        // Address match, send ACK
        if (TWI0.SSTATUS & TWI_AP_ADR_gc) {
            TWI0.SCTRLB |= TWI_ACKACT_ACK_gc;
            TWI0.SCTRLB |= TWI_SCMD_RESPONSE_gc;
        }

        else { //TODO: add proper if clause instead of else
            if (TWI0_TARGET_onStop) {
                TWI0_TARGET_onStop();
            }

            TWI0.SCTRLB |= TWI_ACKACT_NACK_gc;
            TWI0.SCTRLB |= TWI_SCMD_COMPTRANS_gc;
        }
    }
}


static void TWI0_CONTROLLER_init(void) {
    /* Initialize device as TWI controller */

    TWI0_busInit();

    //Configure status register
    TWI0.MCTRLA = TWI_ENABLE_bm;            // Mark device as TWI controller
    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;    // Force TWI bus to IDLE state
}


static void TWI0_CONTROLLER_sendByte(twi_address_t twi_address, uint8_t byte) {
    /* Send one byte to over TWI */
    
    // Construct the address packet. Last bit is the direction bit.
    TWI0.MADDR = (twi_address << 1) | TWI_DIRECTION_OUT;

    // Ensure that 'Write Interrupt Flag' is low, indicating that TWI bus is ready
    while (!(TWI0.MSTATUS & TWI_WIF_bm)) {
        ;
    }
    
    // Pack data to send
    TWI0.MDATA = byte;
    
    // Stop TWI connection
    TWI0.MCTRLB = TWI_MCMD_STOP_gc;         // Set stop condition bit
    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;    // Force bus state to IDLE
}

