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
#include "../../../I2C-group7/hardware/datastructure.h"

// We can instantiate a 'machine_state' to mirror the one on the I2C target
machine_state_t machine_state;

static const uint16_t POLLING_DELAY_ms = 1;
static const uint8_t TARGET_I2C_ADDRESS = 104;
static const uint8_t PACKET_SIZE = sizeof(machine_state);

uint8_t data_buffer[PACKET_SIZE] = {0};


void printBuffer(uint8_t bytes) {
    /* Print the received data over USART */
    for (auto i = 0; i < bytes; i++) {
        Serial.printf("%02X", data_buffer[i]);
        if ((i+1) % 4 == 0) Serial.println();
    }
    Serial.println();
}

void printMachineState(void) {
    /* Print the contents of 'machine_state', using native datatypes */

    Serial.printf("[I2C_DATA]:\n");
    Serial.printf("address: \t%u\n", machine_state.i2c_data.address);
    Serial.printf("last_contact: \t%u\t[s]\n", machine_state.i2c_data.last_contact);
    Serial.println();

    Serial.printf("[ALARM_THRESHOLD]\n");
    Serial.printf("VEXT_HIGH: \t%u\t[mV]\n", machine_state.threshold.VEXT_HIGH);
    Serial.printf("VEXT_LOW: \t%u\t[mV]\n", machine_state.threshold.VEXT_LOW);
    Serial.printf("VINT_HIGH: \t%u\t[mV]\n", machine_state.threshold.VINT_HIGH);
    Serial.printf("VINT_LOW: \t%u\t[mV]\n", machine_state.threshold.VINT_LOW);
    Serial.printf("TEMP_HIGH: \t%u\t[C/100]\n", machine_state.threshold.TEMP_HIGH);
    Serial.printf("FAN_OFFTIME: \t%u\t[s]\n", machine_state.threshold.FAN_OFFTIME);
    Serial.printf("I2C_LASTCOMTIME: %u\t[s]\n", machine_state.threshold.I2C_LASTCOMTIME);
    Serial.println();

    Serial.printf("[SENSOR_DATA]\n");
    Serial.printf("dip_switch: \t%u\n", machine_state.sensor_data.dip_switch);
    Serial.printf("button_builtin: %u\n", machine_state.sensor_data.button_builtin);
    Serial.printf("vext: \t\t%u\t[mV]\n", machine_state.sensor_data.vext);
    Serial.printf("vint: \t\t%u\t[mV]\n", machine_state.sensor_data.vint);
    Serial.printf("temp: \t\t%u\t[C/100]\n", machine_state.sensor_data.temp);
    Serial.printf("fan1_freq: \t%u\t[Hz]\n", machine_state.sensor_data.fan1_freq);
    Serial.printf("fan1_span: \t%u\t[Hz]\n", machine_state.sensor_data.fan1_span);
    Serial.printf("fan1_offtime: \t%u\t[s]\n", machine_state.sensor_data.fan1_offtime);
    Serial.printf("fan2_freq: \t%u\t[Hz]\n", machine_state.sensor_data.fan2_freq);
    Serial.printf("fan2_span: \t%u\t[Hz]\n", machine_state.sensor_data.fan2_span);
    Serial.printf("fan2_offtime: \t%u\t[s]\n", machine_state.sensor_data.fan2_offtime);
    Serial.printf("uptime: \t%u\t[s]\n", machine_state.sensor_data.uptime);
    Serial.println();

    Serial.printf("[MISC.]\n");
    Serial.printf("error_code_has_changed: %u\n", machine_state.error_code_has_changed);
    Serial.printf("reset_flag_register: \t0x%02X\n", machine_state.reset_flag_register);
    Serial.printf("buzzer_state: \t%u\n", machine_state.buzzer_state);
    Serial.printf("error_code: \t%u\n", machine_state.error_code);
    Serial.printf("machine_state_size: %u\n", machine_state.machine_state_size);
    Serial.println();
}

void sendCommand(uint8_t command_num, uint32_t value) {
    /* Send command and value to target */

    Serial.printf("Command: %d value: %d\n", command_num, value);
    Wire.beginTransmission(TARGET_I2C_ADDRESS);

    // Serialize the u32 value to an array of four unsigned bytes 
    uint8_t temp[5];
    temp[0] = command_num;
    temp[1] = (uint8_t) (value >> 24);  // MSB
    temp[2] = (uint8_t) (value >> 16);
    temp[3] = (uint8_t) (value >> 8);
    temp[4] = (uint8_t) (value);        // LSB

    // 5 byte command packet, as used by 'I2C_parseCommand'
    Wire.write(temp, 5);

    // End transmission and record potential error code
    uint8_t i2c_send_result = Wire.endTransmission(true);
    Serial.printf("endTransmission: %u\n", i2c_send_result);
    Serial.println();
    //delay(500);
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
    Serial.println();
    delay(10);
}

