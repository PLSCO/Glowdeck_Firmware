//
//  RightButton.h
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//


#ifndef _RIGHTBUTTON_H
#define _RIGHTBUTTON_H

#include "Button.h"

class RightButton: public Button {

  public:
    RightButton(byte attachToPin);
    
  protected:
    void shortClick();
    void longClick();
          
};

#endif
