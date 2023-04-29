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

// We reserve the 100-107 series for I2C targets
#define I2C_TARGET_DEFAULT_ADDRESS 104
#define I2C_ADDRESS_OFFSET 100

// EEPROM memory layout
#define THRESHOLDS_EEPROM_ADDR 0x0

// Debug printing config
//#define ENABLE_DEBUG_PRINT_TO_USART3

