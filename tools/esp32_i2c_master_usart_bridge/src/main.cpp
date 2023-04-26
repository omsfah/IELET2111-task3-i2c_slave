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
static const uint8_t PACKET_SIZE = 53;
static const uint8_t TARGET_I2C_ADDRESS = 104;

static const uint8_t DEBUG_INCREMENTER_MIN = 48;
static const uint8_t DEBUG_INCREMENTER_MAX = 57;

uint8_t number = DEBUG_INCREMENTER_MIN;
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

    // Read bytes from slave
    uint8_t bytes_received = Wire.requestFrom(TARGET_I2C_ADDRESS, PACKET_SIZE);
    Serial.printf("requestFrom: %u\n", bytes_received);
    Wire.readBytes(data_buffer, bytes_received);    // Move data into buffer

    printBuffer(bytes_received);
}

void setup(void) {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    //Wire.end();
    Wire.begin();   // Default SDA 21, SCL 22

    Serial.println("hello there");
}

void loop(void) {
    // In the loop, ESP32 polls the slave for data every POLLING_DELAY_ms
    delay(POLLING_DELAY_ms);

    sendCommand(0x64, 0x1ff);
    Serial.println();
}
