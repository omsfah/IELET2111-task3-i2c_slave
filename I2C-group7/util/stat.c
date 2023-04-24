#include "stat.h"

uint16_t mean(uint16_t data[], uint16_t len) {
    uint32_t sum = 0;
    for (uint16_t i = 0; i < len; i++) {
        sum += data[i];
    }
    return sum / len;
}

uint16_t standardDeviation(uint16_t data[], uint16_t len) {
    uint16_t _mean = mean(data, len);
    uint32_t _temp;

    // Sum of square of every members distance from the mean
    for (uint16_t i = 0; i < len; i++) {
       _temp += (data[i] - _mean) * (data[i] - _mean);
    }

    return sqrt(_temp/len);
}
