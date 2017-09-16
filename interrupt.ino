void ISR_F() {
  detach_interrupts(); //TEMPORARILY DISABLE INTERRUPTS
  
  if ((long)(micros() - last_micros_f) >= (debounce*1000)) { 
    front_btn(0);
    if (digitalRead(PRESS_F) == LOW) { // STILL PRESSED
      long downTime; long downStart = millis();
      while (1)
      {
        downTime = (millis() - downStart); digitalRead(PRESS_F);
        if (digitalRead(PRESS_F) == HIGH) //UNPRESSED
        {
          if (downTime > hold_time) //POST-HOLD THRESHOLD (HELD)
          {
            front_btn(2); hold_f = 1; last_micros_f = micros(); reattach_interrupts(); return;
          }
          else //PRE-HOLD THRESHOLD (PRESSED)
          {
            //CLICKED OR DOUBLE CLICKED?
            long dbl_wait = millis();
            do
            {
              leds_off();
              if (digitalRead(PRESS_F) == LOW) //DOUBLE CLICKED (WITHIN 750 MS WINDOW) 
              {
                front_btn(3); last_micros_f = micros(); reattach_interrupts(); return;
              }
            } while (millis() - dbl_wait < 700);
            front_btn(1); last_micros_f = micros(); reattach_interrupts(); return; //SINGLE CLICKED
          }
        }
        if (downTime > hold_time) //REACHED TIME LIMIT
        {
          front_btn(2); hold_f = 1; last_micros_f = micros(); reattach_interrupts(); return;
        }            
      }
    }
    else  //NOT STILL PRESSED
    {
      //CLICKED OR DOUBLE CLICKED?
      long dbl_wait = millis();
      do
      {
        leds_off();
        if (digitalRead(PRESS_F) == LOW) //DOUBLE CLICKED (WITHIN 750 MS WINDOW) 
        {
          front_btn(3); last_micros_f = micros(); reattach_interrupts(); return;
        }
      } while (millis() - dbl_wait < 700);
      front_btn(1); last_micros_f = micros(); reattach_interrupts(); return; //SINGLE CLICKED
    }
  }
  else
  {
    last_micros_f = micros(); reattach_interrupts(); return;
  }
}

void ISR_L() {
  detach_interrupts(); //TEMPORARILY DISABLE INTERRUPTS
  
  if ((long)(micros() - last_micros_l) >= (debounce*1000)) 
  { 
    left_btn(0);
    if (digitalRead(PRESS_L) == LOW) //STILL PRESSED (HOLD OR PRESS)
    {
      long downTime; long downStart = millis();
      while (1)
      {
        downTime = (millis() - downStart); digitalRead(PRESS_L);
        if (digitalRead(PRESS_L) == HIGH) //UNPRESSED
        {
          if (downTime > hold_time) //POST-HOLD THRESHOLD (HELD)
          {
            left_btn(2); hold_l = 1; last_micros_l = micros(); reattach_interrupts(); return;
          }
          else //PRE-HOLD THRESHOLD (PRESSED)
          {
            left_btn(1); last_micros_l = micros(); reattach_interrupts(); return;           
          }
        }
        if (downTime > hold_time) //REACHED TIME LIMIT (HELD)
        {
          left_btn(2); hold_l = 1; last_micros_l = micros(); reattach_interrupts(); return;
        }            
      }
    }
    else  //NOT STILL PRESSED
    {
      left_btn(1); last_micros_l = micros(); reattach_interrupts(); return;             
    }
  }
  else
  {
    last_micros_l = micros(); reattach_interrupts(); return;
  }
}

void ISR_R() {
  detach_interrupts(); //TEMPORARILY DISABLE INTERRUPTS
  
  if ((long)(micros() - last_micros_r) >= (debounce*1000)) 
  { 
    right_btn(0);
    if (digitalRead(PRESS_R) == LOW) //STILL PRESSED (HOLD OR PRESS)
    {
      long downTime; long downStart = millis();
      while (1)
      {
        downTime = (millis() - downStart); digitalRead(PRESS_R);
        if (digitalRead(PRESS_R) == HIGH) //UNPRESSED
        {
          if (downTime > hold_time) //POST-HOLD THRESHOLD (HELD)
          {
            right_btn(2); hold_r = 1; last_micros_r = micros(); reattach_interrupts(); return;
          }
          else //PRE-HOLD THRESHOLD (PRESSED)
          {
            right_btn(1); last_micros_r = micros(); reattach_interrupts(); return;           
          }
        }
        if (downTime > hold_time) //REACHED TIME LIMIT (HELD)
        {
          right_btn(2); hold_r = 1; last_micros_r = micros(); reattach_interrupts(); return;
        }            
      }
    }
    else  //NOT STILL PRESSED
    {
      right_btn(1); last_micros_r = micros(); reattach_interrupts(); return;             
    }
  }
  else
  {
    last_micros_r = micros(); reattach_interrupts(); return;
  }
}

void ISRSUB_F() {
  detach_interrupts(); //TEMPORARILY DISABLE INTERRUPTS
  if ((long)(micros() - last_micros_f) >= (debounce*1000)) 
  { 
    digitalRead(PRESS_F);
    click_f = 1; last_micros_f = micros(); reattach_interrupts(); return; //SINGLE CLICKED
  }
  else
  {
    last_micros_f = micros(); reattach_interrupts(); return;
  }
}

void ISRSUB_L() {
  detach_interrupts(); //TEMPORARILY DISABLE INTERRUPT 
  if ((long)(micros() - last_micros_l) >= (debounce*1000)) 
  { 
    digitalRead(PRESS_L);
    click_l = 1; last_micros_l = micros(); reattach_interrupts(); return;
  }
  else
  {
    last_micros_l = micros(); reattach_interrupts(); return;
  }
}

void ISRSUB_R() {
  detach_interrupts(); //TEMPORARILY DISABLE INTERRUPTS
  if ((long)(micros() - last_micros_r) >= (debounce*1000)) 
  { 
    digitalRead(PRESS_R);
    click_r = 1; last_micros_r = micros(); reattach_interrupts(); return;
  }
  else
  {
    last_micros_r = micros(); reattach_interrupts(); return;
  }
}



















