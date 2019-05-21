//
//  Runnable.h
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//

#ifndef _RUNNABLE_H
#define _RUNNABLE_H

class Runnable {
  static Runnable *headRunnable;
  Runnable *nextRunnable;
 
  public:
    Runnable() {
      nextRunnable = headRunnable;
      headRunnable = this;
    }
        
    virtual void setup() = 0;
    virtual void loop() = 0;

    static void setupAll() {
      for (Runnable *r = headRunnable; r; r = r->nextRunnable)
        r->setup();
    }

    static void loopAll() {
      for (Runnable *r = headRunnable; r; r = r->nextRunnable)
        r->loop();
    }
};

#endif
