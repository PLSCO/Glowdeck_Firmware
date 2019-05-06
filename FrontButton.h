//
//  FrontButton.h
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//


#ifndef _FRONTBUTTON_H
#define _FRONTBUTTON_H

#include "Button.h"

class FrontButton: public Button {

  public:
    FrontButton(byte attachToPin);
    
  protected:
    void shortClick();
    void longClick();

};

#endif
