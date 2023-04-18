#include "read.h"

uint8_t DIP4_read(void) {
    /* Reads an unsigned binary encoded integer from the
     * DIP switch. Least significant digit is to the right. */

    uint8_t temp = 15;
    if (PORTC.IN & PIN3_bm) {
        temp -= 1;  // LSB
    }
    if (PORTC.IN & PIN2_bm) {
        temp -= 2;
    }
    if (PORTC.IN & PIN1_bm) {
        temp -= 4;
    }
    if (PORTC.IN & PIN0_bm) {
        temp -= 8;  // MSB
    }
    return temp;
}
