/**
 * @file dab_services.h
 * @brief DAB service scanning and management
 * @author AVIT Research Ltd
 */

#ifndef DAB_SERVICES_H
#define DAB_SERVICES_H

#include <stdint.h>

/**
 * @brief Scan for all available DAB services
 * Performs a full frequency scan and stores found ensembles
 * 
 * @return Number of ensembles found
 */
void ScanforServices(void);

/**
 * @brief Callback function triggered when new service data is received
 * Called by DABShield library when radio text or PS name updates
 */
void ServiceData(void);

/**
 * @brief Internal DAB frequency scan
 * 
 * @return Number of ensembles found
 */
uint8_t DAB_scan(void);

#endif // DAB_SERVICES_H
