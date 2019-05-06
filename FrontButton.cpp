//
//  FrontButton.cpp
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//

#include "FrontButton.h"
#include "Button.h"

extern SerialManager serialManager;

FrontButton::FrontButton(byte attachToPin) : Button(attachToPin) {
}

void FrontButton::shortClick() {
  
  // Short click  
  // SerialManager::logEvent("FrontButton::shortClick()");
  serialManager.logEvent("FrontButton::shortClick()");

  
}

void FrontButton::longClick() {
  
  // Long click
  // SerialManager::logEvent("FrontButton::longClick()");
  serialManager.logEvent("FrontButton::longClick()");
}
