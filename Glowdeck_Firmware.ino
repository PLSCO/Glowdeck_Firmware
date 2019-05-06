//
//  Glowdeck_Firmware.ino
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//

#include "GlobalClass.h"

// TODO: Move these to GlobalClass
#include "FrontButton.h"
#include "LeftButton.h"
#include "RightButton.h"

// MARK: Globals

Runnable              *Runnable::headRunnable = NULL;

FrontButton           frontButton         (FRONT_BUTTON);
LeftButton            leftButton          (LEFT_BUTTON);
RightButton           rightButton         (RIGHT_BUTTON);

GlowdeckManager       glowdeckManager     ();
BootloaderManager     bootloaderManager   ();
SerialManager         serialManager       (&Serial );
WifiManager           wifiManager         (&Serial1);
BluetoothManager      bluetoothManager    (&Serial2);
LEDManager            ledManager          (1);
DisplayManager        displayManager      (LCD_SCK, LCD_MOSI, LCD_CS, LCD_RST, LCD_DC, LCD_BL);

// MARK: Setup

void setup() {
  Runnable::setupAll();
}

// MARK: Loop

void loop() {
  Runnable::loopAll();
}
