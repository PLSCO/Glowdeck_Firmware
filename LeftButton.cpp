//
//  LeftButton.cpp
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//

#include "LeftButton.h"
#include "Button.h"


LeftButton::LeftButton(byte attachToPin) : Button(attachToPin) {
}

void LeftButton::shortClick() {
  
  // Short click  
  SerialManager::logEvent("LeftButton::shortClick()");
  
}

void LeftButton::longClick() {
  
  // Long click
  SerialManager::logEvent("LeftButton::longClick()");
  
}








