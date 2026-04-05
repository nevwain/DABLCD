/**
 * @file display.cpp
 * @brief LCD display and UI functions implementation
 * @author AVIT Research Ltd
 */

#include "display.h"
#include "globals.h"
#include "charset.h"
#include "config.h"
#include <stdio.h>
#include <string.h>

void display_init(void)
{
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("AVIT DAB/FM v2.0");
  lcd.setCursor(0, 1);
  lcd.print("Initialising....");
  lcd.setBacklight(1);
  
  initLCDCharacters();
  
  display_state = DISPLAY_INIT;
}

void process_display(void)
{
  switch (display_state)
  {
    case DISPLAY_INIT:
      displayTimer = 1000;
      display_state = DISPLAY_DELAY;
      break;
      
    case DISPLAY_DELAY:
      if (displayTimer == 0)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        
        if (dab_mode == true)
        {
          if (NumOfEnsembles > 0)
          {
            lcd.print(Ensemble[ensemble].service[service].Label);
          }
          else
          {
            lcd.setCursor(0, 0);
            lcd.print("No DAB stations ");
          }
        }
        else
        {
          // FM mode display
          char freqstring[32];
          if (strlen(fm_ps) == 0)
          {
            sprintf(freqstring, "%-8s   %3d.%01d", "FM      ", fm_freq / 100, (fm_freq % 100) / 10);
          }
          else
          {
            sprintf(freqstring, "%-8s   %3d.%01d", fm_ps, fm_freq / 100, (fm_freq % 100) / 10);
          }
          lcd.print(freqstring);
        }
        
        text_index = 0;
        lastMinutes = 99;
        display_state = DISPLAY_RUN;
      }
      break;
      
    case DISPLAY_RUN:
      if (displayTimer == 0)
      {
        char text[17];
        uint8_t text_len;

        if (text_mode == TEXTMODE_TIME)
        {
          // Display time
          DABTime dabtime;
          char timestring[16];

          if (dab_mode == true)
          {
            if (NumOfEnsembles > 0)
            {
              Dab.time(&dabtime);
            }
            else
            {
              lcd.setCursor(0, 0);
              lcd.print("No DAB stations ");
            }
          }
          else
          {
            // FM mode - use DAB time
            dabtime.Days = Dab.Days;
            dabtime.Months = Dab.Months;
            dabtime.Year = Dab.Year;
            dabtime.Hours = Dab.Hours;
            dabtime.Minutes = Dab.Minutes;
            dabtime.Seconds = 0;
          }
          
          if (dabtime.Minutes != lastMinutes)
          {
            lastMinutes = dabtime.Minutes;
            lcd.setCursor(0, 1);
            sprintf(timestring, "%02d/%02d/%02d ", dabtime.Days, dabtime.Months, dabtime.Year);
            lcd.print(timestring);
            sprintf(timestring, "%02d:%02d  ", dabtime.Hours, dabtime.Minutes);
            lcd.print(timestring);
          }
          displayTimer = 1000;
        }
        else if (text_mode == TEXTMODE_SERVICEDATA)
        {
          // Display service data (radio text)
          if ((NumOfEnsembles == 0) && (dab_mode == true))
          {
            lcd.setCursor(0, 0);
            lcd.print("No DAB stations ");
          }
          else
          {
            lcd.setCursor(0, 1);
            text_len = strlen(service_text);
            
            if (text_len > 0)
            {
              strncpy(text, &service_text[text_index], 16);
              for (uint8_t i = text_len; i < 16; i++)
              {
                text[i] = ' ';
              }
              text[16] = '\0';

              lcd.print(text);

              if (text_len > 16)
              {
                text_index++;
              }

              if (text_index == 1)
              {
                displayTimer = 1000;
              }
              else if ((text_index + 16) > text_len)
              {
                text_index = 0;
                displayTimer = 1000;
              }
              else
              {
                displayTimer = 333;  // Scroll speed
              }
            }
            else
            {
              lcd.print("                ");
              displayTimer = 1000;
            }
          }
        }
        else  // TEXTMODE_ENSEMBLE
        {
          // Display ensemble name
          if ((NumOfEnsembles == 0) && (dab_mode == true))
          {
            lcd.setCursor(0, 0);
            lcd.print("No DAB stations ");
          }
          else
          {
            lcd.setCursor(0, 1);
            text_len = strlen(Ensemble[ensemble].Ensemble);
            
            if (text_len > 0)
            {
              strncpy(text, Ensemble[ensemble].Ensemble, 16);
              for (uint8_t i = text_len; i < 16; i++)
              {
                text[i] = ' ';
              }
              text[16] = '\0';
              lcd.print(text);
            }
            displayTimer = 1000;
          }
        }
      }
      break;
      
    case DISPLAY_SERVICE:
      lcd.clear();
      lcd.setCursor(0, 0);
      
      if (dab_mode == true)
      {
        if (NumOfEnsembles > 0)
        {
          lcd.print(Ensemble[ensemble].service[service].Label);
        }
        else
        {
          lcd.setCursor(0, 0);
          lcd.print("No DAB stations ");
        }
      }
      else
      {
        // FM mode service display
        char freqstring[32];
        if (strlen(fm_ps) == 0)
        {
          sprintf(freqstring, "%-8s   %3d.%01d", "FM      ", fm_freq / 100, (fm_freq % 100) / 10);
        }
        else
        {
          sprintf(freqstring, "%-8s   %3d.%01d", fm_ps, fm_freq / 100, (fm_freq % 100) / 10);
        }
        lcd.print(freqstring);
      }
      
      text_index = 0;
      lastMinutes = 99;
      display_state = DISPLAY_RUN;
      break;
      
    case DISPLAY_VOL:
      lcd.setCursor(0, 1);
      lcd.print("Vol:            ");
      if (vol > 9)
        lcd.setCursor(5, 1);
      else
        lcd.setCursor(6, 1);
      lcd.print(vol);
      display_state = DISPLAY_DELAY;
      displayTimer = 1000;
      break;
      
    case DISPLAY_MENU:
      lcd.setCursor(0, 0);
      lcd.print("Menu            ");
      lcd.setCursor(0, 1);
      
      if (dab_mode == true)
      {
        if (menu_mode <= 1)
        {
          lcd.print("FM Mode         ");
        }
        else if (menu_mode == 2)
        {
          lcd.print("Re-SCAN         ");
        }
      }
      else
      {
        if (menu_mode <= 1)
        {
          lcd.print("DAB Mode        ");
        }
      }
      break;
  }
}
