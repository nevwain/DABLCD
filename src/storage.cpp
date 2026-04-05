/**
 * @file storage.cpp
 * @brief EEPROM and Flash storage management implementation
 * @author AVIT Research Ltd
 */

#include "storage.h"
#include "globals.h"
#include "config.h"
#include <string.h>

#ifdef USE_EEPROM

// ============================================================
// Platform-specific storage read/write
// ============================================================

#ifdef DUE_FLASH

/**
 * Arduino Due - Uses DueFlashStorage library
 */

void write_settings_to_storage(void)
{
  dueFlashStorage.write(FLASH_ensemble, ensemble);
  dueFlashStorage.write(FLASH_service, service);
  dueFlashStorage.write(FLASH_dab_mode, dab_mode);
  dueFlashStorage.write(FLASH_vol, vol);
  dueFlashStorage.write(FLASH_fm_freq, (uint8_t *)&fm_freq, sizeof(fm_freq));
}

void read_settings_from_storage(void)
{
  service = dueFlashStorage.read(FLASH_service);
  ensemble = dueFlashStorage.read(FLASH_ensemble);
  vol = dueFlashStorage.read(FLASH_vol);
  dab_mode = dueFlashStorage.read(FLASH_dab_mode);
  memcpy(&fm_freq, dueFlashStorage.readAddress(FLASH_fm_freq), sizeof(fm_freq));
}

void save_ensembles_to_storage(void)
{
  magic_check = MAGIC;
  dueFlashStorage.write(FLASH_NumOfEnsembles, NumOfEnsembles);
  dueFlashStorage.write(FLASH_ensemble, ensemble);
  dueFlashStorage.write(FLASH_service, service);
  dueFlashStorage.write(FLASH_dab_mode, dab_mode);
  dueFlashStorage.write(FLASH_vol, vol);
  dueFlashStorage.write(FLASH_magic_check, (uint8_t *)&magic_check, sizeof(magic_check));
  dueFlashStorage.write(FLASH_fm_freq, (uint8_t *)&fm_freq, sizeof(fm_freq));
  dueFlashStorage.write(FLASH_Ensembles, (uint8_t *)&NVEnsemble, sizeof(NVEnsemble));
}

void load_ensembles_from_storage(void)
{
  NumOfEnsembles = dueFlashStorage.read(FLASH_NumOfEnsembles);
  dab_mode = dueFlashStorage.read(FLASH_dab_mode);
  memcpy(&magic_check, dueFlashStorage.readAddress(FLASH_magic_check), sizeof(magic_check));
}

#elif ARDUINO_ARCH_SAMD

/**
 * Arduino SAMD (Zero, M0) - Uses FlashStorage_SAMD library
 */

void write_settings_to_storage(void)
{
  EEPROM.write(FLASH_ensemble, ensemble);
  EEPROM.write(FLASH_service, service);
  EEPROM.write(FLASH_dab_mode, dab_mode);
  EEPROM.write(FLASH_vol, vol);
  EEPROM.put(FLASH_fm_freq, fm_freq);
  
  if (!EEPROM.getCommitASAP())
  {
    Serial.println("CommitASAP not set. Need commit()");
    EEPROM.commit();
  }
}

void read_settings_from_storage(void)
{
  service = EEPROM.read(FLASH_service);
  ensemble = EEPROM.read(FLASH_ensemble);
  vol = EEPROM.read(FLASH_vol);
  dab_mode = EEPROM.read(FLASH_dab_mode);
  EEPROM.get(FLASH_fm_freq, fm_freq);
}

void save_ensembles_to_storage(void)
{
  magic_check = MAGIC;
  EEPROM.write(FLASH_NumOfEnsembles, NumOfEnsembles);
  EEPROM.write(FLASH_ensemble, ensemble);
  EEPROM.write(FLASH_service, service);
  EEPROM.write(FLASH_dab_mode, dab_mode);
  EEPROM.write(FLASH_vol, vol);
  EEPROM.put(FLASH_magic_check, magic_check);
  EEPROM.put(FLASH_fm_freq, fm_freq);
  EEPROM.put(FLASH_Ensembles, NVEnsemble);
  
  if (!EEPROM.getCommitASAP())
  {
    Serial.println("CommitASAP not set. Need commit()");
    EEPROM.commit();
  }
}

void load_ensembles_from_storage(void)
{
  NumOfEnsembles = EEPROM.read(FLASH_NumOfEnsembles);
  dab_mode = EEPROM.read(FLASH_dab_mode);
  EEPROM.get(FLASH_magic_check, magic_check);
}

#else

/**
 * Default implementation - Standard EEPROM (UNO R4, etc)
 */

void write_settings_to_storage(void)
{
  EEPROM.write(FLASH_ensemble, ensemble);
  EEPROM.write(FLASH_service, service);
  EEPROM.write(FLASH_dab_mode, dab_mode);
  EEPROM.write(FLASH_vol, vol);
  EEPROM.put(FLASH_fm_freq, fm_freq);
}

void read_settings_from_storage(void)
{
  service = EEPROM.read(FLASH_service);
  ensemble = EEPROM.read(FLASH_ensemble);
  vol = EEPROM.read(FLASH_vol);
  dab_mode = EEPROM.read(FLASH_dab_mode);
  EEPROM.get(FLASH_fm_freq, fm_freq);
}

void save_ensembles_to_storage(void)
{
  magic_check = MAGIC;
  EEPROM.write(FLASH_NumOfEnsembles, NumOfEnsembles);
  EEPROM.write(FLASH_ensemble, ensemble);
  EEPROM.write(FLASH_service, service);
  EEPROM.write(FLASH_dab_mode, dab_mode);
  EEPROM.write(FLASH_vol, vol);
  EEPROM.put(FLASH_magic_check, magic_check);
  EEPROM.put(FLASH_fm_freq, fm_freq);
  EEPROM.put(FLASH_Ensembles, NVEnsemble);
}

void load_ensembles_from_storage(void)
{
  NumOfEnsembles = EEPROM.read(FLASH_NumOfEnsembles);
  dab_mode = EEPROM.read(FLASH_dab_mode);
  EEPROM.get(FLASH_magic_check, magic_check);
}

#endif  // Platform-specific implementations

#endif  // USE_EEPROM
