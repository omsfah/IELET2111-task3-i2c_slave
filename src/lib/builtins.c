#include "builtins.h"


void BUTTON_BUILTIN_init(void) {
    // Initialize builtin button on PB2
    PORTB.DIRCLR = PIN2_bm;
    PORTB.PIN2CTRL |= PORT_PULLUPEN_bm;
}

void BUTTON_BUILTIN_awaitPress(void) {
    // Halt until the builting button is pressed
    while (PORTB.IN & PIN2_bm) {
    }
}

void LED_BUILTIN_init(void) {
    // Initialize builtin LED on PB3
    PORTB.DIRSET = PIN3_bm;
}

void LED_BUILTIN_toggle(void) {
    // Toggle builtin LED on PB3 
    PORTB.OUT ^= PIN3_bm;
}

void LED_BUILTIN_off(void) {
    // Turn off builtin LED on PB3 
    PORTB.OUTSET = PIN3_bm; // Builtin LED is inverted 1->off
}

void LED_BUILTIN_on(void) {
    // Turn on builtin LED on PB3 
    PORTB.OUTCLR = PIN3_bm; // Builtin LED is inverted 0->on
}

