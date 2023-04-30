/*
 * logging.h
 *
 * Wrapper macro for serial logging functionality
 *
 * board: AVR128DB48 Curiosity Nano
 * project: IELET2111, oppgave 3: 'I2C-slave for monitorering'.
 * authors: Gruppe 7, BIELEKTRO NTNU, Trondheim. Våren 2023
 */

#ifndef LOGGING_H_
#define LOGGING_H_

#include "../configuration.h"

// By overriding printf this way we can easily
// disable it globally in 'configuration.h'

#ifdef ENABLE_DEBUG_PRINT_TO_USART3
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif


#endif /* LOGGING_H */
