/*
 *  esp32_i2c_master_usart_bridge
 *
 *  ESP32 is set up like an I2C controller, polling targets
 *  for data and sending received data over USART/USB
 *  to a computer.
 *
 */

#include <Arduino.h>
#include <Wire.h>

//#include "../../../I2C-group7/hardware/datastructure.h"


static const uint16_t POLLING_DELAY_ms = 1000;
static const uint8_t PACKET_SIZE = 52;
static const uint8_t TARGET_I2C_ADDRESS = 104;

uint8_t data_buffer[PACKET_SIZE] = {0};


void printBuffer(uint8_t bytes) {
    /* Print the received data over USART */
    for (auto i = 0; i < bytes; i++) {
        Serial.printf("%02X", data_buffer[i]);
        if ((i+1) % 4 == 0) Serial.println();
    }
    Serial.println();
}


void sendCommand(uint8_t command_num, uint32_t value) {
    /* Send command and value to target */

    Serial.printf("Command: %d value: %d\n", command_num, value);
    Wire.beginTransmission(TARGET_I2C_ADDRESS);

    // Serialize the u32 value to four unsigned bytes 
    uint8_t _v4 = (uint8_t) (value);
    uint8_t _v3 = (uint8_t) (value >> 8);
    uint8_t _v2 = (uint8_t) (value >> 16);
    uint8_t _v1 = (uint8_t) (value >> 24);

    // 5 byte command packet, as used by 'I2C_parseCommand'
    Wire.write(command_num);    // Command (see 'I2C_COMMAND' in 'datastructure.h)
    Wire.write(_v1);    // MSB
    Wire.write(_v2);
    Wire.write(_v3);
    Wire.write(_v4);    // LSB

    // End transmission and record potential error code
    uint8_t i2c_send_result = Wire.endTransmission(true);
    Serial.printf("endTransmission: %u\n", i2c_send_result);
    Serial.println();
}

void requestBytes(uint8_t bytes_requested) {
    /* Request a number of bytes from the target */

    Serial.printf("requesting %u bytes\n", bytes_requested);
    uint8_t bytes_received = Wire.requestFrom(TARGET_I2C_ADDRESS, bytes_requested);
    Serial.printf("received %u bytes\n", bytes_received);
    Wire.readBytes(data_buffer, bytes_received);    // Move data into buffer
    uint8_t i2c_send_result = Wire.endTransmission(true);
    Serial.printf("endTransmission: %u\n", i2c_send_result);

    printBuffer(bytes_received);
}

void demoCommandSystem(void) {
    sendCommand(100, 3);        // SET_THRESHOLD_VEXT_HIGH
    sendCommand(101, 0xff);     // SET_THRESHOLD_VEXT_LOW
    sendCommand(102, 0xff);
    sendCommand(103, 0xff);
    sendCommand(104, 0xff);
    sendCommand(105, 0xff);
    sendCommand(106, 0xff);
}

void setup(void) {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Wire.begin();   // Default SDA 21, SCL 22
    Serial.println("hello there");
}

void loop(void) {
    // In the loop, ESP32 polls the slave for data every POLLING_DELAY_ms
    delay(POLLING_DELAY_ms);

    demoCommandSystem();

    sendCommand(10, 0);   // Send a command to target
    requestBytes(52);           // Request data from target

    Serial.println();
}

