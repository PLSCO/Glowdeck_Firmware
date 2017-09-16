//
//  ClickQueue.h
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//

#include "ClickQueue.h"


ClickQueue::ClickQueue(byte attachTo) : pin(attachTo) {  
}

void ClickQueue::setup() {
  
  pinMode(pin, OUTPUT);
  
  state = NOT_CLICKING;
  
  pendingClicks = 0;
  
}

void ClickQueue::loop() {
  
  switch (state) {
    
    case NOT_CLICKING:
    
      if (pendingClicks > 0) {
        
        pendingClicks --;
        
        digitalWrite(pin, HIGH);
        
        clickStartMs = millis();
        
        state = CLICK_DOWN;
        
      }
      
      break;
      
    case CLICK_DOWN:
    
      if (millis() - clickStartMs > CLICK_DOWN_MS) {
        
        digitalWrite(pin, LOW);
        
        state = CLICK_PAUSE;
        
      }
      
      break;
      
    case CLICK_PAUSE:
    
      if (millis() - clickStartMs > CLICK_TOTAL_MS) {
        
        state = NOT_CLICKING;

      }
      
      break;
      
  }
  
}

void ClickQueue::addAClick() {
  
  pendingClicks++;
  
}

void ClickQueue::addClicks(int clicks) {
  
  pendingClicks += clicks;
  
}

void ClickQueue::cancel() {
  
  pendingClicks = 0;
  
  digitalWrite(pin, LOW);
  
  state = NOT_CLICKING;
  
}









