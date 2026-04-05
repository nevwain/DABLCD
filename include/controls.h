/**
 * @file controls.h
 * @brief Button and input control handling
 * @author AVIT Research Ltd
 */

#ifndef CONTROLS_H
#define CONTROLS_H

#include <stdint.h>

/**
 * @brief Process button inputs and execute corresponding actions
 * 
 * @param buttons Bitmask of button states from LCD shield
 *                BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_SELECT
 */
void process_buttons(uint8_t buttons);

/**
 * @brief Move to next service (next DAB service or increase FM frequency)
 */
void DAB_NextService(void);

/**
 * @brief Move to previous service (previous DAB service or decrease FM frequency)
 */
void DAB_PreviousService(void);

/**
 * @brief Increase volume
 */
void DAB_VolUp(void);

/**
 * @brief Decrease volume
 */
void DAB_VolDown(void);

#endif // CONTROLS_H
