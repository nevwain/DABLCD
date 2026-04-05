/**
 * @file controls.cpp
 * @brief Button and input control handling implementation
 * @author AVIT Research Ltd
 */

#include "controls.h"
#include "globals.h"
#include "display.h"
#include "dab_services.h"
#include "config.h"

// ============================================================
// Volume Control
// ============================================================

void DAB_VolUp(void)
{
  if (vol < 63)
  {
    vol++;
    Dab.vol(vol);
#ifdef USE_EEPROM
    writeCurrrentSettingsToFlash = true;
    writeCurrentSettingsToFlashDelayTimer = WAIT_SAVECURRENTSETTINGS;
#endif
  }
}

void DAB_VolDown(void)
{
  if (vol > 0)
  {
    vol--;
    Dab.vol(vol);
#ifdef USE_EEPROM
    writeCurrrentSettingsToFlash = true;
    writeCurrentSettingsToFlashDelayTimer = WAIT_SAVECURRENTSETTINGS;
#endif
  }
}

// ============================================================
// Service Navigation
// ============================================================

void DAB_NextService(void)
{
  if (Ensemble[ensemble].numberofservices > 0)
  {
    if (service < (Ensemble[ensemble].numberofservices - 1))
    {
      service++;
    }
    else
    {
      ensemble++;
      if (ensemble > (NumOfEnsembles - 1))
      {
        ensemble = 0;
      }
      service = 0;
    }
    service_text[0] = '\0';
#ifdef USE_EEPROM
    writeCurrrentSettingsToFlash = true;
    writeCurrentSettingsToFlashDelayTimer = WAIT_SAVECURRENTSETTINGS;
#endif
  }
}

void DAB_PreviousService(void)
{
  if (Ensemble[ensemble].numberofservices > 0)
  {
    if (service > 0)
    {
      service--;
    }
    else
    {
      if (ensemble > 0)
      {
        ensemble--;
      }
      else
      {
        ensemble = NumOfEnsembles - 1;
      }
      service = Ensemble[ensemble].numberofservices - 1;
    }
    service_text[0] = '\0';
#ifdef USE_EEPROM
    writeCurrrentSettingsToFlash = true;
    writeCurrentSettingsToFlashDelayTimer = WAIT_SAVECURRENTSETTINGS;
#endif
  }
}

// ============================================================
// Button Processing
// ============================================================

