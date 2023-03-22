/*
 *  esp32_i2c_master_usart_bridge
 *
 *  ESP32 is setup like an I2C master, polling slave
 *  for data and sending received data over USART/USB
 *  to a computer.
 *
 */

#include <Arduino.h>
#include <Wire.h>


static const uint16_t POLLING_DELAY_ms = 1000;
static const uint16_t PACKET_SIZE_bytes = 4;
static const uint8_t SLAVE_I2C_ADDRESS = 4;


void setup(void) {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Wire.begin();   // Default SDA 21, SCL 22

    Serial.println("hello there");
}

void loop(void) {
    // In the loop, ESP32 polls the slave for data every POLLING_DELAY_ms
    delay(POLLING_DELAY_ms);
    
    
    // Establish communication of address packet
    Wire.beginTransmission(SLAVE_I2C_ADDRESS);

    // Optionally send something to the slave
    Wire.write(42);

    // End transmission and record potential error code
    uint8_t i2c_send_result = Wire.endTransmission(true);
    Serial.printf("endTransmission: %u\n", i2c_send_result);

    // Read bytes from slave
    uint8_t bytes_received = Wire.requestFrom(SLAVE_I2C_ADDRESS, PACKET_SIZE_bytes);
    Serial.printf("requestFrom: %u\n", bytes_received);

    // If we receive more than 0 bytes, send over USART
    if (bytes_received > 0) {
        uint8_t _receive_buffer[bytes_received];            // Allocate buffer space 
        Wire.readBytes(_receive_buffer, bytes_received);    // Move data into buffer

        // Print the received data over USART
        for (auto i = 0; i < bytes_received; i++) {
            Serial.println(_receive_buffer[i]);
        }
    }
}
