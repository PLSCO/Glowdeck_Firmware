//
//  Button.h
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//

#ifndef _BUTTON_H
#define _BUTTON_H

#include "GlobalClass.h"


class Button: public Runnable {
  
  const byte pin;
  int state;
  unsigned long buttonDownMs;
  
  protected:
    virtual void shortClick() = 0;
    virtual void longClick() = 0;
  
  public:
    Button(byte attachTo);
    
    void setup();
    void loop();
};

#endif







