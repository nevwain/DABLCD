/**
 * @file globals.h
 * @brief Global variable declarations for DABLCD application
 * @author AVIT Research Ltd
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include "config.h"
#include "types.h"
#include <Adafruit_RGBLCDShield.h>

// ============================================================
// Hardware Objects
// ============================================================
extern DAB Dab;
extern Adafruit_RGBLCDShield lcd;

#ifdef ARDUINO_SAM_DUE
extern DueFlashStorage dueFlashStorage;
#endif

// ============================================================
// Ensemble and Service Management
// ============================================================
extern stEnsembles NVEnsemble;
extern DABEnsembles *Ensemble;
extern uint8_t NumOfEnsembles;

// ============================================================
// Audio Control
// ============================================================
extern uint8_t vol;

// ============================================================
// Service Selection
// ============================================================
extern uint8_t service;
extern uint8_t ensemble;
extern uint8_t freq;

// ============================================================
// Display and Text
// ============================================================
extern char service_text[DAB_MAX_SERVICEDATA_LEN];
extern uint8_t text_index;
extern uint8_t lastMinutes;
extern uint8_t text_mode;
extern uint16_t displayTimer;
extern uint8_t display_state;

// ============================================================
// Button States
// ============================================================
extern uint8_t buttons;
extern uint8_t lastbuttons;
extern uint16_t readbuttonsTimer;
extern uint16_t debouncebuttonsTimer;
extern uint8_t button_select_timer;
extern uint8_t seek_timer;
extern uint8_t seek_dir;
extern uint8_t menu_mode;
extern uint16_t menu_timer;

// ============================================================
// Timing
// ============================================================
extern uint32_t previous;

// ============================================================
// FM Mode
// ============================================================
extern uint16_t fm_freq;
extern char fm_ps[9];
extern uint8_t dab_mode;

// ============================================================
// Serial/RX Buffer
// ============================================================
extern byte rxindex;
extern char rxdata[32];

// ============================================================
// EEPROM Storage Control
// ============================================================
#ifdef USE_EEPROM
extern uint16_t writeCurrentSettingsToFlashDelayTimer;
extern uint8_t writeCurrrentSettingsToFlash;
extern int16_t magic_check;
#endif

#endif // GLOBALS_H
