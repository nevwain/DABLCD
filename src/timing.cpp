/**
 * @file timing.cpp
 * @brief Timing and timer functions implementation
 * @author AVIT Research Ltd
 */

#include "timing.h"
#include "globals.h"
#include "config.h"

void timer1ms(void)
{
  // Decrement all active timers
  if (readbuttonsTimer > 0)
  {
    readbuttonsTimer--;
  }
  
  if (debouncebuttonsTimer > 0)
  {
    debouncebuttonsTimer--;
  }
  
  if (displayTimer > 0)
  {
    displayTimer--;
  }
  
  if (menu_timer > 0)
  {
    menu_timer--;
    if (menu_timer == 0)
    {
      menu_mode = 0;
      display_state = DISPLAY_DELAY;
      displayTimer = 0;
    }
  }

#ifdef USE_EEPROM
  if (writeCurrentSettingsToFlashDelayTimer > 0)
  {
    writeCurrentSettingsToFlashDelayTimer--;
  }
#endif
}
