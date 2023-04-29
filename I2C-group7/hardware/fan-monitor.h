/*
 * hardware/fan-monitor.h
 *
 * Functionality for monitoring the turn frequency of
 * two 5-bladed fans. Implementation is specific to the 
 * fan/voltage monitoring system.
 *
 * pins and systems:
 *   FAN1:  PE0-->AC0-->EVSYS0-->TCB0
 *   FAN2:  PD3-->AC1-->EVSYS2-->TCB1
 *
 * board: AVR128DB48 Curiosity Nano
 *
 */

#ifndef FAN_MONITOR_H_
#define FAN_MONITOR_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#include "../util/stat.h"

#define FAN_MEAS_BUFFER_SIZE 16

void FAN_MONITOR_init(void);
uint16_t FAN_MONITOR_1_readSpan(void);
uint16_t FAN_MONITOR_2_readSpan(void);
uint16_t FAN_MONITOR_1_readFrequency(void);
uint16_t FAN_MONITOR_2_readFrequency(void);

#endif
