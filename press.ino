void init_press() {
  pinMode(PRESS_F, INPUT_PULLUP); pinMode(PRESS_L, INPUT_PULLUP); pinMode(PRESS_R, INPUT_PULLUP);
  digitalWrite(PRESS_F, HIGH); digitalWrite(PRESS_L, HIGH); digitalWrite(PRESS_R, HIGH); 
}

void front_btn(uint8_t type) {
  if (type == 0) { //PRESS OR HOLD
    leds_front(32, 124, 17, 12); // leds_front(24, 255, 214, 24); 
    digitalRead(PRESS_F);
  }
  
  if (type == 1) { // PRESS
    if (POW == 0) {
      power_on();
      return;
    }
    else if (PANE != 1) {
      if (PANE == 6) {
        if ((BLE == 1) || (SPP == 1)) {
          BC127.print(F("SEND ERS^\n\r"));
        }
      }
      if (messageOn == true) {
        messageOn = false;
        if (BLE == 1) {
          BC127.print(F("SEND SMS:0^\n\r"));
        }
      }
      msg_flag = 0;
      home_pane();
      PANE = 1;
    }
    else if (eventPress != true) {
      if (alarm != -1) {
        eventPress = true; 
        return;
      }
    }
    else if (call == 1) {
      call = 2;
      BC127.print(F("ANSWER\r"));
    }
    else if (call >= 2) {
      call = 0; 
      BC127.print(F("END\r"));
    }
    else if (voice == 1) {
      if ((music == 1) && (artist.length() >= 2))
      {
        clear_stream(); print_track();
      }
      else if ((STR == 1) && (STR_ICON != 1) && ((social_count+personal_count) > 0) && ((BLE == 1) || (SPP == 1)))
      {
        if (PANE == 1) 
        {
          clear_stream(); streams_banner();
        }
      }
      voice = 0;
      BC127.print(F("TOGGLE_VR\r"));
    }
    else if ((msg_flag == 1) && (STR == 1)) {
      msg_flag = 0; leds_off();
      if (personal_count >= 2)
      {
        personal_counter = 1;
        if (PANE == 5) personal_next();
      }
      else
      {
        personal_counter = -1;
        if (PANE != 1) home_pane();
      }
    }

    /*
    else if ((HUE != 999) || (MSA == 1))
    {
      if (MSA == 1)
      {
        music_sync_stop();
        MSL = 0;
        if (BLE == 1)
        {
          BC127.print(F("SEND MSL:0^\n\r")); BC127.flush();
        }
        
        if (music == 1)
        {
          BC127.print(F("MUSIC PAUSE\r"));
          music = 2; if (MST == 1) clear_stream();
          if ((STR == 1) && (social_count+personal_count > 0) && ((BLE == 1) || (SPP == 1)))
          {
            streams_banner();
          }
        }
      }
      
      FastLED.clear(); COL[0] = 0; COL[1] = 0; COL[2] = 0; HUE = 999;
      if ((BLE == 1) || (SPP == 1))
      {
        BC127.print(F("SEND HUE:999^\n\r"));
      }
    }
    */
    else if (music == 1) {
      leds_off();
      BC127.print(F("MUSIC PAUSE\r"));
      BC127.flush();
      music = 2; 
      if (MST == 1) clear_stream();
      if (MSA == 1) music_sync_stop();
      if (ANA == 1) animationStop();
      if (STR == 1) 
      {
        if ((social_count+personal_count > 0) && ((BLE == 1) || (SPP == 1)))
        {
          streams_banner();
        }
      }
    }
    else if (music == 2) {
      BC127.print(F("MUSIC PLAY\r"));
      BC127.flush();
      music = 1;
      if (MST == 1) print_track();
      if ((MSL == 1) && (MSA != 1)) {
        //leds_off();
        //music_sync_stop();
        //MSL = 1;
        music_sync_start();
        return;
      }
    }
    else if ((music == 0) && (BTC == 1)) {
      BC127.print(F("MUSIC PLAY\r"));
      BC127.flush();
    }
    else if (MSA != 0) {
      music_sync_stop();
    }
    else if (ANA != 0) {
      animationStop();
    }
    else {
      leds_off();
    }
    
    return;
  }

  /*
  else if (type == 3) { //DOUBLE
    if ((NGT != 1) && (LED != 0) && (LSW != 0)) leds_front(32, 124, 17, 12); // leds_front(24, 255, 214, 24); 
    digitalRead(PRESS_F);
    
    if (music == 1) {
      if (MSL == 0) {
        MSL = 1;
        music_sync_start(); 
        return;
      }
      else {
        MSL = 0;
        music_sync_stop();
        FastLED.clear(); FastLED.show();
        return; 
      }
    }
    
    
    if (PANE == 1)
    {
      if (STR == 1)
      {
        if (((PRC == 0) || ((PRC == 1) && ((BLE == 1) || (SPP == 1))) || ((PRC == 2) && (CHQ == 1))))
        {
          if (social_count >= 1)
          {
            social_pane();
          }
          else if (personal_count >= 1)
          {
            personal_pane();
          }
        }
      }
    }
    else {
      home_pane();
    }
    
    leds_off();
    if ((HUE != 999) && ((BLE == 1) || (SPP == 1))) {
      HUE = 999;
      BC127.print(F("SEND COL:0:0:0^\n\r"));
    }
  }
  */
  
  else if (type == 2) { //HOLD
    leds_front(157, 25, 41, 255);
    
    if (POW == 0) {
      power_on();
      return;
    }
    
    if (BTC == 1) {
      if (MSA == 1) {
        music_sync_stop();
        MSL = 0;
      }
      
      BC127.print(F("TOGGLE_VR\r"));
      if (voice == 1) voice = 0;
      else if (voice == 0) voice = 1;
      
      if (PANE == 1) {
        print_info(F("* voice assist *")); 
        info_flag = true;
        ROTATE.reset();
      }
      
    }
    else {
      if (BLE != 1) BC127.print(F("ADVERTISING ON\r"));  
    }
    
    leds_off();
  }
}

