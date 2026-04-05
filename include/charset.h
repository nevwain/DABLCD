/**
 * @file charset.h
 * @brief LCD character set conversion utilities
 * @author AVIT Research Ltd
 */

#ifndef CHARSET_H
#define CHARSET_H

#include <stdint.h>

// LCD custom character data for accented vowels
extern const uint8_t abar[];  // a with bar (0x08)
extern const uint8_t ebar[];  // e with bar (0x09)
extern const uint8_t ibar[];  // i with bar (0x0A)
extern const uint8_t obar[];  // o with bar (0x0B)
extern const uint8_t ubar[];  // u with bar (0x0C)

// Character conversion table from EBU Latin to LCD charset
extern const char EBULatintoLCD[0x80];

/**
 * @brief Replace special characters in place in a string
 * Converts EBU Latin extended characters (0x80-0xFF) to LCD charset
 * 
 * @param p Pointer to string buffer
 * @param len Length of string to process
 */
void replaceSpecialCharsInplace(char *p, int len);

/**
 * @brief Initialize LCD custom characters
 * Creates custom characters for accented vowels on the LCD
 */
void initLCDCharacters(void);

#endif // CHARSET_H
