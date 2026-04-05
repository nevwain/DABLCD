/**
 * @file config.h
 * @brief Platform-specific configuration and includes for DABLCD application
 * @author AVIT Research Ltd
 * @version 2.0
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"
#include <SPI.h>
#include <DABShield.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

// ============================================================
// Speaker Output Configuration
// ============================================================
#define SPEAKER_OUTPUT  SPEAKER_DIFF   // Options: SPEAKER_NONE, SPEAKER_DIFF, SPEAKER_STEREO

// ============================================================
// Platform-Specific Configuration
// ============================================================

#if defined(ARDUINO_UNOR4_MINIMA) || defined(ARDUINO_UNOR4_WIFI)
  //UNOR4 has 8k EEPROM
  #define MAX_ENSEMBLES 8
  #define USE_EEPROM
  #include "EEPROM.h"
#endif

#ifdef ARDUINO_ARCH_SAMD
  #define Serial SerialUSB
  //Comment out the following line if using the ICSP connector (and modified DABShield).
  #define DAB_SPI_BITBANG
  
  // Include API for FlashStorage only valid for SAMD21 (M0/Zero etc).
  // FlashStorage has copy of EEPROM in RAM... limit to 12k
  #define MAX_ENSEMBLES 12
  #define USE_EEPROM
  #define EEPROM_EMULATION_SIZE     (12 * 1024)
  #include <FlashStorage_SAMD.h>
#endif

#ifdef ARDUINO_ARCH_SAM
  #define Serial SerialUSB
  //Comment out the following line if using the ICSP connector (and modified DABShield).
  #define DAB_SPI_BITBANG
#endif

#ifdef ARDUINO_SAM_DUE
  #define USE_EEPROM
  #define DUE_FLASH
  #include <DueFlashStorage.h>
#endif

// ============================================================
// SPI Configuration
// ============================================================

// Default SPI pins
#if defined(ARDUINO_ARCH_ESP32)
  const byte SLAVE_SELECT_PIN = 12;
#else
  const byte SLAVE_SELECT_PIN = 8;
#endif

#ifdef DAB_SPI_BITBANG
  #if defined(ARDUINO_ARCH_ESP32)
    const byte SCK_PIN = 18;
    const byte MISO_PIN = 19;
    const byte MOSI_PIN = 23;
  #else
    const byte SCK_PIN = 13;
    const byte MISO_PIN = 12;
    const byte MOSI_PIN = 11;
  #endif
#endif

// ============================================================
// Default Ensemble Count (if not platform-specific)
// ============================================================
#ifndef MAX_ENSEMBLES
  #define MAX_ENSEMBLES 16
#endif

// ============================================================
// Utility Defines
// ============================================================
#define MAX_SERVICES_PER_ENSEMBLE 24
#define SERIAL_BAUD_RATE 115200

#endif // CONFIG_H
