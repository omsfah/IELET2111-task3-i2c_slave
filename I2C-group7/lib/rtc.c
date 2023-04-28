#include "rtc.h"


volatile uint32_t pti_period_counter = 0;


ISR(RTC_PIT_vect) {
    /* Interrupt routine for Periodic Interrupt Timer (PIT) on
     * the RTC peripheral. */

    // With a 32 cycle prescaler and a 32.768 [kHz] oscillator, this
    // routine is called with a frequency of 1024 [Hz]
    pti_period_counter++;

    RTC.PITINTFLAGS = RTC_PI_bm;
}


static void XOSC32K_configure(void) {
    /* Configure the 32.768 [kHz] oscillator */

    // To change the settings safely, we disable the clock
    // source temporarily while changing the select bit.
    uint8_t temp = CLKCTRL.XOSC32KCTRLA;

    // Disable clock
    temp &= ~CLKCTRL_ENABLE_bm;
    ccp_write_io((void*)&CLKCTRL.XOSC32KCTRLA, temp);
    while (CLKCTRL.MCLKSTATUS & CLKCTRL_XOSC32KS_bm) {}

    // Change clock selection 
    temp = CLKCTRL.XOSC32KCTRLA;
    temp &= ~CLKCTRL_SEL_bm;
    ccp_write_io((void*)&CLKCTRL.XOSC32KCTRLA, temp);

    // Enable clock
    temp = CLKCTRL.XOSC32KCTRLA;
    temp |= CLKCTRL_SEL_bm;
    ccp_write_io((void*)&CLKCTRL.XOSC32KCTRLA, temp);
}


void RTC_UPTIME_COUNTER_init(void) {
    /* Initialize a periodic interrupt on the RTC */

    // Configure RTC clocksource
    XOSC32K_configure();

    // Set the PIT period prescaler to 32 and enable interrupt
    RTC.PITCTRLA = RTC_PERIOD_CYC32768_gc | RTC_PITEN_bm;

    // Enable interrupts for for the period timer
    RTC.PITINTCTRL = RTC_PI_bm;
    sei();
}

uint32_t RTC_UPTIME_COUNTER_read(void) {
    /* Returns time [seconds] the system has been running */

    // This implementation is conveniently simple and uses floating point
    // arithmetics, it might not be suitable if performance is important.
    return (pti_period_counter * (1024.0 / 1000.0));
}
