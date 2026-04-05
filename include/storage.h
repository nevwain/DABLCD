/**
 * @file storage.h
 * @brief EEPROM and Flash storage management
 * @author AVIT Research Ltd
 */

#ifndef STORAGE_H
#define STORAGE_H

#include "config.h"

#ifdef USE_EEPROM

// Magic number for EEPROM validation
#define MAGIC 8500

// EEPROM storage locations
#define FLASH_ensemble        0    // byte
#define FLASH_service         1    // byte
#define FLASH_vol             2    // byte
#define FLASH_dab_mode        3    // byte
#define FLASH_NumOfEnsembles  4    // byte  
#define FLASH_magic_check     8    // word (2 bytes)
#define FLASH_fm_freq         12   // word (2 bytes)
#define FLASH_Ensembles       16   // and lots more

#define WAIT_SAVECURRENTSETTINGS  10000  // wait 10 sec. after change before save

/**
 * @brief Write settings to EEPROM/Flash storage
 * Writes current ensemble, service, volume, and mode information
 */
void write_settings_to_storage(void);

/**
 * @brief Read settings from EEPROM/Flash storage
 * Restores previously saved ensemble, service, volume, and mode
 */
void read_settings_from_storage(void);

/**
 * @brief Save ensemble list to storage
 * Stores the scanned ensemble list for later retrieval
 */
void save_ensembles_to_storage(void);

/**
 * @brief Load ensemble list from storage
 * Restores the previously scanned ensemble list
 */
void load_ensembles_from_storage(void);

#endif // USE_EEPROM

#endif // STORAGE_H
