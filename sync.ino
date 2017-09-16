void synchronize() {  
  //GUI REFRESHES VARIABLES THAT STORE THE GLOWDECK HARDWARE STATE AND SYNCS THE HEADER ICONS ACCORDINGLY
  if (GUI.check() == 1) {
    if (PANE == 1) update_clock();
    GUI.reset();
  }
  
  if (HARDWARE.check() == 1) {
    if (PANE == 1) {
      if (DBA == 1) update_bright();
    }
    HARDWARE.reset();
  }
  
  if (ROTATE.check() == 1) {
    
    if (info_flag == true) {
      monthsLast = -1; 
      foreLast = F("0"); 
      info_flag = false;
      if (PANE == 1) update_clock(); 
    }
    
    eventHandler();
    
    if ((ver_flag == 0) && (BLE == 1)) {
      BC127.print("SEND VER:0." + String(VER) + "^\n\r"); 
      BC127.flush();
      ver_flag = 1;
      ROTATE.reset();
      return;
    }
    
    if ((time_flag != 1) && (BLE == 1)) {
      BC127.print(F("SEND TIM^\n\r")); BC127.flush();
      time_flag = 1;
      ROTATE.reset();
      return;
    }
   
    bat_flag++;
    if ((bat_flag % 10 == 0) && ((BLE == 1) || (SPP == 1)) && ((CGS == 1) || (CHQ == 1))) {
      if (BLE == 1) {
        BC127.print(F("SEND BAT^\n\r")); 
        BC127.flush();
        ROTATE.reset();
        return;
      }
      else if (SPP == 1)
      {
        BC127.print(F("SEND BA^\n\r")); 
        BC127.flush();
        ROTATE.reset();
        return;
      }
      
    }
    
    wtr_flag++;
    if ((wtr_flag % 25 == 0) && ((BLE == 1) || (SPP == 1))) {
      BC127.print(F("SEND WTR^\n\r")); 
      BC127.flush();
      
      ROTATE.reset();
      return;
    }
    
    
    if ((STR == 1) && (music == 1)) {
      banner_rotate++; if (banner_rotate > 2) banner_rotate = 0;
      
      if (banner_rotate == 2) {
        if (ip_flag == 1)
        {
          if ((BLE == 1) || (SPP == 1))
          {
            BC127.print(F("SEND WIC:OK^\n\r"));
          }
          //clear_stream();
          //print_stream(F("Web"), F("http://") + String(sta_ip));
          sta_flag = 1;
          ip_flag = 2;
          ROTATE.reset();
          return;
        }
        else if (ip_flag == -5)
        {
          if ((BLE == 1) || (SPP == 1))
          {
            BC127.print(F("SEND WIC:OK^\n\r")); //SHOULD BE ERR
          }
          //clear_stream();
          //print_stream(F("Web"), F("http://10.10.100.254"));
          sta_flag = 1;
          ip_flag = -6;
          ROTATE.reset();
          return;
        }
        else if (sta_flag == 1)
        {
          //clear_stream();
          if (prv_flag == 1)
          {
            send_init(); prv_flag = 0;
          }
          sta_flag = 0;
        }
        else
        {
          banner_rotate = 0;
        }
      }
      
      if (banner_rotate == 0)
      {
        print_track();
      }
      else
      {
        if (PANE == 1) 
        {
          if ((personal_count+social_count) > 0)
          {
            streams_banner();
          }
        }
        else if (PANE == 4)
        {
          hours = hour(); minutes = minute(); 
          if (minutes != minutesLast)
          {
            String addZ = F(""); String ampm;
            if ((hours >= 0) && (hours <= 11))
            {
              ampm = F("AM");
            }
            else
            {
              ampm = F("PM");
            }
            if (minutes < 10) addZ = F("0");
            minutesLast = minutes; 
            social_banner(hrConvert(hours) + F(":") + addZ + minutes + F(" ") + ampm + F(" | ") + forecast + F(" | ") + temperature);
          }
        }
        
        
        else if (PANE == 5)
        {
          hours = hour(); minutes = minute(); 
          if (minutes != minutesLast)
          {
            String addZ = F(""); String ampm;
            if ((hours >= 0) && (hours <= 11))
            {
              ampm = F("AM");
            }
            else
            {
              ampm = F("PM");
            }
            if (minutes < 10) 
            {
              addZ = F("0");
            }
            minutesLast = minutes; 
            personal_banner(hrConvert(hours) + F(":") + addZ + minutes + F(" ") + ampm + F(" | ") + forecast + F(" | ") + temperature);
          }
        }
      }
    }
    else {
      
      /*
      if (ip_flag == 1)
      {
        if ((BLE == 1) || (SPP == 1))
        {
          BC127.print(F("SEND WIC:OK^\n\r"));
        }
        //clear_stream();
        //print_stream(F("Web"), F("http://") + String(sta_ip));
        ip_flag = 2;
        //sta_flag = 1;
        ROTATE.reset();
        return;
      }
      else if (ip_flag == -5)
      {
        if ((BLE == 1) || (SPP == 1))
        {
          //BC127.print(F("SEND WIC:ERR^\n\r"));
          BC127.print(F("SEND WIC:OK^\n\r"));
        }
        //clear_stream();
        //print_stream(F("Web"), F("http://10.10.100.254"));
        ip_flag = -6;
        //sta_flag = 1;
        ROTATE.reset();
        return;
      }
      
      */
      
      //***SKETCH PANE***
      if (PANE == 6)
      {
        /*
        hours = hour(); minutes = minute(); 
        if (minutes != minutesLast)
        {
          String addZ = F(""); String ampm;
          if ((hours >= 0) && (hours <= 11))
          {
            ampm = F("AM");
          }
          else
          {
            ampm = F("PM");
          }
          if (minutes < 10) 
          {
            addZ = F("0");
          }
          minutesLast = minutes;
          personal_banner(hrConvert(hours) + F(":") + addZ + minutes + " " + ampm + " | " + forecast + " | " + temperature);
         
        }
        */
      }
      
      //***CALL POPUP PANE***
      else if (PANE == 7)
      {
        hours = hour(); minutes = minute(); 
        if (minutes != minutesLast)
        {
          String addZ = F(""); String ampm;
          if ((hours >= 0) && (hours <= 11))
          {
            ampm = F("AM");
          }
          else
          {
            ampm = F("PM");
          }
          if (minutes < 10) 
          {
            addZ = F("0");
          }
          minutesLast = minutes;
          personal_banner(hrConvert(hours) + F(":") + addZ + minutes + F(" ") + ampm + F(" | ") + forecast + F(" | ") + temperature);
        }        
      }

      //***MESSAGE POPUP PANE***
      else if (PANE == 8)
      {
        hours = hour(); minutes = minute(); 
        if (minutes != minutesLast)
        {
          String addZ = F(""); String ampm;
          if ((hours >= 0) && (hours <= 11))
          {
            ampm = F("AM");
          }
          else
          {
            ampm = F("PM");
          }
          if (minutes < 10) 
          {
            addZ = F("0");
          }
          minutesLast = minutes;
          personal_banner(hrConvert(hours) + F(":") + addZ + minutes + F(" ") + ampm + F(" | ") + forecast + F(" | ") + temperature);
        }        
      }
      
      // CUSTOM POPUP PANE
      else if (PANE == 9) {
        hours = hour(); minutes = minute(); 
        if (minutes != minutesLast) {
          String addZ = F(""); 
          String ampm;
          if ((hours >= 0) && (hours <= 11)) {
            ampm = F("AM");
          }
          else {
            ampm = F("PM");
          }
          if (minutes < 10) {
            addZ = F("0");
          }
          minutesLast = minutes;
          personal_banner(hrConvert(hours) + F(":") + addZ + minutes + F(" ") + ampm + F(" | ") + forecast + F(" | ") + temperature);
        }        
      }      
      
      
      /*
      if (sta_flag == 1)
      {
        //clear_stream();
        if (prv_flag == 1)
        {
          send_init(); prv_flag = 0;
        }
        sta_flag = 0;
      }
      */
      if (music == 1) {
        if (music_rotate != -1) {
          print_track();
        }
      }
      else if (STR == 1) {
        if (((personal_count+social_count)>0) && (PANE == 1) && ((BLE == 1) || (SPP == 1)) && (STR_ICON != 1) && (PANE == 1)) {
          streams_banner();
        }
      }
    }
    ROTATE.reset();
  }
  
  //SAFE CHECKS LED STATE AND TURNS OFF LED ARRAY LEFT ON AND IDLE FOR TOO LONG
  if (SAFE.check() == 1) {
    if (leds_cutoff == 0) {
      if (((COL[0] != 0) || (COL[1] != 0) || (COL[2] != 0)) || (HUE != 999) || (ANM != -1) || (MSL != 0)) {
        leds_cutoff = 1; 
        leds_start = millis();
      }
    }
    else if (leds_cutoff == 1) {
      if (((COL[0] == 0) && (COL[1] == 0) && (COL[2] == 0)) && (ANM == -1) && (MSL == 0) && (HUE == 999)) {
        leds_cutoff = 0; 
        leds_start = 0;
      }
      else {
        if ((millis() - leds_start) > 3600000) {   // AUTO TURN OFF LEDS IF THEY HAVE BEEN ON AN IDLE FOR > 1 HOUR
          leds_off();
          leds_cutoff = 0;
          leds_start = 0;
        }
      }
    }
    SAFE.reset();
  }
  
  //LINK1 MONITORS THE STATUS OF THE WIFI MODULE
  /*
  if (LINK1.check() == 1)
  {   
    if (WIF != 0)
    {
      uint8_t WIO_LAST = WIO; wifi_status();
      if ( ((WIO == 1) && (WIO_LAST == 0)) && ((ip_flag != 2) && (ip_flag != -6)) )
      {
        if ( ((sta_ip.length() <= 8) || (sta_ip == F("10.10.100.254"))) && (ip_flag > -5) )
        {
          ip_flag--;
          USR.print(F("AT+WANN\r")); usr_handler(usr_response(0));
          if (sta_ip.length() > 8)
          {
            ip_flag = 1;
            USR.print(F("AT+WSSSID\r")); String rsp = usr_shorten(usr_response(0));
            if (rsp.length() > 2)
            {
              WID = rsp;
              USR.print(F("AT+GDSSID=")); USR.print(WID); USR.print(F("\r")); usr_response(0);
            }
          }
        }
      }
      WIO_LAST = WIO;  
    }
    
    LINK1.reset();
  }
  */
  
  // LINK2 MONITORS THE STATUS OF THE BLUETOOTH MODULE
  if (LINK2.check() == 1) {
    status_flag++;
    if (status_flag % 10 == 0) {
      status_flag = 0;
      bc127_status();
      
      if (BTC == 0) {
        if ((BLE == 0) && (SPP == 0)) {
          BC127.print("ADVERTISING ON\r");
          BTQ = F("DISCOVERABLE ON");
        }
  
        if (((btr_flag == 1) && (attempts_flag < 2)) && ((BLE == 1) || (SPP == 1)))
        {
          BC127.print(F("OPEN ") + BTR + F(" HFP\r")); BC127.flush();
          BTQ = F("OPEN ") + BTR + F(" MAP"); 
          attempts_flag++;
        }
      }
      else {
        if (MAP != 1) {
          if ((btr_flag == 1) && (remap_flag <= 3)) { 
            BC127.print(F("OPEN ") + BTR + F(" MAP\r"));
            BC127.flush();
            remap_flag++;
          }
        }
        else {
          if (BLE != 1) {
            BC127.print(F("ADVERTISING ON\r")); 
            BC127.flush();
          }
          // BTQ = F("STATUS");
          BC127.print(F("STATUS\r")); 
        }
      }
      
      if ((BLE == 1) || (SPP == 1)) {
        update_charge();
      }
    }
    LINK2.reset();
  }
  
  //STREAMS UPDATES STREAMS CONTENT
  if (STREAMS.check() == 1) {
    //if ((CGS == 1) && (CHQ == 1))
    if (CGS == 1) {
      if ((BLE == 1) || ((SPP == 1) && (STR != 1))) {
        BC127.print(F("SEND BAT^\n\r")); 
        BC127.flush();
        if (CGS == 1) smart_charge();
      }
    }
    
    if ((BTC == 0) && (btr_flag == 1)) {
      BTQ = F("OPEN ") + BTR + F(" HFP");
    }
    else if ((BTC == 1) && (btr_flag == 1) && (MAP != 1) && (attempts_flag <= 4)) {
      BTQ = F("OPEN ") + BTR + F(" MAP"); attempts_flag++;
    }
    else if ((BTC == 1) && (MAP == 1)) {
      BC127.print(F("SEND MAP:1^\n\r")); BC127.flush();
    }
    
    /*
    if (((BLE == 0) && (SPP == 0)) && ((bleAddr.length() == 12) && (bleAddr != F("000000000000")))) {
      BC127.print(F("OPEN ") + bleAddr + F(" BLE\r")); BC127.flush(); return;
    }
    */
    if (BLE == 1) {
      ver_flag = (ver_flag + 1) % 10;
    }
    else {
      ver_flag = 0;
    }
    
    if ((BLE == 1) && (ver_flag == 0)) {
      String transmit = F("SEND VER:0.") + String(VER) + F("^\n\r");
      BC127.print(transmit); 
      BC127.flush(); //return;
    }
    else {
      if (PANE == 1) {
        if ((BLE == 1) || (SPP == 1)) {

          if (forecast.length() < 2) {
            wtr_flag++;
            
            if (wtr_flag % 25 == 0) {
              // weather_timer = millis();
              BC127.print(F("SEND WTR^\n\r")); 
              BC127.flush();
            }
          }
        } 
      }
    }
    
    
    if (STR == 1)  {
      //WE GET STREAM CONTENT VIA BLUETOOTH AS FIRST PREFERENCE
      if ((BLE == 1) || (SPP == 1))
      {
        if ((social_count <= 1) || (millis() - social_timer > 240000))
        {
          social_timer = millis();
          //STREAMS.reset();
          BC127.print(F("SEND SOC^\n\r")); BC127.flush();
          //return;
        }
        else if ((personal_count <= 1) || (millis() - personal_timer > 300000))
        {
          personal_timer = millis();
          //STREAMS.reset();
          BC127.print(F("SEND PER^\n\r")); BC127.flush();
          //return;
        }
        else 
        {
          if (PANE == 1)
          {
            if ((personal_count+social_count) > 0)
            {
              if ((music != 1) && (STR_ICON != 1))
              {
                streams_banner();
              }
            }
          }
          else if (PANE == 4)
          {
            if ((social_counter < social_count) && (social_count > 0))
            {
              social_next();
            }
            else if ((social_counter >= social_count) && (social_count > 0))
            {
              if (personal_count > 0)
              {
                social_counter = -1; 
                personal_counter = -1;
                personal_pane();
                //STREAMS.reset();
                social_timer = millis(); 
                personal_timer = millis();
                BC127.print(F("SEND SOC^\n\r"));
                return;
              }
              else
              {
                social_counter = -1; 
                personal_counter = -1;
                home_pane();
                //STREAMS.reset();
                personal_timer = millis();
                BC127.print(F("SEND PER^\n\r"));
                return;
              }
            }
            else
            {
              social_counter = -1;
              home_pane();
              //STREAMS.reset();
              weather_timer = millis(); 
              social_timer = millis(); 
              personal_timer = millis();
              BC127.print(F("SEND WTR^\n\r"));
              return;
            }
          }
          else if (PANE == 5)
          {
            if ((personal_counter < personal_count) && (personal_count > 0))
            {
              personal_next(); return;
            }
            else if ((personal_counter >= personal_count) && (personal_count > 0))
            {
              if (social_count > 0)
              {
                social_counter = -1; personal_counter = -1;
                social_pane();
                //STREAMS.reset();
                social_timer = millis();
                personal_timer = millis();
                BC127.print(F("SEND PER^\n\r"));
                return;
              }
              else
              {
                social_counter = -1; personal_counter = -1;
                home_pane();
                //STREAMS.reset();
                social_timer = millis(); 
                personal_timer = millis();
                BC127.print(F("SEND SOC^\n\r"));
                return;
              }
            }
            else
            {
              personal_counter = -1;
              home_pane();
              //STREAMS.reset();
              weather_timer = millis(); 
              social_timer = millis(); 
              personal_timer = millis();
              BC127.print(F("SEND WTR^\n\r"));
              return;
            }
          }
        }
      }
      //WE USE WIFI AS SECOND RESORT
      /*
      else if (WIO == 1)
      {
        
        //if (REG == 0)
        //{
        //  get_stream(F("v"));
        //}

        if (REG == 1)
        {
          if (PANE == 1)
          {
            if (forecast == F("0"))
            {
              get_stream(F("w")); return;
            }
            else
            {
              weatherWait++;
              if (weatherWait > 10)
              {
                get_stream(F("w")); weatherWait = 0; return;
              }
            }
            
            if (social_count == 0)
            {
              get_stream(F("nc"));
              if (social_count >= 1)
              {
                if (PANE == 1)
                {
                  streams_banner(social_count);
                }
              }
            }
            else if (social_count >= 1)
            {
              if (social[1][3].length() <= 2)
              {
                get_stream(F("n"));
              }
            }
          }
          else if (PANE == 4)
          {
            if (social_count > social_counter)
            {
              news_rotate++; if (news_rotate > 2) news_rotate = 0;
              if (news_rotate == 2) social_next();
            }
          }
        }
      }
      */
      //NO CONNECTIVITY
      else {
        if (PANE != 1) {
          home_pane();
        }
        else {
          if (STR_ICON == 1) {
            clear_stream(); 
            STR_ICON = 0;
          }
        }
        personal_counter = -1; 
        social_counter = -1;
        personal_count = 0; 
        social_count = 0;
        //STREAMS.reset();
        weather_timer = millis(); 
        social_timer = millis(); 
        personal_timer = millis();
        //wtr_flag++;
        //if (wtr_flag == 50) {
          //wtr_flag = 0;
        BC127.print(F("SEND WTR^\n\r")); 
        BC127.flush();
        //}
        //return;
      }
      STREAMS.reset();
    }
  }
}

