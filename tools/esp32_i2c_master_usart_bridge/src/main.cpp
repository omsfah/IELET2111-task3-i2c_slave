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


static const uint16_t POLLING_DELAY_ms = 100;
static const uint8_t PACKET_SIZE = 2;
static const uint8_t TARGET_I2C_ADDRESS = 104;

static const uint8_t DEBUG_INCREMENTER_MIN = 48;
static const uint8_t DEBUG_INCREMENTER_MAX = 57;

uint8_t number = DEBUG_INCREMENTER_MIN;
uint8_t data_buffer[PACKET_SIZE] = {};


void print_buffer(uint8_t bytes) {
    // Print the received data over USART
    for (auto i = 0; i < bytes; i++) {
        Serial.println(data_buffer[i]);
    }
    Serial.println();
}

void request_data_from_target(uint8_t bytes) {
    Serial.printf("requesting data from target\n");
    Wire.beginTransmission(TARGET_I2C_ADDRESS);

    // End transmission and record potential error code
    uint8_t i2c_send_result = Wire.endTransmission(true);
    Serial.printf("endTransmission: %u\n", i2c_send_result);

    uint8_t bytes_received = Wire.requestFrom(TARGET_I2C_ADDRESS, bytes);
    Serial.printf("reading %u bytes from target\n", bytes_received);
    Wire.readBytes(data_buffer, bytes_received);    // Move data into buffer

    // End transmission and record potential error code
    i2c_send_result = Wire.endTransmission(true);
    Serial.printf("endTransmission: %u\n", i2c_send_result);
    
    print_buffer(bytes_received);
}

void old(void) {
    // Establish communication of address packet
    Wire.beginTransmission(TARGET_I2C_ADDRESS);

    // Optionally send something to the slave
    number++;
    if (number > DEBUG_INCREMENTER_MAX) {
        number = DEBUG_INCREMENTER_MIN;
    }
    Serial.printf("Sending data: %d\n", number);
    Wire.write(number);
    Wire.write(0);
    Wire.write(0);
    Wire.write(255);
    Wire.write(0);
    Wire.write(255);

    // End transmission and record potential error code
    uint8_t i2c_send_result = Wire.endTransmission(true);
    Serial.printf("endTransmission: %u\n", i2c_send_result);

    // Read bytes from slave
    uint8_t bytes_received = Wire.requestFrom(TARGET_I2C_ADDRESS, PACKET_SIZE);
    Serial.printf("requestFrom: %u\n", bytes_received);

    // If we receive more than 0 bytes, send over USART
    if (bytes_received > 0) {
        uint8_t _receive_buffer[bytes_received];            // Allocate buffer space 
        Wire.readBytes(_receive_buffer, bytes_received);    // Move data into buffer
        for (auto i = 0; i < bytes_received; i++) {
            Serial.println(_receive_buffer[i]);
        }
    }
    Serial.printf("\n");
}

void command(uint8_t command_num, uint32_t value) {
    Wire.beginTransmission(TARGET_I2C_ADDRESS);

    Serial.printf("Command: %d value: %d\n", command_num, value);

    uint8_t _v4 = (uint8_t) (value);
    uint8_t _v3 = (uint8_t) (value >> 4);
    uint8_t _v2 = (uint8_t) (value >> 8);
    uint8_t _v1 = (uint8_t) (value >> 12);

    Wire.write(command_num);
    //Wire.write(value);
    Wire.write(_v1);
    Wire.write(_v2);
    Wire.write(_v3);
    Wire.write(_v4);

    // End transmission and record potential error code
    uint8_t i2c_send_result = Wire.endTransmission(true);
    Serial.printf("endTransmission: %u\n", i2c_send_result);
    // Read bytes from slave
    uint8_t bytes_received = Wire.requestFrom(TARGET_I2C_ADDRESS, PACKET_SIZE);
    Serial.printf("requestFrom: %u\n", bytes_received);
}

void send_incrementing_number(void) {
    // Establish communication of address packet
    Wire.beginTransmission(TARGET_I2C_ADDRESS);

    number++;
    if (number > DEBUG_INCREMENTER_MAX) {
        number = DEBUG_INCREMENTER_MIN;
    }
    Serial.printf("Sending data: %d\n", number);
    Wire.write(number);

    // End transmission and record potential error code
    uint8_t i2c_send_result = Wire.endTransmission(true);
    Serial.printf("endTransmission: %u\n", i2c_send_result);

}

void setup(void) {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Wire.end();
    Wire.begin();   // Default SDA 21, SCL 22

    Serial.println("hello there");
}

void loop(void) {
    // In the loop, ESP32 polls the slave for data every POLLING_DELAY_ms
    delay(POLLING_DELAY_ms);

    //request_data_from_target(32);
    //old();
    command(3, 31337);
    //send_incrementing_number();
    Serial.println();
}