void requestMachineState(void) {
    /* Request 'machine_state', assuming it is already 
     * in the I2C targets transmission buffer */

    Serial.printf("requesting 'machine_state' (%u bytes)\n", PACKET_SIZE);
    uint8_t bytes_received = Wire.requestFrom(TARGET_I2C_ADDRESS, PACKET_SIZE);
    Serial.printf("received %u bytes\n", bytes_received);
    Wire.readBytes(data_buffer, bytes_received);    // Move data into buffer
    memcpy(&machine_state, data_buffer, PACKET_SIZE);
    uint8_t i2c_send_result = Wire.endTransmission(true);
    Serial.printf("endTransmission: %u\n", i2c_send_result);

    Serial.println();
    delay(10);
}

namespace test {
    void readWholeMachineState(void) {
        Serial.println("[Running test]: ");

        sendCommand(100, 0); // Command 'machine_state' to be sent
        requestMachineState();
        printMachineState();
        printBuffer(PACKET_SIZE);
        memset(&data_buffer, 0, PACKET_SIZE);
    }

    void readTemperature(void) {
        Serial.println("[Running test]: readTemperature");

        for (auto i = 0; i<3; i++) {
            sendCommand(122, 0);
            requestBytes(2);
        }
    }
    
    void readFreq(void) {
        Serial.println("[Running test]: readFreq");

        sendCommand(130, 0);
        requestMachineState();
        Serial.printf("fan1_freq: \t%u\n", machine_state.sensor_data.fan1_freq);

    }

    void transmissionBufferMultiplexing(void) {
        /* Test the transmission buffer multiplexing and sending
         * by requesting different parts of the 'machine_state' */
        Serial.println("[Running test]: transmissionBufferMultiplexing");

        for (auto i = 0; i<3; i++) {
            sendCommand(122, 0); // Command 'machine_state.sensor_data.temp' to be sent
            requestBytes(52);   // Request 52 bytes and print
        }
        for (auto i = 0; i<3; i++) {
            sendCommand(101, 0); // Command 'machine_state.sensor_data' to be sent
            requestBytes(52);   // Request 52 bytes and print
        }
        for (auto i = 0; i<3; i++) {
            sendCommand(100, 0); // Command 'machine_state' to be sent
            requestBytes(52);   // Request 52 bytes and print
        }
    }

    void setAllThresholds(uint32_t val) {
        /* Command a change of all thresholds */
        Serial.println("[Running test]: setAllThresholds");

        for (auto i = 0; i < 1; i++) {
            sendCommand(0, 0);
            sendCommand(200, val);
            sendCommand(201, val);
            sendCommand(202, val);
            sendCommand(203, val);
            sendCommand(204, val);
            sendCommand(205, val);
            sendCommand(206, val);
        }
    }

    void setSaneDefaultThresholds(void) {
        /* Command a change of all thresholds to sane default values */
        Serial.println("[Running test]: setSaneDefaultThresholds");

        for (auto i = 0; i < 1; i++) {
            sendCommand(0, 0);
            sendCommand(200, 12800);    // VEXT 12.8 [V] max
            sendCommand(201, 10500);    // VEXT 10.5 [V] min
            sendCommand(202, 5500);     // VINT 5.5 [V] max
            sendCommand(203, 4300);     // VINT 4.3 [V] min
            sendCommand(204, 9000);     // TEMP 90 [C] max
            sendCommand(205, 1);        // fan off for 1 [s]
            sendCommand(206, 10);       // last i2c communication, 10 [s] ago
        }
    }

    void cmdUsartDebugPrintOnce(void) {
        sendCommand(3, 0);
    }

    void cmdSumAlarms(void) {
        sendCommand(5, 0);
    }

    void cmdSaveThresholds(void) {
        delay(2000);
        sendCommand(0, 0);
        sendCommand(5, 0);
        test::setSaneDefaultThresholds();
        test::setSaneDefaultThresholds();
        test::setSaneDefaultThresholds();
        test::setSaneDefaultThresholds();

        test::readWholeMachineState();
        test::cmdUsartDebugPrintOnce();

        delay(2000);
        sendCommand(0, 0);
        sendCommand(7, 0);

        delay(2000);
        sendCommand(1, 0);
    }
}


void setup(void) {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Wire.begin();   // Default SDA 21, SCL 22
    Serial.println("hello there");
    Serial.printf("'machine_state' size: %d\n\n", PACKET_SIZE);

}

void loop(void) {

    //test::readTemperature();
    //test::transmissionBufferMultiplexing();
    //test::cmdSumAlarms();

    delay(1000);
    /*
    test::readFreq();
    test::cmdUsartDebugPrintOnce();
    test::setAllThresholds(0xffffffff);
    test::readWholeMachineState();
    test::cmdUsartDebugPrintOnce();
    delay(1000);
    */

/*
    test::setSaneDefaultThresholds();
    test::setSaneDefaultThresholds();

    test::readWholeMachineState();
    test::cmdUsartDebugPrintOnce();

    test::setSaneDefaultThresholds();
    test::setSaneDefaultThresholds();
    */

    test::readWholeMachineState();
    test::cmdUsartDebugPrintOnce();

}

