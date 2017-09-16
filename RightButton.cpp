//
//  RightButton.cpp
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//

#include "RightButton.h"
#include "Button.h"

RightButton::RightButton(byte attachToPin) : Button(attachToPin) {
}

void RightButton::shortClick() {
  
  // Short click  
  SerialManager::logEvent("RightButton::shortClick()");
  
}

void RightButton::longClick() {
  
  // Long click
  SerialManager::logEvent("RightButton::longClick()");
  
}








