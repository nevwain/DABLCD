/**
 * @file dab_services.cpp
 * @brief DAB service scanning and management implementation
 * @author AVIT Research Ltd
 */

#include "dab_services.h"
#include "globals.h"
#include "charset.h"
#include "storage.h"
#include "config.h"
#include <string.h>

void ServiceData(void)
{
  // Remove spaces from end of string
  for (uint8_t i = strlen(Dab.ServiceData) - 1; i > 0; i--)
  {
    if (Dab.ServiceData[i] == ' ')
    {
      Dab.ServiceData[i] = '\0';
    }
    else
    {
      break;
    }
  }
  
  // Convert charset for LCD display
  replaceSpecialCharsInplace(Dab.ServiceData, strlen(Dab.ServiceData));

  // Compare to previous text
  if (strcmp(service_text, Dab.ServiceData) != 0)
  {
    strcpy(service_text, Dab.ServiceData);
    text_index = 0;
  }
  
  // Handle PS (Program Service) name for FM
  replaceSpecialCharsInplace(Dab.ps, strlen(Dab.ps));
  if (strcmp(fm_ps, (char *)Dab.ps) != 0)
  {
    strcpy(fm_ps, (char *)Dab.ps);
    display_state = DISPLAY_SERVICE;
  }
}

uint8_t DAB_scan(void)
{
  uint8_t freq_index;
  uint8_t ensemble_index;
  uint8_t numofservices;
  uint8_t i;

  ensemble_index = 0;

  // Scan all DAB frequencies
  for (freq_index = 0; freq_index < DAB_FREQS; freq_index++)
  {
    // Display progress on LCD
    if (freq_index > 9)
      lcd.setCursor(11, 1);
    else
      lcd.setCursor(12, 1);
    lcd.print(freq_index);
    lcd.setCursor(13, 1);
    lcd.print("/37");

    Dab.tune(freq_index);
    
    if (Dab.servicevalid() == true)
    {
      // Found a valid ensemble with services
      Ensemble[ensemble_index].freq_index = freq_index;
      
      // Convert charset for LCD display
      replaceSpecialCharsInplace(Dab.Ensemble, strlen(Dab.Ensemble));
      strcpy(Ensemble[ensemble_index].Ensemble, Dab.Ensemble);
      
      // Ensure we don't overflow our array
      numofservices = Dab.numberofservices;
      if (numofservices > MAX_SERVICES_PER_ENSEMBLE)
      {
        numofservices = MAX_SERVICES_PER_ENSEMBLE;
      }
      Ensemble[ensemble_index].numberofservices = numofservices;
      
      // Copy all services for this ensemble
      for (i = 0; i < numofservices; i++)
      {
        Ensemble[ensemble_index].service[i].ServiceID = Dab.service[i].ServiceID;
        Ensemble[ensemble_index].service[i].CompID = Dab.service[i].CompID;
        
        // Convert charset for LCD display
        replaceSpecialCharsInplace(Dab.service[i].Label, strlen(Dab.service[i].Label));
        strcpy(Ensemble[ensemble_index].service[i].Label, Dab.service[i].Label);
      }
      
      ensemble_index++;
      
      // Limit to our array size
      if (ensemble_index == MAX_ENSEMBLES)
      {
        break;
      }
    }
  }
  
  return ensemble_index;
}

void ScanforServices(void)
{
  // Display scanning message
  lcd.setCursor(0, 1);
  lcd.print("Scanning...     ");

  ensemble = 0;
  service = 0;
  NumOfEnsembles = DAB_scan();

#ifdef USE_EEPROM
  // Write to EEPROM even if nothing found
  // EEPROM will be needed if we switch to FM
  lcd.setCursor(0, 1);
  lcd.print("save to EEPROM..");
  
  save_ensembles_to_storage();
#endif

  if (NumOfEnsembles > 0)
  {
    // Something has been found - tune to first service
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tuning ensemble:");
    lcd.setCursor(0, 1);
    lcd.print(Ensemble[ensemble].Ensemble);

    Dab.tune(Ensemble[ensemble].freq_index);
    Dab.set_service(service);
  }
}
