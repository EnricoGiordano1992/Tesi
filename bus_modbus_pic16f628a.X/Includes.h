
#ifndef __INCLUDES_H
#define __INCLUDES_H

// Define CPU Frequency
// This must be defined, if __delay_ms() or 
// __delay_us() functions are used in the code
#define _XTAL_FREQ   4000000

#define LED1 RA1
#define LED2 RA0
#define SIGNAL RA3

#include <htc.h> 
#include "driver/TIMER.h"
#include "driver/UART.h"
#include <pic16f628a.h>

#endif