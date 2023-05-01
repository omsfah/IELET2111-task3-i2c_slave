/*
 * configuration.h
 *
 * Global configuration file for the
 * I2C target fan/voltage monitoring system
 *
 * board: AVR128DB48 Curiosity Nano
 * project: IELET2111, oppgave 3: 'I2C-slave for monitorering'.
 * authors: Gruppe 7, BIELEKTRO NTNU, Trondheim. Våren 2023
 */


#define F_CPU 4000000UL

// We reserve the 100-107 series for I2C targets
#define I2C_TARGET_DEFAULT_ADDRESS 104
#define I2C_ADDRESS_OFFSET 100

// EEPROM memory layout
#define THRESHOLDS_EEPROM_ADDR 0x0

// Debug printing config
#define ENABLE_DEBUG_PRINT_TO_USART3

// Factory settings for the alarm thresholds
#define FACTORY_VEXT_HIGH       12800
#define FACTORY_VEXT_LOW        10500
#define FACTORY_VINT_HIGH       5500
#define FACTORY_VINT_LOW        4300
#define FACTORY_TEMP_HIGH       9000
#define FACTORY_FAN_OFFTIME     1
#define FACTORY_I2C_LASTCOMTIME 10

// Settings for the I2C buffer internals
#define TRANSMISSION_BUFFER_SIZE 48
#define RECEIVE_BUFFER_SIZE 5

