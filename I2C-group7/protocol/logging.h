/*
 *  logging.h
 *
 *  Wrapper macro for serial logging functionality
 *
 *  board: AVR128DB48 Curiosity Nano
 *
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
