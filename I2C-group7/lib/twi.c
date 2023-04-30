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

void TWI0_enableBusPullups(void) {
    PORTA.PINCONFIG = PORT_PULLUPEN_bm;
    PORTA.PINCTRLUPD = PIN2_bm | PIN3_bm;
}

void TWI0_disableBusPullups(void) {
    PORTA.PINCONFIG &= ~PORT_PULLUPEN_bm;
    PORTA.PINCTRLUPD = PIN2_bm | PIN3_bm;
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
    /* Interrupt service routine for TWI target
     *
     * While in TWI target (client) operation, an interrupt on
     * this vector can happen in five scenarios:
     *
     *  A: Address package, controller will send data
     *  B: Address package, controller requests data
     *  C: Stop condition received
     *  D: Controller sent data, a byte has been received in SDATA
     *  E: Target will send data, a byte should be written to SDATA
     *
     * We enumerate the state and call the user defined callback functions.
     * See flow chart 29-6 in the avr128db48 data sheet.
     */

    cli();  // We disable interrupts temporarily

    // Address/stop interrupt. Scenario A, B, or C
    if (TWI0.SSTATUS & TWI_APIF_bm) {
        if (TWI0.SSTATUS & TWI_AP_ADR_gc) {
            // Scenario A or B.
            
            // We send 'ACK' and command the TWI controller to proceed
            TWI0.SCTRLB = TWI_ACKACT_ACK_gc | TWI_SCMD_RESPONSE_gc;
        }

        else {
            // Scenario C.

            // We send 'NACK' and end transaction with the COMPTRANS command
            TWI0.SCTRLB = TWI_ACKACT_NACK_gc | TWI_SCMD_COMPTRANS_gc;

            // We call the user defined callback function
            TWI0_TARGET_onStop();
        }
    }

    // Data interrupt. Scenario D or E
    else if (TWI0.SSTATUS & TWI_DIF_bm) {
        bool direction = (TWI0.SSTATUS & TWI_DIR_bm) >> TWI_DIR_bp;

        if (direction == 0) {
            // Scenario D: if direction bit is 0, controller sends us data

            // We give the received byte to the user defined callback function
            TWI0_TARGET_onReceive(TWI0.SDATA);
        }

        else if (direction == 1) {
            // Scenario E: if direction bit is 1, we send data to controller

            // We get the byte from the user defined callback function
            TWI0.SDATA = TWI0_TARGET_onTransmit();
        }

        // In both scenarios, we send 'ACK' and command the TWI controller to proceed
        TWI0.SCTRLB = TWI_ACKACT_ACK_gc | TWI_SCMD_RESPONSE_gc;
    }

    sei();  // Re-enable interrupts
}


void TWI0_CONTROLLER_init(void) {
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

