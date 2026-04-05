/**
 * @file timing.h
 * @brief Timing and timer functions
 * @author AVIT Research Ltd
 */

#ifndef TIMING_H
#define TIMING_H

#include <stdint.h>

/**
 * @brief 1ms timer callback
 * Called once per millisecond to decrement all active timers
 * Should be called from main loop timing code
 */
void timer1ms(void);

#endif // TIMING_H
