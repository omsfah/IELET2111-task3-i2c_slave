#include "fan-monitor.h"



// We store values in buffers for two reasons
//  1. We can filter high frequency noise by taking the mean
//     of the array.
//  2. We can calculate the span of the array values, enabling
//     us to enumerate whether the fans have stopped.
volatile uint16_t fan1_freq[FAN_MEAS_BUFFER_SIZE];
volatile uint16_t fan2_freq[FAN_MEAS_BUFFER_SIZE];
volatile uint8_t fan1_buffer_index = 0;
volatile uint8_t fan2_buffer_index = 0;


ISR(AC0_AC_vect) {
    /* ISR for AC0.
     * Interrupt is handled by TCB0, but we need to set the flag
     * high or the program hangs*/
    AC0.STATUS = AC_CMPIF_bm;
}

ISR(AC1_AC_vect) {
    /* ISR for AC1.
     * Interrupt is handled by TCB1, but we need to set the flag
     * high or the program hangs*/
    AC1.STATUS = AC_CMPIF_bm;
}

ISR(TCB0_INT_vect) {
    /* ISR for FAN 1 frequency count event */

    cli();  // Disable interrupts

    // We store the current value into the fan frequency buffer
    fan1_freq[fan1_buffer_index] = TCB0.CCMP;

    // We increment the buffer index
    if (fan1_buffer_index < FAN_MEAS_BUFFER_SIZE) {
        fan1_buffer_index ++;
    }
    else {
        fan1_buffer_index = 0;
    }
    sei();  // Re-enable interrupts
}

ISR(TCB1_INT_vect) {
    /* ISR for FAN 2 frequency count event */

    cli();  // Disable interrupts

    // We store the current value into the fan frequency buffer
    fan2_freq[fan2_buffer_index] = TCB1.CCMP;

    // We increment the buffer index
    if (fan2_buffer_index < FAN_MEAS_BUFFER_SIZE) {
        fan2_buffer_index ++;
    }
    else {
        fan2_buffer_index = 0;
    }
    sei();  // Re-enable interrupts
}

void FAN_MONITOR_init(void) {
    /* Initialize monitoring for the two fans.
     *   FAN1:  PE0-->AC0-->EVSYS0-->TCB0
     *   FAN2:  PD3-->AC1-->EVSYS2-->TCB1
     */

    // Pin init
    PORTE.PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTD.PIN3CTRL = PORT_ISC_INPUT_DISABLE_gc;

    // Initialize analog comparator AC0
    AC0.DACREF = 0;
    AC0.INTCTRL = AC_INTMODE_NORMAL_POSEDGE_gc | AC_CMP_bm;
    AC0.MUXCTRL = AC_MUXPOS_AINP1_gc | AC_MUXNEG_DACREF_gc;
    AC0.CTRLA = AC_ENABLE_bm | AC_HYSMODE_LARGE_gc;

    // Initialize analog comparator AC1
    AC1.DACREF = 0;
    AC1.INTCTRL = AC_INTMODE_NORMAL_POSEDGE_gc | AC_CMP_bm;
    AC1.MUXCTRL = AC_MUXPOS_AINP1_gc | AC_MUXNEG_DACREF_gc;
    AC1.CTRLA = AC_ENABLE_bm | AC_HYSMODE_LARGE_gc;

    // Set up the TCB0 timer for FAN 1
    TCB0.CTRLB = TCB_CNTMODE_FRQ_gc;    // Input capture freqency
    TCB0.EVCTRL = TCB_CAPTEI_bm;        // Enable event input
    TCB0.INTCTRL = TCB_CAPT_bm;         // Enable capture or timeout

    // Set up the TCB1 timer for FAN 2
    TCB1.CTRLB = TCB_CNTMODE_FRQ_gc;    // Input capture freqency
    TCB1.EVCTRL = TCB_CAPTEI_bm;        // Enable event input
    TCB1.INTCTRL = TCB_CAPT_bm;         // Enable capture or timeout

    // Enable the two timers
    TCB0.CTRLA = TCB_ENABLE_bm | TCB_RUNSTDBY_bm;
    TCB1.CTRLA = TCB_ENABLE_bm | TCB_RUNSTDBY_bm;

    EVSYS.CHANNEL0 = EVSYS_CHANNEL0_AC0_OUT_gc; // Link AC0 to event channel 0
    EVSYS.CHANNEL2 = EVSYS_CHANNEL2_AC1_OUT_gc; // Link AC1 to event channel 2
    EVSYS.USERTCB0CAPT = EVSYS_CHANNEL00_bm; // Link TCB0 to event channel 0
    EVSYS.USERTCB1CAPT = EVSYS_CHANNEL02_bm; // Link TCB1 to event channel 2
}

uint16_t FAN_MONITOR_1_readSpan(void) {
    return span(fan1_freq, FAN_MEAS_BUFFER_SIZE);
}


uint16_t FAN_MONITOR_2_readSpan(void) {
    return span(fan2_freq, FAN_MEAS_BUFFER_SIZE);
}


uint16_t FAN_MONITOR_1_readFrequency(void) {
    uint16_t temp = mean(fan1_freq, FAN_MEAS_BUFFER_SIZE);
    return temp;
}


uint16_t FAN_MONITOR_2_readFrequency(void) {
    uint16_t temp = mean(fan2_freq, FAN_MEAS_BUFFER_SIZE);
    return temp;
}

