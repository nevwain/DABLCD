/**
 * @file display.h
 * @brief LCD display and UI functions
 * @author AVIT Research Ltd
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

/**
 * @brief Process and update the display based on current state
 * Handles all display state transitions and rendering
 */
void process_display(void);

/**
 * @brief Initialize display with startup message
 */
void display_init(void);

#endif // DISPLAY_H
