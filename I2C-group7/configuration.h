/*
 * configuration.h
 *
 * Global configuration file for the
 * I2C target fan/voltage monitoring system
 *
 * board: AVR128DB48 Curiosity Nano
 *
 */


#define F_CPU 4000000UL

#define I2C_TARGET_DEFAULT_ADDRESS 0x04

// Debug printing config
//#define ENABLE_DEBUG_PRINT_TO_USART3
