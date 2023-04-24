/*
 * stat.h
 *
 * A general statistics library written in C
 *
 */

#ifndef STAT_H_
#define STAT_H_

#include <math.h>
#include <stdint.h>

uint16_t mean(uint16_t data[], uint16_t len);
uint16_t standardDeviation(uint16_t data[], uint16_t len);

#endif

