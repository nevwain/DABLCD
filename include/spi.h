/**
 * @file spi.h
 * @brief SPI communication interface for DAB module
 * @author AVIT Research Ltd
 */

#ifndef SPI_H
#define SPI_H

#include <stdint.h>

/**
 * @brief Send/receive SPI message to DAB module
 * Uses either bit-bang or hardware SPI depending on platform configuration
 * 
 * @param data Pointer to data buffer (in/out)
 * @param len Length of data buffer in bytes
 */
void DABSpiMsg(unsigned char *data, uint32_t len);

/**
 * @brief Initialize SPI interface
 * Sets up pins and SPI mode based on platform configuration
 */
void spi_init(void);

#endif // SPI_H
