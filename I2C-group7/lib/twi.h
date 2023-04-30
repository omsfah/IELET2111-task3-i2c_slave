/*
 * twi.h
 *
 * AVR abstractions for Two Wire Interface (I2C)
 *
 * board: AVR128DB48 Curiosity Nano
 * project: IELET2111, oppgave 3: 'I2C-slave for monitorering'.
 * authors: Gruppe 7, BIELEKTRO NTNU, Trondheim. Våren 2023
 */


#ifndef TWI_H_
#define TWI_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>


// Macros for parsing the TWI0.MSTATUS register flags
#define TWI0_IS_CLOCKHELD()     TWI0.MSTATUS & TWI_CLKHOLD_bm
#define TWI0_IS_BUSERR()        TWI0.MSTATUS & TWI_BUSERR_bm
#define TWI0_IS_ARBLOST()       TWI0.MSTATUS & TWI_ARBLOST_bm
#define TWI0_IS_BUSBUSY()       ((TWI0.MSTATUS & TWI_BUSSTATE_BUSY_gc) == TWI_BUSSTATE_BUSY_gc)


#define TWI_DIRECTION_OUT 0
#define TWI_DIRECTION_IN 1
#define TWI_WRITE false
#define TWI_READ true

/**************** typedef twi_address_t
 * @brief 7 bit TWI address. First bit (MSB) is discarded (-xxx xxxx)
 */
typedef uint8_t twi_address_t;


/**************** TODO: docstring
 */
typedef void (*twi_receive_callback_t)(uint8_t);
typedef uint8_t (*twi_transmit_callback_t)(void);
typedef void (*twi_stop_callback_t)(void);

void TWI_TARGET_registerReceiveCallback(twi_receive_callback_t function);
void TWI_TARGET_registerTransmitCallback(twi_transmit_callback_t function);
void TWI_TARGET_registerStopCallback(twi_stop_callback_t function);


/**************** TODO: docstring
 */
void TWI0_disableBusPullups(void);

/**************** TODO: docstring
 */
void TWI0_disableBusPullups(void);

/**************** TODO: docstring
 */
void TWI0_TARGET_updateAddress(twi_address_t target_address);


/**************** TODO: docstring
 */
void TWI0_TARGET_updateAddress(twi_address_t target_address);

/**************** function TWI0_CONTROLLER_init
 * @brief Initialize TWI controller
 */
static void TWI0_CONTROLLER_init(void);


/**************** function TWI0_CONTROLLER_sendByte
 * @brief Send one byte over TWI
 * @param uint8_t twi_address   7 bit twi address
 * @param uint8_t byte          data
 */
void TWI0_CONTROLLER_sendByte(twi_address_t twi_address, uint8_t byte);

#endif
