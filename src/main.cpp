/**
 * @file main.cpp
 * @brief DABLCD Main Application - Arduino Setup and Loop
 * @brief DAB/LCD Shield Example Application
 * @author AVIT Research Ltd
 * @version 2.0 - Updated for IDE 2.3.3 onwards
 * 
 * Library Dependencies:
 *    * DABShield
 *    * Adafruit_RGBLCDShield
 *    * FlashStorage_SAMD (SAMD devices)
 *    * DueFlashStorage (Arduino Due)
 *
 * Version History:
 * v2.0 27/08/2025 - Updated for IDE 2.3.3, refactored into modular C++ files
 * v1.1 23/09/2020 - Added ESP32 D1 R32 Support
 * v1.0 16/07/2020 - Display mapping for EU chars
 * Earlier versions - See documentation
 */

#include "Arduino.h"
#include "config.h"
#include "globals.h"
#include "types.h"
#include "display.h"
#include "controls.h"
#include "dab_services.h"
#include "charset.h"
#include "timing.h"
#include "spi.h"
#include "storage.h"

// ============================================================
// Global Variable Instantiation
// ============================================================

DAB Dab;
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

#ifdef ARDUINO_SAM_DUE
DueFlashStorage dueFlashStorage;
#endif

stEnsembles NVEnsemble;
DABEnsembles *Ensemble;
uint8_t NumOfEnsembles;

uint8_t vol = 63;
uint8_t service = 0;
uint8_t ensemble = 0;
uint8_t freq = 0;

char service_text[DAB_MAX_SERVICEDATA_LEN];
uint8_t text_index;
uint8_t lastMinutes = 99;
uint8_t text_mode = TEXTMODE_SERVICEDATA;

uint8_t dab_mode = true;
uint8_t buttons;
uint8_t lastbuttons;
uint16_t readbuttonsTimer;
uint16_t debouncebuttonsTimer;
uint32_t previous;

uint16_t fm_freq = 8750;
char fm_ps[9] = "";

uint16_t displayTimer;
uint8_t display_state;

byte rxindex = 0;
char rxdata[32];

// Button state tracking for menu
uint8_t button_select_timer = 0;
uint8_t button_select = true;
uint8_t seek_timer = 0;
uint8_t seek_dir = 0;
uint8_t menu_mode = 0;
uint16_t menu_timer = 0;

#ifdef USE_EEPROM
uint16_t writeCurrentSettingsToFlashDelayTimer;
uint8_t writeCurrrentSettingsToFlash = 0;
int16_t magic_check;
#endif

// ============================================================
// Setup Function
// ============================================================

void setup()
{
  // Initialize serial communication
  Serial.begin(SERIAL_BAUD_RATE);

  // Initialize SPI interface
  spi_init();

  // Initialize and display startup message
  display_init();
  
  // DAB Setup
  Ensemble = &NVEnsemble.Ensemble[0];
  Dab.setCallback(ServiceData);
  Dab.speaker(SPEAKER_OUTPUT);
  Dab.begin(0);

  if (Dab.error != 0)
  {
    lcd.setCursor(0, 1);
    lcd.print("ERROR");
  }
  else
  {
#ifdef USE_EEPROM
    
    load_ensembles_from_storage();
    
    buttons = lcd.readButtons();

    if ((magic_check != MAGIC) || 
        (buttons & BUTTON_SELECT) ||
        ((dab_mode == true) && (NumOfEnsembles == 0)))
    {
      // Force rescan on invalid magic, SELECT button pressed, or no DAB stations
      dab_mode = true;
      vol = 25;
      Dab.vol(vol);
      ScanforServices();
    }
    else
    {
      // Load previously stored settings
      lcd.setCursor(0, 1);
      lcd.print("Read EEPROM..");

      read_settings_from_storage();
      Dab.vol(vol);

      if (dab_mode == true)
      {
        if (NumOfEnsembles > 0)
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Tuning ensemble:");
          lcd.setCursor(0, 1);
          lcd.print(Ensemble[ensemble].Ensemble);
          Dab.tuneservice(Ensemble[ensemble].freq_index, 
                        Ensemble[ensemble].service[service].ServiceID, 
                        Ensemble[ensemble].service[service].CompID);
        }
      }
      else
      {
        Dab.begin(1);
        Dab.tune(fm_freq);
        Dab.vol(vol);
      }
    }
#else
    ScanforServices();
#endif
  }
  
  previous = millis();
}

// ============================================================
// Main Loop Function
// ============================================================

void loop()
{
  // Perform DAB receiver task
  Dab.task();

#ifdef USE_EEPROM
  // Delayed save of settings to protect EEPROM/Flash from wear
  if (writeCurrentSettingsToFlashDelayTimer == 0 && writeCurrrentSettingsToFlash)
  {
    lcd.setCursor(0, 1);
    lcd.print("Save Setting... ");
    
    write_settings_to_storage();
    
    writeCurrrentSettingsToFlash = 0;
    lcd.setCursor(13, 1);
    lcd.print("ok");
    displayTimer = 1000;
  }
#endif

  // Read buttons with debouncing
  if (readbuttonsTimer == 0)
  {
    buttons = lcd.readButtons();
    readbuttonsTimer = 50;  // Read buttons every 50ms

    if (debouncebuttonsTimer == 0)
    {
      if (buttons > 0)
      {
        process_buttons(buttons);
        process_display();
        
        if (buttons != lastbuttons)
        {
          debouncebuttonsTimer = 500;  // Longer debounce on state change
        }
        else
        {
          debouncebuttonsTimer = 200;  // Shorter repeat delay
        }
      }
    }
    
    if (buttons == 0)
    {
      debouncebuttonsTimer = 0;
      process_buttons(buttons);
    }
    
    lastbuttons = buttons;
    process_display();
  }

  // 1ms timer tick
  uint32_t current = millis();
  uint32_t expired = current - previous;
  
  if (expired > 0)
  {
    while (expired)
    {
      timer1ms();
      expired--;
    }
    previous = current;
  }
}
