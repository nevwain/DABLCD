/**
 * @file types.h
 * @brief Data structure definitions for DABLCD application
 * @author AVIT Research Ltd
 */

#ifndef TYPES_H
#define TYPES_H

#include <DABShield.h>

// ============================================================
// Text Display Modes
// ============================================================
enum TextMode {
  TEXTMODE_SERVICEDATA = 0,
  TEXTMODE_TIME = 1,
  TEXTMODE_ENSEMBLE = 2
};

// ============================================================
// Display States
// ============================================================
enum DisplayState {
  DISPLAY_INIT = 0,
  DISPLAY_DELAY = 1,
  DISPLAY_RUN = 2,
  DISPLAY_SERVICE = 3,
  DISPLAY_VOL = 4,
  DISPLAY_MENU = 5
};

// ============================================================
// DAB Ensemble and Service Storage
// ============================================================

/**
 * @struct DABEnsembles
 * @brief Stores a DAB ensemble with all its services
 */
typedef struct _DABEnsembles
{
  uint8_t freq_index;
  char Ensemble[17];
  uint8_t numberofservices;
  DABService service[24];  // MAX_SERVICES_PER_ENSEMBLE
} DABEnsembles;

/**
 * @struct stEnsembles
 * @brief Container for all stored ensembles in non-volatile memory
 */
typedef struct
{
  DABEnsembles Ensemble[16];  // MAX_ENSEMBLES (default)
} stEnsembles;

#endif // TYPES_H
