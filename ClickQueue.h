//
//  ClickQueue.h
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//


#ifndef _CLICKQUEUE_H
#define _CLICKQUEUE_H

#include "GlobalClass.h"

class ClickQueue: public Runnable {
    const byte pin;
  
    // Slow for testing
    const unsigned CLICK_DOWN_MS = 250;
    const unsigned CLICK_TOTAL_MS = 1000;

    enum State {
      NOT_CLICKING = 0,
      CLICK_DOWN = 1,
      CLICK_PAUSE = 2
    } state;

    unsigned long clickStartMs;    
    int pendingClicks;

  public:
    ClickQueue(byte attachTo);

    void setup();
    void loop();

    void addAClick();
    void addClicks(int clicks);
    void cancel();
};

#endif
