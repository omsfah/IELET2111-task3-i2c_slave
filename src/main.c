#define F_CPU 4000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "lib/adc.h"
#include "lib/builtins.h"
#include "lib/twi.h"
#include "lib/usart.h"


int main(void) {
    USART3_init();
    BUTTON_BUILTIN_init();
    LED_BUILTIN_init();

    stdout = &USART3_stream;


    for (;;) {
        printf("Hello, world!\n");
        LED_BUILTIN_toggle();
        _delay_ms(100);
    }
}

