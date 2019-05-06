//
//  GlobalClass.h
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//


#ifndef _GLOBALCLASS_H
#define _GLOBALCLASS_H


#include "Globals.h"                        // Global variables and defines
#include "Runnable.h"                       // Optimizes setup/loop methods

// #include "Utility.h"

#include "Button.h"                         // Manages tactile buttons
#include "GlowdeckManager.h"                // Manages device properties and settings
#include "BootloaderManager.h"              // Manages bootloader and firmware upgrade
#include "SerialManager.h"                  // Manages serial ports
#include "BluetoothManager.h"               // Manages bluetooth
#include "WifiManager.h"                    // Manages wifi
#include "LEDManager.h"                     // Manages array of leds
#include "DisplayManager.h"                 // Manages display


class GlobalClass {
  
  public:
    GlobalClass();
  
  protected:
    friend class Runnable;
    friend class Button;
    friend class FrontButton;
    friend class LeftButton;
    friend class RightButton;
    friend class GlowdeckManager;
    friend class SerialManager;
    friend class BluetoothManager;
    friend class WifiManager;
    friend class BootloaderManager;
    friend class LEDManager;
    friend class DisplayManager;
};

#endif
