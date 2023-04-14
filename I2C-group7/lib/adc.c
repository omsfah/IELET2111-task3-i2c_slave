#include "adc.h"

uint16_t adcVal = 0;


void ADC0_init(uint8_t pin, ADC_MODE adc_mode) {
    /* Enable pin on port D for analog to digital conversion */

    // Disable digital input buffer
    switch(pin) {
        case 0:
            PORTD.PIN0CTRL &= ~PORT_ISC_gm;
            PORTD.PIN0CTRL |= PORT_ISC_INPUT_DISABLE_gc;
            break;
        case 1:
            PORTD.PIN1CTRL &= ~PORT_ISC_gm;
            PORTD.PIN1CTRL |= PORT_ISC_INPUT_DISABLE_gc;
            break;
        case 2:
            PORTD.PIN2CTRL &= ~PORT_ISC_gm;
            PORTD.PIN2CTRL |= PORT_ISC_INPUT_DISABLE_gc;
            break;
        case 3:
            PORTD.PIN3CTRL &= ~PORT_ISC_gm;
            PORTD.PIN3CTRL |= PORT_ISC_INPUT_DISABLE_gc;
            break;
        case 4:
            PORTD.PIN4CTRL &= ~PORT_ISC_gm;
            PORTD.PIN4CTRL |= PORT_ISC_INPUT_DISABLE_gc;
            break;
        case 5:
            PORTD.PIN5CTRL &= ~PORT_ISC_gm;
            PORTD.PIN5CTRL |= PORT_ISC_INPUT_DISABLE_gc;
            break;
        case 6:
            PORTD.PIN6CTRL &= ~PORT_ISC_gm;
            PORTD.PIN6CTRL |= PORT_ISC_INPUT_DISABLE_gc;
            break;
        case 7:
            PORTD.PIN7CTRL &= ~PORT_ISC_gm;
            PORTD.PIN7CTRL |= PORT_ISC_INPUT_DISABLE_gc;
            break;
    }

    // Disable pull-up resistor
    switch(pin) {
        case 0:
            PORTD.PIN0CTRL &= ~PORT_PULLUPEN_bm;
            break;
        case 1:
            PORTD.PIN1CTRL &= ~PORT_PULLUPEN_bm;
            break;
        case 2:
            PORTD.PIN2CTRL &= ~PORT_PULLUPEN_bm;
            break;
        case 3:
            PORTD.PIN3CTRL &= ~PORT_PULLUPEN_bm;
            break;
        case 4:
            PORTD.PIN4CTRL &= ~PORT_PULLUPEN_bm;
            break;
        case 5:
            PORTD.PIN5CTRL &= ~PORT_PULLUPEN_bm;
            break;
        case 6:
            PORTD.PIN6CTRL &= ~PORT_PULLUPEN_bm;
            break;
        case 7:
            PORTD.PIN7CTRL &= ~PORT_PULLUPEN_bm;
            break;
    }

    // Scale adc clock to CLC_PER divided by 4
    ADC0.CTRLC = ADC_PRESC_DIV4_gc;

    // Enable ADC in 10 bit mode
    ADC0.CTRLA = ADC_ENABLE_bm | ADC_RESSEL_10BIT_gc;

    // Reference to internal voltage rail
    VREF.ADC0REF = VREF_REFSEL_VDD_gc;

    // Set ADC mode
    switch(adc_mode) {
        case SINGLE_CONVERSION_MODE:
            // This is the default mode
            break;
        case FREERUN_MODE:
            ADC0.CTRLA |= ADC_FREERUN_bm;
            break;
    }
}


uint16_t ADC0_readSingle(uint8_t pin) {
    /* SINGLE_CONVERSION_MODE: read ADC value from 'pin' */

    // Select ADC channel
    switch(pin) {
        case 0:
            ADC0.MUXPOS = ADC_MUXPOS_AIN0_gc;
            break;
        case 1:
            ADC0.MUXPOS = ADC_MUXPOS_AIN1_gc;
            break;
        case 2:
            ADC0.MUXPOS = ADC_MUXPOS_AIN2_gc;
            break;
        case 3:
            ADC0.MUXPOS = ADC_MUXPOS_AIN3_gc;
            break;
        case 4:
            ADC0.MUXPOS = ADC_MUXPOS_AIN4_gc;
            break;
        case 5:
            ADC0.MUXPOS = ADC_MUXPOS_AIN5_gc;
            break;
        case 6:
            ADC0.MUXPOS = ADC_MUXPOS_AIN6_gc;
            break;
        case 7:
            ADC0.MUXPOS = ADC_MUXPOS_AIN7_gc;
            break;
    }

    // Start ADC conversion
    ADC0.COMMAND = ADC_STCONV_bm;

    // Wait until ADC conversion is done
    while (!(ADC0.INTFLAGS & ADC_RESRDY_bm)) {
        ;
    }

    // Clear the input flag by writing "1" to it
    ADC0.INTFLAGS = ADC_RESRDY_bm;

    // Finally return result
    return ADC0.RES;
}

uint16_t ADC0_read(uint8_t pin) {
    /* Read ADC from 'pin' on port D.
     * ADC must be initialized in FREERUN mode and
     * ACD conversion must have been started with the 
     * ADC0_start function */

    // Select ADC channel
    switch(pin) {
        case 0:
            ADC0.MUXPOS = ADC_MUXPOS_AIN0_gc;
            break;
        case 1:
            ADC0.MUXPOS = ADC_MUXPOS_AIN1_gc;
            break;
        case 2:
            ADC0.MUXPOS = ADC_MUXPOS_AIN2_gc;
            break;
        case 3:
            ADC0.MUXPOS = ADC_MUXPOS_AIN3_gc;
            break;
        case 4:
            ADC0.MUXPOS = ADC_MUXPOS_AIN4_gc;
            break;
        case 5:
            ADC0.MUXPOS = ADC_MUXPOS_AIN5_gc;
            break;
        case 6:
            ADC0.MUXPOS = ADC_MUXPOS_AIN6_gc;
            break;
        case 7:
            ADC0.MUXPOS = ADC_MUXPOS_AIN7_gc;
            break;
    }

    // Clear the input flag by writing "1" to it
    ADC0.INTFLAGS = ADC_RESRDY_bm;

    // Finally return result
    return ADC0.RES;
}

void ADC0_start(void){
    /* Start ADC conversion*/
    ADC0.COMMAND = ADC_STCONV_bm;
}

bool ADC0_conversionDone(void) {
    /* Returns true if ADC conversion is done */
    return (ADC0.INTFLAGS & ADC_RESRDY_bm);
}