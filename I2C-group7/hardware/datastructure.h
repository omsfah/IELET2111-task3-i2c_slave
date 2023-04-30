/*
 * datastructure.h
 *
 * Data structure definitions that defines communication
 * internally on the AVR128DB48 and externally over I2C.
 *
 * This file is also read by the ESP32 controller sketch, and
 * unfortunately this required us to use a lot of compiler directives
 * to enforce byte layout compatibility between C and C++. Both struct
 * padding and C's implicit enum size played into this, and may also
 * cause compatibility issues in the future with other compilers and
 * changes to the data containers.
 *
 * board: AVR128DB48 Curiosity Nano, ESP32
 *
 */

#ifndef DATASTRUCTURE_H_
#define DATASTRUCTURE_H_

#ifdef __cplusplus
#pragma pack(1)
extern "C" {
#endif

typedef enum {
    /* I2C commands reference.
     *
     *  series 0-99: Direct commands
     *  series 100-119: Bulk transmission
     *  series 120-199: Single value transmission
     *  series 200-255: Threshold settings
     */
    NO_COMMAND = 0,
    SOFTWARE_RESET = 1,
    unused2 = 2,
    USART_DEBUG_PRINT_ONCE = 3,
    TEST_ALARM = 4,
    SUM_ALARM = 5,
    SAVE_THRESHOLDS_TO_EEPROM = 7,
    LOAD_THRESHOLDS_FROM_EEPROM = 8,
    FACTORY_RESET = 9,
    ENABLE_I2C_PULLUPS = 10,
    DISABLE_I2C_PULLUPS = 11,
    SENDCONTAINER_MACHINE_STATE = 100,
    SENDCONTAINER_SENSOR_DATA = 101,
    SENDCONTAINER_THRESHOLDS = 102,
    SEND_VEXT = 120,
    SEND_VINT = 121,
    SEND_TEMP = 122,
    SEND_FAN1_FREQ = 130,
    SEND_FAN1_SPAN = 131,
    SEND_FAN1_OFFTIME = 132,
    SEND_FAN2_FREQ = 140,
    SEND_FAN2_SPAN = 141,
    SEND_FAN2_OFFTIME = 142,
    SEND_UPTIME = 150,
    SEND_RESET_FLAG_REGISTER = 151,
    SEND_ERROR_CODE = 152,
    SET_THRESHOLD_VEXT_HIGH = 200,
    SET_THRESHOLD_VEXT_LOW = 201,
    SET_THRESHOLD_VINT_HIGH = 202,
    SET_THRESHOLD_VINT_LOW = 203,
    SET_THRESHOLD_TEMP_HIGH = 204,
    SET_THRESHOLD_FAN_OFFTIME = 205,
    SET_THRESHOLD_I2C_LASTCOMTIME = 206
} I2C_COMMAND;


typedef struct __attribute__((__packed__)) {
    /* Thresholds/maximum values for the systems
     * before alarm will go off */

    // External voltage adc level, [millivolts]
    uint16_t VEXT_HIGH;
    uint16_t VEXT_LOW;

    // Internal voltage adc level, [millivolts]
    uint16_t VINT_HIGH;
    uint16_t VINT_LOW;

    uint16_t TEMP_HIGH;         // Temperature [celsius / 100]
    uint16_t FAN_OFFTIME;       // Time since fan stopped, [seconds]
    uint16_t I2C_LASTCOMTIME;   // Time since last communication with master, [seconds]
} alarm_threshold_t;


typedef struct __attribute__((__packed__)) {
    /* Readings of sensors are represented here.
     *
     * Note: A change in datatype here must also be reflected in
     * 'I2C_parseCommand' in 'I2C.c'
     * */
    
    uint16_t vext;
    uint16_t vint;
    uint16_t temp;
    uint16_t fan1_freq;
    uint16_t fan1_span;
    uint16_t fan1_offtime;
    uint16_t fan2_freq;
    uint16_t fan2_span;
    uint16_t fan2_offtime;
    uint32_t uptime;
    uint8_t dip_switch;
    uint8_t button_builtin;
} measurements_t;


typedef struct __attribute__((__packed__)) {
    /* Data structure containing I2C data and status.
     * Since the I2C routine is writing to this through
     * interrupts, it must be instantiated as volatile. */

    uint32_t last_contact;
    uint8_t address;
} i2c_data_t;


typedef enum 
#ifdef __cplusplus
: uint8_t
#endif
__attribute__((__packed__)) {
    /* System error codes */
    SYSTEM_OK = 0,  // (cannot be just 'OK' due to C redeclaration of enum problem)
    ALARM_TEMPERATURE = 1,
    ALARM_SINGLE_FAN_FAILURE = 2,
    ALARM_BOTH_FAN_FAILURE = 3,
    ALARM_VEXT_HIGH = 4,
    ALARM_VEXT_LOW = 5,
    ALARM_VINT_HIGH = 6,
    ALARM_VINT_LOW = 7,
    ALARM_I2C_NOCONTACT = 8,
    unused_9

    // Error code candidates:
    //  - ALARM_UNEXPECTED_RESTART
    //  - ALARM_WATCHDOG_TIMEOUT
} error_code_t;


typedef enum 
#ifdef __cplusplus
: uint8_t
#endif
__attribute__((__packed__)) {
    /* State machine for the alarm buzzer */
    BUZZER_OFF = 0,
    BUZZER_CONSTANT_ON = 1,
    BUZZER_SUMMED = 2
} buzzer_state_t;


typedef struct __attribute__((__packed__)) {  
    /* Container data structure for the whole machine state */

    volatile measurements_t sensor_data;
    volatile alarm_threshold_t threshold;
    volatile i2c_data_t i2c_data;
    uint8_t error_code_has_changed;
    uint8_t reset_flag_register;
    buzzer_state_t buzzer_state;
    error_code_t error_code;
    uint8_t machine_state_size;
} machine_state_t;



#ifdef __cplusplus
}
#endif

#endif /* DATASTRUCTURE_H_ */