void eventHandler() {
  /*
  //time_t t = Teensy3Clock.get();
  hours = hour(); minutes = minute();
  
  String ampm = F("PM");
  if (hours < 12)
  {
    ampm = F("AM");
  }

  String nowTime = hrConvert(hours); 
  nowTime += F(":"); 
  if (minutes < 10) 
  {
    nowTime += F("0");
  }
  nowTime += minutes;
  nowTime += F(" "); 
  nowTime += ampm;
  
  for (int i = 1; i < 10; i++)
  {
    //ONLY CHECK TIME BASED ALARMS
    
    //if ((i >= 1) && (i <= 3))
    //{
    //  //NON TIME BASED ALARMS
    //  continue;
   // }
    
    if (EVENT[i][0] == "1")
    {
      if ((EVENT[i][1].indexOf(nowTime) != -1) && (alarm != i))
      {
        EVENT[i][0] = "2";
 
        if ((BLE == 1) || (SPP == 1))
        {
          BC127.print(F("SEND EVT:") + String(i) + F("^\n\r")); BC127.flush(); delay(5);
        }
        eventPerform(i);
        return;
      }
    }
  }
  */
}

void eventPerform(int id) {
  /*
  alarm = id; 
  eventPress = false;
  int eventHue = EVENT[id][2].toInt();
  int eventAudio = EVENT[id][3].toInt();
 
  if (PANE != 1)
  {
    home_pane();
  }
 
  if (music == 1)
  {
    BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(8);
    BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(8);
    BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(8);
    BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(8);
    BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(8);
    BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(8);
    BC127.print(F("MUSIC PAUSE\r")); BC127.flush(); delay(25); bc127_response();
    music = 2;
    if (MSA >= 1) 
    {
      music_sync_stop();
      MSA = 0;
      MSL = 0;
      if (BLE == 1)
      {
        BC127.print(F("SEND MSL:0^\n\r")); BC127.flush();
      }
    }
  }
  clear_stream();
  print_info(F("* event alarm *")); print_stream(F("press front to dismiss"), F(""));
  
  if (DBR < 10)
  {
    int start_dbr = analog_convert(DBR); 
    int end_dbr = analog_convert(10);
    
    for (int e = start_dbr; e < 256; e++)
    {
      analogWrite(LCD_BL, e); delay(15);
    }
    
    DBR = 10;
  }
 
  FastLED.clear(); FastLED.setBrightness(0);
  
  if (eventHue != 999)
  {
    for (int o = 0; o < NUM_LEDS; o++)
    {
      leds[o].setHue(eventHue);
    }
    FastLED.show();
    
    for (int z = 0; z < 4; z++)
    {
      for (int d = 0; d < 256; d++)
      {
        FastLED.setBrightness(d);
        FastLED.show();
        FastLED.delay(2);
        if (eventDismissCheck() == 1)
        {
          FastLED.clear(); 
          eventPress = false;
          return;
        }
      }
      FastLED.delay(120);
      for (int d = 255; d > -1; d--)
      {
        FastLED.setBrightness(d);
        FastLED.show();
        FastLED.delay(2);
        if (eventDismissCheck() == 1)
        {
          FastLED.clear(); 
          eventPress = false;
          return;
        }
      }
      FastLED.delay(625);
      if (eventDismissCheck() == 1)
      {
        FastLED.clear(); 
        eventPress = false;
        return;
      }
    }
  }
  if (eventAudio > 0)
  {
    if (BTC == 1)
    {
      SND = 1; digitalWrite(AMP, HIGH);
      BC127.print(F("MUSIC BACKWARD\r")); BC127.flush(); bc127_response();
      //if (eventAudio == 1)
      //{
      //  for (int p = 0; p < 15; p++)
      //  {
      //    BC127.print(F("VOLUME DOWN\r")); BC127.flush(); delay(25);
      //  }
      //}
      BC127.print(F("MUSIC PLAY\r")); BC127.flush(); bc127_response(); delay(10);
      BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(20);
      BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(10);
      BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(10);
      BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(10);
      BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(10);
      BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(10);
      BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(10);
      BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(10);
      BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(10);
      BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(10);
      BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(10);
      BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(10);
      BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(10);
      BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(5);
      BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(5);
      BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(5);
      BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(5);
      BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(5);
      BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(5);
      BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(5);
      BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(5);
      BC127.print(F("VOLUME DOWN\r")); BC127.flush(); bc127_response(); delay(50);
      BC127.print(F("VOLUME UP\r")); BC127.flush(); bc127_response();
    }
    else
    {
      if ((BLE == 1) && (eventAudio != 0))
      {
        BC127.print(F("SEND PLAY^\n\r")); BC127.flush(); bc127_response();
        //BC127.print(F("SEND VOLUP^\n\r")); BC127.flush(); delay(25);
      }
    }
  }
  
  FastLED.clear(); FastLED.setBrightness(255); VIB = 255;
  int hue_tmp = HUE; int millisStart = millis();
  int vol_iterations = 0; int vol_ups = 0; int routine = 0;
  int led_delay = 28;
  
  do 
  {
    if (routine == 0)
    {
      confetti();
    }
    else if (routine == 1)
    {
      bpm();
    }
    else if (routine == 2)
    {
      sinelon();
    }
    else if (routine == 3)
    {
      juggle();
    }
    
    FastLED.show(); FastLED.delay(led_delay); 
    vol_iterations++;
    HUE = (HUE+1) % 256;
    
    if (eventDismissCheck() == 1)
    {
      HUE = hue_tmp;
      eventPress = false;
      return;
    }
    
    if (vol_iterations >= 350)
    {
      if (eventAudio > 0)
      {
        if (BTC == 1)
        {
          if (eventAudio == 1)
          {
            if (vol_ups < 15)
            {
              BC127.print(F("VOLUME UP\r")); BC127.flush();
            }
          }
        }
        else
        {
          if ((BLE == 1) && (eventAudio != 0))
          {
            BC127.print(F("SEND PLAY^\n\r")); BC127.flush();
            //BC127.print(F("SEND VOLUP^\n\r")); BC127.flush(); delay(25);
          }
        }
      }
      
      if (random(1, 5) == 2)
      {
        update_clock();
      }
      
      if (vol_ups < 15)
      {
        led_delay--;
      }
      
      vol_iterations = 0; vol_ups++; 
      routine = (routine + 1) % 4;
    }
    
  } while (millis() - millisStart < 360000);
  
  if (eventAudio > 0)
  {
    if (BTC == 1)
    {
      for (int p = 0; p < 6; p++)
      {
        BC127.print(F("VOLUME DOWN\r")); BC127.flush(); delay(100);
      }
      BC127.print(F("MUSIC PAUSE\r")); BC127.flush();
      BC127.print(F("VOLUME UP\r")); BC127.flush(); delay(25); 
      digitalWrite(AMP, LOW); SND = 0; 
    }
    else
    {
      if ((BLE == 1) && (eventAudio != 0))
      {
        BC127.print(F("SEND PAUSE^\n\r")); BC127.flush(); delay(15);
      } 
    }
  }
  clear_info(); clear_stream(); print_info(F("* alarm missed *"));
  fadeout_leds(); FastLED.clear(); vol_ups = 0; vol_iterations = 0; 
  HUE = hue_tmp;
  
  if (eventDismissCheck() == 1)
  {
    FastLED.clear(); 
    eventPress = false;
    bc127_response();
    return;
  }

  EVENT[alarm][0] = F("0");
  eventPress = false; 
  FastLED.clear();
  FastLED.setBrightness(255);
  VIB = 255;
  monthsLast = -1; foreLast = F("0"); minutesLast = -1;
  clear_info(); print_info(F("* alarm dismissed *")); ROTATE.reset();
  clear_stream(); 
  if ((BLE == 1) || (SPP == 1))
  {
    BC127.print(F("SEND EVT:") + String(alarm) + F("^\n\r")); 
    BC127.flush(); delay(50);
  }
  
  if (BTC == 1)
  {
    if (eventAudio == 1)
    {
      for (int p = 0; p < 6; p++)
      {
        BC127.print(F("VOLUME DOWN\r")); BC127.flush(); delay(100);
      }
    }
    BC127.print(F("MUSIC PAUSE\r")); BC127.flush(); delay(25);
    BC127.print(F("VOLUME UP\r")); BC127.flush(); delay(25);
  }
  else
  {
    if ((BLE == 1) && (eventAudio != 0))
    {
      BC127.print(F("SEND PAUSE^\n\r")); BC127.flush(); 
    }
  }
  alarm = -1;
  update_clock();
  return;
  
  */
}

