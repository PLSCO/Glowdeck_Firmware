//
//  DisplayManager.h
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//

#ifndef _DISPLAYMANAGER_H
#define _DISPLAYMANAGER_H

#include "GlobalClass.h"

class DisplayManager: public Runnable {
  
  public:
    DisplayManager(uint8_t clk, uint8_t mosi, uint8_t cs, uint8_t rst, uint8_t dc, uint8_t bl);

    void setup();
    void loop();

    static void printStream(String type, String text, String source, String keyword);
    static void printBluetooth(uint8_t on);
    static void printWifi(uint8_t on);
    static void printTypeIcon(String type);
    static void printHeadline(String text, String focusWord);
    static void printSource(String text);
    static void printStreamSpectrum(String active, int q);
    static void printStreamSquare(String type, int id, int q);
    static void printStreamBorder(void);

    uint8_t brightness      = 10;
    bool    autoBrightness  = false;
  
  private:
    uint8_t _clk, _mosi, _cs, _rst, _dc, _bl;
    
};

#endif