void left_btn(uint8_t type) {  
  if (type == 0) { // PRESS OR HOLD
    leds_left(32, 124, 17, 6); // leds_left(24, 255, 214, 24); 
    digitalRead(PRESS_L);
  }
  
  if (type == 1) { //PRESS
    if (PANE == 4) {
      social_back();
    }
    else if (PANE == 5) {
      personal_back();
    }
    else if (PANE == 6) {      
      //if ((BLE == 1) || (SPP == 1)) {
        //sketch_pane();
        //BC127.print(F("XY^\n\r"));
        //return;
        //BC127.print(F("SEND ERS^\n\r"));
      //}
      home_pane();
    }
    else { //if (BTC == 1)
      if (call == 2) {
        clear_stream(); 
        BC127.print(F("REJECT\r"));
        BC127.flush();
        call = 0;
      }
      else if (call == 1) { 
        clear_stream();
        BC127.print(F("END\r"));
        BC127.flush();
        call = 0;
      }
      else {
        BC127.print(F("VOLUME DOWN\r"));
        BC127.flush();
      }
    }
    leds_off();
  }
  else if (type == 2) { // HOLD
    leds_left(157, 25, 41, 255);
    
    if ((PANE == 4) && (personal_count > 0)) {
      personal_pane();
    }
    else if ((PANE == 5) && (social_count > 0)) {
      social_pane();
    }
    else if ((PANE == 4) || (PANE == 5)) {
      home_pane();
    }
    else if (music == 1) {
      BC127.print(F("MUSIC BACKWARD\r"));
      BC127.flush();
    }
    
    leds_off();
    if ((HUE != 999) && ((BLE == 1) || (SPP == 1)))
    {
      HUE = 999;
      BC127.print(F("SEND HUE:999^\n\r"));
    }    
  }
}

void right_btn(uint8_t type)
{
  if (type == 0) { //PRESS OR HOLD
    leds_right(32, 124, 17, 6); // leds_right(24, 255, 214, 24); 
    digitalRead(PRESS_R);
  }
  
  if (type == 1) { //PRESS
    if (PANE == 4) {
      social_next();
    }
    else if (PANE == 5) {
      personal_next();
    }
    else if (PANE == 6) {
      home_pane();
      if ((BLE == 1) || (SPP == 1)) {
        BC127.print(F("SEND ERS^\n\r"));
      }
    }
    else { //if (BTC == 1)
      if (call == 2) {
        clear_stream();
        BC127.print(F("ANSWER\r"));
        call = 1;
      }
      else {
        BC127.print(F("VOLUME UP\r"));
        BC127.flush();
      }
    }
    leds_off();
  }
  else if (type == 2) { // HOLD
    leds_right(157, 25, 41, 255);
    if ((PANE == 4) && (personal_count > 0)) {
      personal_pane();
    }
    else if ((PANE == 5) && (social_count > 0)) {
      social_pane();
    }
    else if ((PANE == 4) || (PANE == 5)) {
      home_pane();
    }
    else if (music == 1) {
      BC127.print(F("MUSIC FORWARD\r"));
      BC127.flush();
    }
    
    leds_off();
  }
}

















