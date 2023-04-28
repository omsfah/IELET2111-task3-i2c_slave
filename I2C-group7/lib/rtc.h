/*
 *  rtc.h
 *
 *  AVR abstractions real time counter (RTC)
 *
 *  board: AVR128DB48 Curiosity Nano
 *
 */

#ifndef RTC_H_
#define RTC_H_


#include <avr/io.h>
#include <avr/interrupt.h>

void RTC_UPTIME_COUNTER_init(void);
uint32_t RTC_UPTIME_COUNTER_read(void);

#endif
