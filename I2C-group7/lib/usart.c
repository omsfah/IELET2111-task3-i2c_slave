#include "usart.h"


void USART3_init(void) {
    /* Initialize serial communication on USART3 */

    PORTB.DIRSET = PIN0_bm;  // TX-pin as output
    PORTB.DIRCLR = PIN1_bm;  // RX-pin as input
    USART3.BAUD = USART3_BAUD_RATE(9600);
    USART3.CTRLB |= USART_RXEN_bm | USART_TXEN_bm; // Enable RX and TX
}


void USART3_sendChar(char c) {
    /* Send a single character over USART */

    while (!(USART3.STATUS & USART_DREIF_bm)) {
        // Wait for data register to empty
    }
    // Finally send the character by writing it to the send register
    USART3.TXDATAL = c;
}


void USART3_sendString(char* str) {
    /* Send null terminated string over USART */

    for (size_t i = 0; i < strlen(str); i++) {
        USART3_sendChar(str[i]);
    }
}


uint8_t USART3_printChar(char c, FILE* stream) {
    /* USART3 interface function for "printf" stream override */

    USART3_sendChar(c);
    return 0;
}


uint8_t USART3_readChar() {
    /* Read character from the USART3 RX port */

    while (!(USART3.STATUS & USART_RXCIF_bm)) {
        ;
    }
    return USART3.RXDATAL;
}

FILE USART3_stream = FDEV_SETUP_STREAM(USART3_printChar, NULL, _FDEV_SETUP_WRITE);
