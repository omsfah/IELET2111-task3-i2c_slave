/*
 * protocol/I2C_packet.h
 *
 * Packet definition for I2C communication,
 * specific to the I2C target fan/voltage monitoring system.
 *
 * board: AVR128DB48 Curiosity Nano
 *
 */

#ifndef PROTOCOL_I2C_PACKET_H_
#define PROTOCOL_I2C_PACKET_H_

#include <avr/io.h>


// Struct typedef for I2C packet
typedef struct {
    uint16_t externally_supplied_voltage;
    uint16_t regulated_mcu_voltage;
    uint16_t fan_speed;
    uint32_t uptime;
} i2c_status_packet_t;


#endif
