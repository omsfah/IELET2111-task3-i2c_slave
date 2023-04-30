#include "seven-segment.h"


/* These inline functions set the different segments of
 * the display high. If the pins are to be changed, they
 * can be done here.
 */
static inline void TOP(void) {PORTD.OUTSET |= PIN5_bm;}
static inline void UL(void)  {PORTD.OUTSET |= PIN6_bm;}
static inline void UR(void)  {PORTD.OUTSET |= PIN4_bm;}
static inline void MID(void) {PORTD.OUTSET |= PIN7_bm;}
static inline void LL(void)  {PORTC.OUTSET |= PIN7_bm;}
static inline void LR(void)  {PORTC.OUTSET |= PIN5_bm;}
static inline void BOT(void) {PORTC.OUTSET |= PIN6_bm;}
static inline void DOT(void) {PORTC.OUTSET |= PIN4_bm;}


void SEVEN_SEGMENT_init(void) {
    /* Initialize generic seven segment display.
     * 
     * Connection from the PORTD pins on Curiosity Nano AVR128db48 to
     * the display is as follows:
     *
     * 7 6 GND 5 4  PORTD
     * -----------
     * | display |
     * | display |
     * | display |
     * | display |
     * | display |
     * -----------
     * 7 6 GND 5 4  PORTC
     */
    PORTD.DIRSET = 0xf0;    // Pins PD4-PD7
    PORTC.DIRSET = 0xf0;    // Pins PC4-PC7
}

void SEVEN_SEGMENT_showNumber(int8_t number) {
    /* Show a number (0-9) on the seven segment display */

    /* LED segment diagram
     *   -PD5-
     *  |     |
     * PD6   PD4
     *  |     |
     *   -PD7-
     *  |     |
     * PC7   PC5
     *  |     |
     *   -PC6-  PC4
     *
     */
    PORTD.OUTCLR = 0xf0;
    PORTC.OUTCLR = 0xf0;

    switch (number) {
        case 0:
            TOP(); UL(); UR(); LL(); LR(); BOT();
        break;
        case 1:
            UR(); LR();
        break;
        case 2:
            TOP(); UR(); MID(); LL(); BOT();
        break;
        case 3:
            TOP(); UR(); MID(); LR(); BOT();
        break;
        case 4:
            UL(); MID(); UR(); LR();
        break;
        case 5:
            TOP(); UL(); MID(); LR(); BOT();
        break;
        case 6:
            TOP(); UL(); MID(); LL(); LR(); BOT();
        break;
        case 7:
            TOP(); UR(); LR();
        break;
        case 8:
            TOP(); UL(); UR(); MID(); LL(); LR(); BOT();
        break;
        case 9:
            TOP(); UL(); MID(); UR(); LR();
        break;
        case -1:
            // For an input of -1, we clear the screen
            PORTD.OUTSET = 0xf0;
            PORTC.OUTSET = 0xf0;
        break;
        default:
            // For an undefined input, we light the decimal dot to indicate
            // the number is out of range
            DOT();
    }
}
