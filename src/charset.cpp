/**
 * @file charset.cpp
 * @brief LCD character set conversion utilities implementation
 * @author AVIT Research Ltd
 */

#include "charset.h"
#include "globals.h"

// ============================================================
// LCD Custom Character Data
// ============================================================

// Accented vowel character bitmaps
const uint8_t abar[] = {0x0E, 0x00, 0x0E, 0x01, 0x0F, 0x11, 0x0F, 0x00};  // ā (0x08)
const uint8_t ebar[] = {0x0E, 0x00, 0x0E, 0x11, 0x1F, 0x10, 0x0E, 0x00};  // ē (0x09)
const uint8_t ibar[] = {0x0E, 0x00, 0x0C, 0x04, 0x04, 0x04, 0x0E, 0x00};  // ī (0x0A)
const uint8_t obar[] = {0x0E, 0x00, 0x0E, 0x11, 0x11, 0x11, 0x0E, 0x00};  // ō (0x0B)
const uint8_t ubar[] = {0x0E, 0x00, 0x11, 0x11, 0x11, 0x13, 0x0D, 0x00};  // ū (0x0C)

// ============================================================
// Character Conversion Table
// ============================================================

/**
 * Conversion table from extended EBU Latin (DAB/RDS) to LCD charset
 * Maps characters 0x80-0xFF to appropriate LCD display characters
 */
const char EBULatintoLCD[0x80] = {
  0x08, 0x08, 0x09, 0x09, 0x0A, 0x0A, 0x0B, 0x0B, 0x0C, 0x0C, 'N',  'C',  'S',  0xE2, 'i',  'Y',
  0x08, 0x08, 0x09, 0x09, 0x0A, 0x0A, 0x0B, 0x0B, 0x0C, 0x0C, 'n',  'c',  's',  'g',  'i',  'y', 
  'a',  'a',  'c',  '/',  'G',  'e',  'n',  0x0B, 0xF7, '$',  '$',  '$',  0x7F, '^',  0x7E, 'v', 
  '0',  '1',  '2',  '3',  '+',  'I',  'n',  0x0C, 0xE4, '?',  0xFD, 0xDF, '_',  '_',  '_',  '_', 
  'A',  'A',  'E',  'E',  'I',  'I',  'O',  'O',  'U',  'U',  'R',  'C',  'S',  'Z',  'D',  'L', 
  'A',  'A',  'E',  'E',  'I',  'I',  'O',  'O',  'U',  'U',  'r',  'c',  's',  'z',  'd',  'l', 
  'A',  'A',  'E',  'E',  'y',  'Y',  'O',  'O',  'p',  'n',  'R',  'C',  'S',  'Z',  't',  'd', 
  0x08, 0x08, 'e',  'e',  'w',  'y',  'O',  'o',  'p',  'n',  'r',  'c',  's',  'z',  't',  ' '
};

// ============================================================
// Function Implementations
// ============================================================

void replaceSpecialCharsInplace(char *p, int len)
{
  for (int j = 0; j < len; j++)
  {
    // Correct wrong chars for our LCD display
    unsigned char *cc = (unsigned char *)&p[j];
    if (*cc > 0x7F)
      *cc = EBULatintoLCD[*cc - 0x80];
  }
}

void initLCDCharacters(void)
{
  lcd.createChar(0, (uint8_t *)abar);
  lcd.createChar(1, (uint8_t *)ebar);
  lcd.createChar(2, (uint8_t *)ibar);
  lcd.createChar(3, (uint8_t *)obar);
  lcd.createChar(4, (uint8_t *)ubar);
}