int eventDismissCheck() {
  /*
  if (eventPress == true) 
  {
    EVENT[alarm][0] = F("0");
    eventPress = false; 
    FastLED.clear();
    FastLED.setBrightness(255);
    VIB = 255;
    monthsLast = -1; foreLast = F("0"); minutesLast = -1;
    clear_info(); print_info(F("* alarm dismissed *")); ROTATE.reset();
    clear_stream(); 
    if ((BLE == 1) || (SPP == 1))
    {
      BC127.print(F("SEND EVT:") + String(alarm) + F("^\n\r"));
      BC127.flush(); delay(50);
    }

    if (BTC == 1)
    {
      for (int p = 0; p < 6; p++)
      {
        BC127.print(F("VOLUME DOWN\r")); BC127.flush(); delay(100);
      }
      BC127.print(F("MUSIC PAUSE\r")); BC127.flush(); delay(25);
      BC127.print(F("VOLUME UP\r")); BC127.flush(); delay(25); 
      SND = 0; digitalWrite(AMP, LOW);
      
    }
    else
    {
      if (BLE == 1)
      {
        BC127.print(F("SEND PAUSE^\n\r")); BC127.flush();
      }
    }
    alarm = -1;
    update_clock();
    return 1;
  }
  */
  return 0;
}



















