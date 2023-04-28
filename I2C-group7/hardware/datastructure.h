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
extern "C" {
#pragma pack(1)
#endif

typedef enum {
    /* I2C commands reference.
     *
     *  series 0-9: Direct commands
     *  series 10-19: Bulk transmission
     *  series 20-99: Single value transmission
     *  series 100-149: Threshold settings
     */
    NO_COMMAND = 0,
    REBOOT = 1,
    TEST_ALARM = 2,
    USART_DEBUG_PRINT_ONCE = 3,
    SENDCONTAINER_MACHINE_STATE = 10,
    SENDCONTAINER_SENSOR_DATA = 11,
    SENDCONTAINER_THRESHOLDS = 12,
    SEND_VEXT = 20,
    SEND_VINT = 21,
    SEND_TEMP = 22,
    SEND_FAN1_FREQ = 30,
    SEND_FAN1_SPAN = 31,
    SEND_FAN1_OFFTIME = 32,
    SEND_FAN2_FREQ = 40,
    SEND_FAN2_SPAN = 41,
    SEND_FAN2_OFFTIME = 42,
    SEND_UPTIME = 50,
    SEND_ALARM_STATE = 51,
    SEND_ERROR_CODE = 52,
    SET_THRESHOLD_VEXT_HIGH = 100,
    SET_THRESHOLD_VEXT_LOW = 101,
    SET_THRESHOLD_VINT_HIGH = 102,
    SET_THRESHOLD_VINT_LOW = 103,
    SET_THRESHOLD_TEMP_HIGH = 104,
    SET_THRESHOLD_FAN_OFFTIME = 105,
    SET_THRESHOLD_I2C_LASTCOMTIME = 106
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
    //uint16_t _padding;
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

    volatile i2c_data_t i2c_data;
    volatile alarm_threshold_t threshold;
    volatile measurements_t sensor_data;
    uint8_t error_code_has_changed;
    uint8_t alarm_state;
    buzzer_state_t buzzer_state;
    error_code_t error_code;
    uint8_t machine_state_size;
} machine_state_t;



#ifdef __cplusplus
}
#endif

#endif /* DATASTRUCTURE_H_ */
