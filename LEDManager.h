//
//  LEDManager.h
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//

#ifndef _LEDMANAGER_H
#define _LEDMANAGER_H

#include "GlobalClass.h"

class LEDManager: public Runnable {
  public:
    LEDManager(int state);

    int ledState;
    
    void setup();
    void loop();

    static void nextPattern();
    static void rainbow();
    static void confetti();
    static void sinelon();
    static void bpm();
    static void juggle();

    void startAnimating();
    void off();

    // void rainbowCycle(int wait, int cycles, int dim);
     
  // private:
  //   LEDState _ledState;

  public:
    friend class SerialManager;
};

#endif
