#include "twi.h"


// Instantiate callback function pointers
twi_receive_callback_t TWI0_TARGET_onReceive = 0;
twi_transmit_callback_t TWI0_TARGET_onTransmit = 0;
twi_stop_callback_t TWI0_TARGET_onStop = 0;


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
    
    // Enable internal pull-up for PA2 and PA3
    PORTA.PINCONFIG = PORT_PULLUPEN_bm;
    PORTA.PINCTRLUPD = PIN2_bm | PIN3_bm;
    
    TWI0.CTRLA |= TWI_SDASETUP_4CYC_gc;
    TWI0.CTRLA |= TWI_SDAHOLD_500NS_gc;
    TWI0.CTRLA &= ~TWI_FMPEN_bm;    // Disable fast mode
}

void TWI0_TARGET_updateAddress(twi_address_t target_address) {
    /* Update target address */
    TWI0.SADDR = target_address << 1;
}


void TWI0_TARGET_init(twi_address_t target_address) {
    /* Initialize device as TWI target */

    TWI0_busInit();

    // Register target address
    TWI0.SADDR = target_address << 1;

    // Instantiate callback functions with NULL pointers
    twi_receive_callback_t TWI0_TARGET_onReceive = 0;
    twi_transmit_callback_t TWI0_TARGET_onTransmit = 0;
    twi_stop_callback_t TWI0_TARGET_onStop = 0;

    // Set up target control register
    TWI0.SCTRLA |= TWI_DIEN_bm;             // Enable data interrupts
    TWI0.SCTRLA |= TWI_APIEN_bm;            // Enable address interrupts
    TWI0.SCTRLA |= TWI_PIEN_bm;             // Enable stop interrupts
    TWI0.SCTRLA |= TWI_ENABLE_bm;           // Mark device as TWI target

    // Configure status register
    TWI0.SSTATUS = TWI_BUSSTATE_IDLE_gc;    // Force TWI bus to IDLE state

    // Enable interrupts
    sei();
}


void TWI_TARGET_registerReceiveCallback(twi_receive_callback_t function) {
    /* Register receive callback function for TWI TARGET */
    TWI0_TARGET_onReceive = function;
}


void TWI_TARGET_registerTransmitCallback(twi_transmit_callback_t function) {
    /* Register transmit callback function for TWI TARGET */
    TWI0_TARGET_onTransmit = function;
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

        // Scenario A: Receive data (Controller -> Target)
        if (((TWI0.SSTATUS & TWI_DIR_bm) >> TWI_DIR_bp) == TWI_WRITE) {
            data = TWI0.SDATA;

            // If receive callback function exists, run it on the data
            if (TWI0_TARGET_onReceive) {
                TWI0_TARGET_onReceive(data);
            }
        }

        // Scenario B: Transmit data (Controller <- Target)
        else { // TODO: add proper if clause instead of else
            // If write callback function exists, run it on the data
            if (TWI0_TARGET_onTransmit) {
                data = TWI0_TARGET_onTransmit();
            }

            TWI0.SDATA = data;
        }
        
        // Either way, we send ACK
        TWI0.SCTRLB = TWI_ACKACT_ACK_gc | TWI_SCMD_RESPONSE_gc;
    }

    // Check if address detection flag is high
    if (TWI0.SSTATUS & TWI_APIF_bm) {

        // Address match, send ACK
        if (TWI0.SSTATUS & TWI_AP_ADR_gc) {
            // It turns out that writing to the SCTRLB register in two
            // operations does not yield the same compiled result. 
            // This was the cause of a weird timeout bug that was hard to
            // track down.
            //
            // The culprit is left commented out for future reference.

            /*
            TWI0.SCTRLB |= TWI_ACKACT_ACK_gc;
            TWI0.SCTRLB |= TWI_SCMD_RESPONSE_gc;
            */

            TWI0.SCTRLB = TWI_ACKACT_ACK_gc | TWI_SCMD_RESPONSE_gc;
        }

        else { //TODO: add proper if clause instead of else
            if (TWI0_TARGET_onStop) {
                TWI0_TARGET_onStop();
            }
            
            // Transaction is complete, send acknowledgement
            TWI0.SCTRLB = TWI_ACKACT_NACK_gc | TWI_SCMD_COMPTRANS_gc;
        }
    }
}


static void TWI0_CONTROLLER_init(void) {
    /* Initialize device as TWI controller */

    TWI0_busInit();

    TWI0.MBAUD = (uint8_t) 15;      // Ideally we should calculate baud rate from F_CPU and other
                                    // timing parameters, but 15 works.

    //Configure status register
    TWI0.MCTRLA = TWI_ENABLE_bm;            // Mark device as TWI controller
    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;    // Force TWI bus to IDLE state
}


void TWI0_CONTROLLER_sendByte(twi_address_t twi_address, uint8_t byte) {
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

