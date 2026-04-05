/**
 * @file spi.cpp
 * @brief SPI communication interface implementation
 * @author AVIT Research Ltd
 */

#include "spi.h"
#include "config.h"

void spi_init(void)
{
#ifdef DAB_SPI_BITBANG
  pinMode(SLAVE_SELECT_PIN, OUTPUT);
  pinMode(SCK_PIN, OUTPUT);
  pinMode(MOSI_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT_PULLUP);
  digitalWrite(SLAVE_SELECT_PIN, HIGH);
#else
  pinMode(SLAVE_SELECT_PIN, OUTPUT);
  digitalWrite(SLAVE_SELECT_PIN, HIGH);
  SPI.begin();
#endif
}

#ifdef DAB_SPI_BITBANG

/**
 * Bit-bang SPI implementation for DAB module communication
 * Used on platforms where hardware SPI is unavailable or modified
 */
void DABSpiMsg(unsigned char *data, uint32_t len)
{
  digitalWrite(SCK_PIN, LOW);
  digitalWrite(SLAVE_SELECT_PIN, LOW);
  
  for (uint32_t l = 0; l < len; l++)
  {
    unsigned char spiByte = data[l];
    
    for (uint32_t i = 0; i < 8; i++)
    {
      digitalWrite(MOSI_PIN, (spiByte & 0x80) ? HIGH : LOW);
      delayMicroseconds(1);
      digitalWrite(SCK_PIN, HIGH);
      spiByte = (spiByte << 1) | digitalRead(MISO_PIN);
      digitalWrite(SCK_PIN, LOW);
      delayMicroseconds(1);
    }
    
    data[l] = spiByte;
  }
  
  digitalWrite(SLAVE_SELECT_PIN, HIGH);
}

#else

/**
 * Hardware SPI implementation for DAB module communication
 * Uses the native SPI peripheral for better performance
 */
void DABSpiMsg(unsigned char *data, uint32_t len)
{
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));  // 2MHz
  digitalWrite(SLAVE_SELECT_PIN, LOW);
  SPI.transfer(data, len);
  digitalWrite(SLAVE_SELECT_PIN, HIGH);
  SPI.endTransaction();
}

#endif
