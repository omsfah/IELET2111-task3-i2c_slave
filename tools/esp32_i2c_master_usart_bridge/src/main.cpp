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


static const uint16_t POLLING_DELAY_ms = 1;
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
    delay(10);
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
    delay(10);
}

namespace test {
    void transmissionBufferSwitching(void) {
        /* Request the whole transmission buffer 10 times, then
         * the temperature 10 times. */
        Serial.println("[Running test]: transmissionBufferSwitching");

        for (auto i = 0; i<3; i++) {
            sendCommand(10, 0); // Command 'machine_state' to be sent
            requestBytes(52);   // Request 52 bytes and print
        }
        for (auto i = 0; i<3; i++) {
            sendCommand(22, 0); // Command 'SEND_TEMP'
            requestBytes(52);   // Request 52 bytes and print
        }
    }

    void setAllThresholds(void) {
        /* Command a change of all thresholds */
        Serial.println("[Running test]: setAllThresholds");

        for (auto val = 0; val < 3; val++) {
            sendCommand(100, val);
            sendCommand(101, val);
            sendCommand(102, val);
            sendCommand(103, val);
            sendCommand(104, val);
            sendCommand(105, val);
            sendCommand(106, val);
        }
    }
}


void setup(void) {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Wire.begin();   // Default SDA 21, SCL 22
    Serial.println("hello there");
}

void loop(void) {
    delay(1000);

    test::transmissionBufferSwitching();
    test::setAllThresholds();
}

