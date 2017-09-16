//
//  BootloaderManager.cpp
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//

#include "BootloaderManager.h"
#include "EEPROM.h"


extern SerialManager serialManager;

BootloaderManager::BootloaderManager() {
  return;
}

void BootloaderManager::enterBootloaderMode() {

  // Writing 13 to address 0 causes MCU to enter bootloader mode on startup...
  EEPROM.write(0, 13);

  // Short delay for EEPROM write... (needed?)
  delay(25);

  // Clear out all serial ports
  serialManager.clearAll();

  // Reboot the MCU...
  CPU_RESTART;

}







