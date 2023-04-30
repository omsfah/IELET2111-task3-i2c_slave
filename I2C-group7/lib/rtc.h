/*
 * rtc.h
 *
 * AVR abstractions for real time counter (RTC)
 *
 * board: AVR128DB48 Curiosity Nano
 * project: IELET2111, oppgave 3: 'I2C-slave for monitorering'.
 * authors: Gruppe 7, BIELEKTRO NTNU, Trondheim. Våren 2023
 */

#ifndef RTC_H_
#define RTC_H_


#include <avr/io.h>
#include <avr/interrupt.h>

void RTC_UPTIME_COUNTER_init(void);
uint32_t RTC_UPTIME_COUNTER_read(void);

#endif
