//
//  LeftButton.h
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//


#ifndef _LEFTBUTTON_H
#define _LEFTBUTTON_H

#include "Button.h"

class LeftButton: public Button {

  public:
    LeftButton(byte attachToPin);
    
  protected:
    void shortClick();
    void longClick();

      
};

#endif
