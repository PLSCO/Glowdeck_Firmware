//
//  Button.cpp
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//

#include "Button.h"

Button::Button(byte attachTo) : pin(attachTo) { 
}

void Button::setup() {
  
    pinMode(pin, INPUT_PULLUP);
    
    state = HIGH;
    
}

void Button::loop() {
  
  int prevState = state;
  
  state = digitalRead(pin);
  
  if (prevState == HIGH && state == LOW) {
    
    buttonDownMs = millis();
    
  }
  else if (prevState == LOW && state == HIGH) {
    
    if (millis() - buttonDownMs < 50) {
      
      // ignore this for debounce
      
    }
    else if (millis() - buttonDownMs < 250) {
      
      shortClick();
      
      // Short click  
      // SerialManager::logEvent("FrontButton::shortClick()");
          
      
    }
    else  {
      
      longClick();

      // Long click
      // SerialManager::logEvent("FrontButton::longClick()");
      
    }
    
  }
  
}

void Button::shortClick() {
  
}

void Button::longClick() {
  
}








