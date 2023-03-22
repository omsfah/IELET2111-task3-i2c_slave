/*
 * twi.h
 *
 * AVR abstractions for Two Wire Interface (I2C)
 *
 * board: AVR128DB48 Curiosity Nano
 *
 */


#ifndef TWI_H_
#define TWI_H_

#include <avr/io.h>
#include <avr/interrupt.h>


// Macros for parsing the TWI0.MSTATUS register flags
#define TWI0_IS_CLOCKHELD()		TWI0.MSTATUS & TWI_CLKHOLD_bm
#define TWI0_IS_BUSERR()		TWI0.MSTATUS & TWI_BUSERR_bm
#define TWI0_IS_ARBLOST()		TWI0.MSTATUS & TWI_ARBLOST_bm
#define TWI0_IS_BUSBUSY()		((TWI0.MSTATUS & TWI_BUSSTATE_BUSY_gc) == TWI_BUSSTATE_BUSY_gc)


#define TWI_DIRECTION_OUT 0
#define TWI_DIRECTION_IN 1

/**************** typedef twi_address_t
 * 7 bit TWI address. First bit (MSB) is discarded (-xxx xxxx)
 */
typedef uint8_t twi_address_t 


/**************** function TWI0_init
 * Initialize TWI bus
 */
static void TWI0_init(void);


/**************** function TWI0_sendByte
 * Send one byte over TWI
 * @param uint8_t twi_address   7 bit twi address
 * @param uint8_t byte          data
 */
static void TWI0_sendByte(twi_address_t twi_address, uint8_t byte);


/**************** function TWI0_wait
 * While TWI bus is not ready or is reporting errors, wait forever
 */
static void TWI0_wait(void);


#endif