void process_buttons(uint8_t buttons)
{
  if (menu_mode == 0)
  {
    // Normal mode - handle standard controls
    
    if (buttons & BUTTON_UP)
    {
      DAB_VolUp();
      display_state = DISPLAY_VOL;
    }
    
    if (buttons & BUTTON_DOWN)
    {
      DAB_VolDown();
      display_state = DISPLAY_VOL;
    }
    
    if (buttons & BUTTON_LEFT)
    {
      if (dab_mode == true)
      {
        seek_timer = 5;
        DAB_PreviousService();
        display_state = DISPLAY_SERVICE;
      }
      else
      {
        // FM mode - seek down
        if (seek_timer < 5)
        {
          seek_dir = 0;
          seek_timer++;
        }
        if (seek_timer == 5)
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("FM Seek: down   ");
          Dab.seek(seek_dir, 1);
          fm_freq = Dab.freq;
#ifdef USE_EEPROM
          writeCurrrentSettingsToFlash = true;
          writeCurrentSettingsToFlashDelayTimer = WAIT_SAVECURRENTSETTINGS;
#endif
          service_text[0] = '\0';
          fm_ps[0] = '\0';
          display_state = DISPLAY_SERVICE;
          seek_timer++;
        }
      }
    }
    
    if (buttons & BUTTON_RIGHT)
    {
      if (dab_mode == true)
      {
        seek_timer = 5;
        DAB_NextService();
        display_state = DISPLAY_SERVICE;
      }
      else
      {
        // FM mode - seek up
        if (seek_timer < 5)
        {
          seek_dir = 1;
          seek_timer++;
        }
        if (seek_timer == 5)
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("FM Seek: up     ");
          Dab.seek(seek_dir, 1);
          fm_freq = Dab.freq;
#ifdef USE_EEPROM
          writeCurrrentSettingsToFlash = true;
          writeCurrentSettingsToFlashDelayTimer = WAIT_SAVECURRENTSETTINGS;
#endif
          service_text[0] = '\0';
          fm_ps[0] = '\0';
          display_state = DISPLAY_SERVICE;
          seek_timer++;
        }
      }
    }
    
    if (buttons & BUTTON_SELECT)
    {
      if (button_select_timer < 10)
      {
        button_select_timer++;
      }
      else if (button_select_timer == 10)
      {
        display_state = DISPLAY_MENU;
        process_display();
        button_select_timer++;
      }
    }
    
    // Button release handling
    if (buttons == 0)
    {
      // Seek release
      if (seek_timer > 0)
      {
        if (dab_mode == true)
        {
          // DAB seek release - tune to selected service
          if (seek_timer)
          {
            seek_timer--;
            if (seek_timer == 0)
            {
              lcd.setCursor(0, 1);
              lcd.print("tuning...       ");
              Dab.tuneservice(Ensemble[ensemble].freq_index, 
                            Ensemble[ensemble].service[service].ServiceID, 
                            Ensemble[ensemble].service[service].CompID);
              display_state = DISPLAY_DELAY;
              displayTimer = 1000;
            }
          }
        }
        else
        {
          // FM seek release - increment/decrement frequency
          if (seek_timer < 5)
          {
            if (seek_dir == 1)
            {
              if (fm_freq < 10800)
                fm_freq += 10;
              else
                fm_freq = 8750;
            }
            else
            {
              if (fm_freq > 8750)
                fm_freq -= 10;
              else
                fm_freq = 10800;
            }
            Dab.tune(fm_freq);
#ifdef USE_EEPROM
            writeCurrrentSettingsToFlash = true;
            writeCurrentSettingsToFlashDelayTimer = WAIT_SAVECURRENTSETTINGS;
#endif
            service_text[0] = '\0';
            fm_ps[0] = '\0';
            display_state = DISPLAY_SERVICE;
          }
          seek_timer = 0;
        }
      }
      
      // Select button release
      else if (button_select_timer > 0)
      {
        if (button_select_timer > 10)
        {
          // Long press - enter menu
          menu_mode = 1;
          menu_timer = 5000;
        }
        else
        {
          // Short press - toggle display mode
          switch (text_mode)
          {
            case TEXTMODE_SERVICEDATA:
              text_mode = TEXTMODE_TIME;
              break;
            case TEXTMODE_TIME:
              if (dab_mode == true)
                text_mode = TEXTMODE_ENSEMBLE;
              else
                text_mode = TEXTMODE_SERVICEDATA;
              break;
            case TEXTMODE_ENSEMBLE:
              text_mode = TEXTMODE_SERVICEDATA;
              break;
            default:
              text_mode = TEXTMODE_SERVICEDATA;
              break;
          }
          displayTimer = 0;
          text_index = 0;
          lastMinutes = 99;
        }
        button_select_timer = 0;
      }
    }
  }
  else
  {
    // Menu mode
    
    if (dab_mode == true)
    {
      // DAB mode menu
      if (buttons & BUTTON_RIGHT)
      {
        menu_timer = 5000;
        if (menu_mode < 2)
          menu_mode++;
      }
      
      if (buttons & BUTTON_LEFT)
      {
        menu_timer = 5000;
        if (menu_mode > 1)
          menu_mode--;
      }
      
      // FM Mode selection
      if (menu_mode == 1 && (buttons & BUTTON_SELECT))
      {
        Dab.begin(1);
        Dab.tune(fm_freq);
#ifdef USE_EEPROM
        writeCurrrentSettingsToFlash = true;
        writeCurrentSettingsToFlashDelayTimer = WAIT_SAVECURRENTSETTINGS;
#endif
        text_mode = TEXTMODE_SERVICEDATA;
        dab_mode = false;
        Dab.vol(vol);
        service_text[0] = '\0';
        fm_ps[0] = '\0';
        menu_mode = 0;
        menu_timer = 0;
        display_state = DISPLAY_DELAY;
        displayTimer = 0;
      }
      
      // Re-scan option
      else if (menu_mode == 2 && (buttons & BUTTON_SELECT))
      {
        lcd.setCursor(0, 0);
        lcd.print("Re-SCAN         ");
        ScanforServices();
        service_text[0] = '\0';
        fm_ps[0] = '\0';
        menu_mode = 0;
        menu_timer = 0;
        display_state = DISPLAY_DELAY;
        displayTimer = 0;
      }
    }
    else
    {
      // FM mode menu
      if (buttons & BUTTON_RIGHT)
      {
        menu_timer = 5000;
        if (menu_mode < 1)
          menu_mode++;
      }
      
      if (buttons & BUTTON_LEFT)
      {
        menu_timer = 5000;
        if (menu_mode > 1)
          menu_mode--;
      }
      
      // DAB Mode selection
      if (menu_mode == 1 && (buttons & BUTTON_SELECT))
      {
        Dab.begin(0);
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
        else
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("No DAB stations ");
        }
        dab_mode = true;
        Dab.vol(vol);
        service_text[0] = '\0';
        fm_ps[0] = '\0';
        menu_mode = 0;
        menu_timer = 0;
        display_state = DISPLAY_DELAY;
        displayTimer = 0;
      }
    }
  }
}
