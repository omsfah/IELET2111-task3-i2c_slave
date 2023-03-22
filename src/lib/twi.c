#include "twi.h"


static void TWI0_init(void) {
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
    
    //Configure status register
    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;    // Force TWI bus to IDLE state
    TWI0.MCTRLA = TWI_ENABLE_bm;    // Enable TWI host
}

static void TWI0_sendByte(twi_address_t twi_address, uint8_t byte) {
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

static void TWI0_wait(void) {
    /* Wait forever while the TWI bus is not ready or there is an error */
    while (!(TWI0_IS_CLOCKHELD() || TWI0_IS_BUSERR() || TWI0_IS_ARBLOST() || TWI0_IS_BUSBUSY())) {
        ;
    }
}
