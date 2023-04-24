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


ISR(TCB0_INT_vect) {
    /* ISR for FAN 1 frequency count event */

    // We store the current value into the fan frequency buffer
    fan1_freq[fan1_buffer_index] = TCB0.CCMP;

    // We increment the buffer index
    if (fan1_buffer_index < FAN_MEAS_BUFFER_SIZE) {
        fan1_buffer_index ++;
    }
    else {
        fan1_buffer_index = 0;
    }
}


ISR(TCB1_INT_vect) {
    /* ISR for FAN 2 frequency count event */

    // We store the current value into the fan frequency buffer
    fan2_freq[fan2_buffer_index] = TCB1.CCMP;

    // We increment the buffer index
    if (fan2_buffer_index < FAN_MEAS_BUFFER_SIZE) {
        fan2_buffer_index ++;
    }
    else {
        fan2_buffer_index = 0;
    }
}


void FAN_MONITOR_init(void) {
    // For the two fans monitors, we use pins PE3 and PE7 which are
    // hard-wired to ZCIN1 and ZCIN2.
    PORTD.PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc; // FAN 1
    PORTE.PIN3CTRL = PORT_ISC_INPUT_DISABLE_gc; // FAN 2

    // Enable zero cross detectors 1 and 2
    ZCD0.CTRLA |= ZCD_ENABLE_bm | ZCD_OUTEN_bm;
    ZCD1.CTRLA |= ZCD_ENABLE_bm | ZCD_OUTEN_bm;

    // Set up the TCB0 timer for FAN 1
    TCB0.CTRLB = TCB_CNTMODE_FRQ_gc;    // Input capture freqency
    TCB0.EVCTRL = TCB_CAPTEI_bm;        // Enable event input
    TCB0.INTCTRL = TCB_CAPT_bm;         // Enable capture or timeout

    // Set up the TCB0 timer for FAN 2
    TCB1.CTRLB = TCB_CNTMODE_FRQ_gc;    // Input capture freqency
    TCB1.EVCTRL = TCB_CAPTEI_bm;        // Enable event input
    TCB1.INTCTRL = TCB_CAPT_bm;         // Enable capture or timeout

    // Enable the two timers
    TCB0.CTRLA = TCB_ENABLE_bm | TCB_RUNSTDBY_bm;
    TCB1.CTRLA = TCB_ENABLE_bm | TCB_RUNSTDBY_bm;

    // Set up event system to link the zero cross detectors to timers
    EVSYS.CHANNEL0 = EVSYS_CHANNEL0_ZCD0_gc; // Link ZCD0 to event channel 0
    EVSYS.CHANNEL1 = EVSYS_CHANNEL0_ZCD1_gc; // Link ZCD1 to event channel 1
    EVSYS.USERTCB0CAPT = EVSYS_CHANNEL00_bm; // Link TCB0 to event channel 0
    EVSYS.USERTCB1CAPT = EVSYS_CHANNEL01_bm; // Link TCB1 to event channel 1
}

uint16_t FAN_MONITOR_1_readSpan(void) {
    return span(fan1_freq, FAN_MEAS_BUFFER_SIZE);
}


uint16_t FAN_MONITOR_2_readSpan(void) {
    return span(fan2_freq, FAN_MEAS_BUFFER_SIZE);
}


uint16_t FAN_MONITOR_1_readFrequency(void) {
    return mean(fan1_freq, FAN_MEAS_BUFFER_SIZE);
}


uint16_t FAN_MONITOR_2_readFrequency(void) {
    return mean(fan2_freq, FAN_MEAS_BUFFER_SIZE);
}

