#include "seven-segment.h"

void SEVEN_SEGMENT_init(void) {
    /* Initialize generic seven segment display.
     * 
     * Connection from the PORTD pins on Curiosity Nano AVR128db48 to
     * the display is as follows:
     *
     * 0 1 GND 2 3
     * -----------
     * | display |
     * | display |
     * | display |
     * | display |
     * | display |
     * -----------
     * 4 5 GND 6 7
     */
    PORTD.DIRSET = 0xff;    // All pins 0-7
}

void SEVEN_SEGMENT_showNumber(int8_t number) {
    /* Show a number (0-9) on the seven segment display */

    /* LED segment diagram for PORTD pins 0-7, including decimal dot (7)
     *  --2--
     * |     |
     * 1     3
     * |     |
     *  --0--
     * |     |
     * 4     6
     * |     |
     *  --5--  7
     *
     */

    PORTD.OUTCLR = 0xff;   // Set all pins low

    switch (number) {
        case 0:
            PORTD.OUTSET = 0b01111110;  // Combined bits of pins 1,2,3,4,5,6
        break;
        case 1:
            PORTD.OUTSET = 0b01001000;  // etc.
        break;
        case 2:
            PORTD.OUTSET = 0b00111101;
        break;
        case 3:
            PORTD.OUTSET = 0b01101101;
        break;
        case 4:
            PORTD.OUTSET = 0b01001011;
        break;
        case 5:
            PORTD.OUTSET = 0b01100111;
        break;
        case 6:
            PORTD.OUTSET = 0b01110111;
        break;
        case 7:
            PORTD.OUTSET = 0b01001100;
        break;
        case 8:
            PORTD.OUTSET = 0b01111111;
        break;
        case 9:
            PORTD.OUTSET = 0b01001111;
        break;
        case -1:
            // For an input of -1, we clear the screen
            PORTD.OUTSET = 0;
        break;
        default:
            // For an input above 9, we light the decimal dot to indicate
            // the number is out of range
            if (number > 9) {
                PORTD.OUTSET = PIN7_bm;
            }
    }
}

