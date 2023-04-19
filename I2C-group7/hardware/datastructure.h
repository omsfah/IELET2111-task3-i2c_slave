/*
 * datastructure.h
 *
 * Data structure definitions that defines communication
 * internally on the AVR128DB48 and externally over I2C.
 * This file is also read by the ESP32 controller sketch.
 *
 * board: AVR128DB48 Curiosity Nano
 *
 */

#ifndef DATASTRUCTURE_H_
#define DATASTRUCTURE_H_


typedef struct {
    /* Thresholds/maximum values for the systems
     * before alarm will go off */

    // TODO: Decide if we should use ADC levels or actual (float) values.
    //       Could also use millivolts (which simplifies the datatype)

    // External voltage adc level, [millivolts]
    uint16_t VEXT_HIGH;
    uint16_t VEXT_LOW;

    // Internal voltage adc level, [millivolts]
    uint16_t VINT_HIGH;
    uint16_t VINT_LOW;

    uint16_t TEMP_HIGH;  // Temperature [celsius / 100]
    uint16_t FAN_OFFTIME;       // Time since fan stopped, [milliseconds]
    uint16_t I2C_LASTCOMTIME;   // Time since last communication with master, [milliseconds]
} alarm_threshold_t;


typedef struct {
    /* Readings of sensors are represented here */
    
    uint8_t dip_switch;
    uint16_t vext;
    uint16_t vint;
    uint16_t temp;
    uint16_t fan_speed;
    uint16_t fan_offtime;
    uint32_t uptime;
} measurements_t;


typedef struct {
    /* Data structure containing I2C data and status.
     * Since the I2C routine is writing to this through
     * interrupts, it must be instantiated as volatile. */

    uint8_t address;
    bool data_request_flag;
    bool new_settings_flag;
    bool data_ready;
    bool data_was_read;
} i2c_data_t;


typedef enum {
    /* System error codes */
    OK = 0,
    ALARM_TEMPERATURE = 1,
    ALARM_SINGLE_FAN_FAILURE = 2,
    ALARM_BOTH_FAN_FAILURE = 3,
    ALARM_VEXT_HIGH = 4,
    ALARM_VEXT_LOW = 5,
    ALARM_VINT_HIGH = 6,
    ALARM_VINT_LOW = 6,
    ALARM_I2C_NOCONTACT = 8,
    unused_9

    // Error code candidates:
    //  - ALARM_UNEXPECTED_RESTART
    //  - ALARM_WATCHDOG_TIMEOUT
} error_code_t;


typedef struct {
    /* Container data structure for the whole machine state */

    alarm_threshold_t threshold;
    volatile i2c_data_t i2c_data;
    //measurements_t sensor_data[10]; // TODO: use an array so we can do mean values
    measurements_t sensor_data;
    error_code_t error_code;
    bool error_code_has_changed;
} machine_state_t;


#endif /* DATASTRUCTURE_H_ */
