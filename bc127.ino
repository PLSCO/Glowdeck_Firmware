void init_bcpins() {  
  pinMode(PIO0, OUTPUT); pinMode(PIO1, OUTPUT); pinMode(PIO2, OUTPUT); pinMode(PIO4, OUTPUT); pinMode(PIO5, OUTPUT);
}  

void init_bc127() {
  if (LNM.indexOf(F("BlueCreation")) != -1) {
    force_factory(); 
    init_settings();
  }
}

void bc127_handler(String cmd) { 
  cmd.trim();
  
  // if (DEBUG == 1) MCU.println(cmd);

  MCU.println(cmd);
  
  if ((cmd == F("OK")) || (cmd == F("ERROR")) || (cmd == F(""))) {
    return;
  }
  
  if (cmd.endsWith(F(" S"))) {
    if (cmd.indexOf(F("ROLE_OK")) != -1) {
      if (cmd.length() >= 19) {
        uint8_t a1 = cmd.indexOf(F(" "));
        uint8_t a2 = cmd.indexOf(F(" "), a1+1);
        BTR = cmd.substring(a1+1, a2);
        if ((BTR.length() == 12) && (BTR != F("000000000000"))) {
          btr_flag = 1;
          //BTQ = F("OPEN ") + BTR + F(" MAP\r");
          BC127.print(F("OPEN ") + BTR + F(" MAP\r"));
          BC127.flush();
        }
      }
    }
  }
  
  if (cmd.startsWith(F("STATE "))) {
    bt_state(cmd); 
    return;
  }
  else if ((cmd.startsWith(F("LINK "))) && (cmd.indexOf(F("CONNECTED")) != -1)) {
    bt_link(cmd);
    return;
  }
  else if ((cmd.indexOf(F("_OK")) != -1) || (cmd.indexOf(F("_ERROR")) != -1)) {
    bt_change(cmd);
    return;
  }
  
  // MARK: - Glowdeck BLE Commands
  if (cmd.startsWith(F("RECV "))) {     // COMMAND FROM BLUETOOTH DEVICE (BLE OR SPP)
    if (cmd.indexOf(F(" SPP ")) != -1) {
      if (nextGen) {
        BT_PREFIX = 8;
      }
      else {
        BT_PREFIX = 14;
      }
      SPP = 1;
    }  
    
    if (cmd.indexOf(F("HUE:")) != -1) { // COLOR SELECT - CLOCKWISE SPIN
      int dif = 0;
      if (NUM_LEDS > 56) dif = 55;
      // int col_r; int col_g; int col_b;
      int ind_a = cmd.indexOf(F(":")); 
      int ind_b = cmd.indexOf(F("^"));
      HUE = (cmd.substring(ind_a+1, ind_b)).toInt();
      if ((NGT == 1) || (LSW == 0) || (LED == 0) || (MSA == 1)) return; 
      if (HUE == 999) {
        fadeout_leds();
        COL[0] = 0; 
        COL[1] = 0; 
        COL[2] = 0; 
        HUE_FROM = 999; 
        HUE = 999;
        return;
      }
      
      if ((HUE_FROM != 899) || (SPIN_CLOCKWISE == false)) {
        FastLED.clear();
        COL[0] = 0; 
        COL[1] = 0; 
        COL[2] = 0;
        HUE_FROM = 899; 
        SPIN_CLOCKWISE = true;
      }
      
      //CLEAR FIRST 3 LEDS FROM LAST SPIN
      if (LID == 2+dif) {
        leds[1+dif] = CRGB(0,0,0);
        leds[0+dif] = CRGB(0,0,0);
        leds[NUM_LEDS-1] = CRGB(0,0,0);
      }
      else if (LID == 1+dif) {
        leds[0+dif] = CRGB(0,0,0);
        leds[NUM_LEDS-1] = CRGB(0,0,0);
        leds[NUM_LEDS-2] = CRGB(0,0,0);
      }
      else if (LID == 0+dif) {
        leds[NUM_LEDS-1] = CRGB(0,0,0);
        leds[NUM_LEDS-2] = CRGB(0,0,0);
        leds[NUM_LEDS-3] = CRGB(0,0,0);
      }
      else {
        leds[LID-3] = CRGB(0,0,0);
        leds[LID-2] = CRGB(0,0,0);
        leds[LID-1] = CRGB(0,0,0);
      }
      FastLED.show();
       
      int ledId = LID;
      for (int i = 0; i < 5; i++) {
        leds[ledId].setHue(HUE);
        
        ledId++;
        if (ledId > (NUM_LEDS-1)) {
          ledId = (ledId-(NUM_LEDS-1+dif))-1;
        }
      }    
      FastLED.show();
      
      LID++; LID++; LID++;
      if (LID > (NUM_LEDS-1)) {
        LID = (LID-(NUM_LEDS-1+dif))-1;
      }
      
      return;
    }
    else if (cmd.indexOf(F("HUX:")) != -1) { // COLOR SELECT - COUNTERCLOCKWISE SPIN
      int dif = 0;
      if (NUM_LEDS > 56) {
        dif = 55;
      }
      
      // int col_r; int col_g; int col_b;
      int ind_a = cmd.indexOf(F(":")); int ind_b = cmd.indexOf(F("^"));
      HUE = (cmd.substring(ind_a+1, ind_b)).toInt();
      
      if ((NGT == 1) || (LSW == 0) || (LED == 0) || (MSA == 1)) {
        return; 
      }
      
      if (HUE == 999)
      {
        fadeout_leds();
        COL[0] = 0; COL[1] = 0; COL[2] = 0; HUE_FROM = 999; HUE = 999;
        return;
      }
      if ((HUE_FROM != 899) || (SPIN_CLOCKWISE == true))
      {
        FastLED.clear();
        COL[0] = 0; COL[1] = 0; COL[2] = 0;
        HUE_FROM = 899; SPIN_CLOCKWISE = false;
      }

      //CLEAR FIRST 3 LEDS FROM LAST SPIN
      if (LID == (NUM_LEDS-5+dif))
      {
        leds[NUM_LEDS-1+dif] = CRGB(0,0,0);
        leds[NUM_LEDS-2+dif] = CRGB(0,0,0);
        leds[NUM_LEDS-3+dif] = CRGB(0,0,0);        
      }
      else if (LID == (NUM_LEDS-4+dif))
      {
        leds[0+dif] = CRGB(0,0,0);
        leds[NUM_LEDS-1+dif] = CRGB(0,0,0);
        leds[NUM_LEDS-2+dif] = CRGB(0,0,0);
      }
      else if (LID == (NUM_LEDS-3+dif))
      {
        leds[1+dif] = CRGB(0,0,0);
        leds[0+dif] = CRGB(0,0,0);
        leds[NUM_LEDS-1+dif] = CRGB(0,0,0);
      }
      else if (LID == (NUM_LEDS-2+dif))
      {
        leds[2+dif] = CRGB(0,0,0);
        leds[1+dif] = CRGB(0,0,0);
        leds[0+dif] = CRGB(0,0,0);
      }
      else if (LID == (NUM_LEDS-1+dif))
      {
        leds[3+dif] = CRGB(0,0,0);
        leds[2+dif] = CRGB(0,0,0);
        leds[1+dif] = CRGB(0,0,0);
      }
      else
      {
        FastLED.clear();
      } 
      FastLED.show();
      
      int ledId = LID;
      for (int i = 0; i < 5; i++)
      {
        leds[ledId].setHue(HUE);
        
        ledId--;
        
        if (ledId < 0)
        {
          ledId = ((NUM_LEDS-1+dif)+ledId)+1;
          if (ledId > (NUM_LEDS-1+dif))
          {
            ledId = 0; LID = 0;
          }
        }
      }    
      FastLED.show();

      LID--; LID--; LID--;
      if (LID < 0)
      {
        LID = ((NUM_LEDS-1+dif)+LID)+1;
        if (LID > (NUM_LEDS-1+dif))
        {
          LID = 0;
        }
      }
      return;
    }
    else if (cmd.indexOf(F("CLR:")) != -1) { // COLOR WHEEL LANDED
      int dif = 0;
      if (NUM_LEDS > 56) {
        dif = 55;
      }
      
      int ind_a = cmd.indexOf(F(":")); 
      int ind_b = cmd.indexOf(F("^"));
      HUE = (cmd.substring(ind_a+1, ind_b)).toInt();
      
      if ((NGT == 1) || (LSW == 0) || (LED == 0) || (MSA == 1) || (ANM != -1)) return; 
      
      
      //HERE WE EITHER FADE THE LEDS OR GO HOME PANE
      if (HUE == 999) {
        if (PANE != 1)
        {
          msg_flag = 0;
          fadeout_leds();
          COL[0] = 0; COL[1] = 0; COL[2] = 0;
          HUE_FROM = 999;          
          home_pane();
          PANE = 1;
          return;
        }
        else {
          fadeout_leds();
          COL[0] = 0; COL[1] = 0; COL[2] = 0;
          HUE_FROM = 999;
          return;
        }
      }
      
      int d = LID; int e = LID; int p;
      int w = ((NUM_LEDS-dif)/2)+1;
      
      for (p = 0; p < w; p++)
      {
        d++; e--;
        if (d > (NUM_LEDS-1)-dif)
        {
          d = (d-(NUM_LEDS-1)-dif)-1;
        }
        if (e < 0)
        {
          e = ((NUM_LEDS-1)+e-dif)+1;
        }
        leds[d].setHue(HUE);
        leds[e].setHue(HUE);
        FastLED.show();
        FastLED.delay(10);
      }
      int half = ((NUM_LEDS-dif)/2) + dif;
      COL[0] = leds[half].r; COL[1] = leds[half].g; COL[2] = leds[half].b;
      HUE_FROM = HUE;
      return;        
    }
    else if (cmd.indexOf(F("VER^")) != -1) { // FIRMWARE VERISON GET COMMAND
      BC127.print(F("SEND VER:0.") + String(VER) + F("^\n\r")); 
      BC127.flush();
      return;
    }
    else if (cmd.indexOf(F("LIT:")) != -1) { // SET INDIVIDUAL LED COLOR
      // Example: LIT:33:255:255:255:255:1^
      // Result:  Sets LED 33 to WHITE at FULL BRIGHTNESS with TONE
      
      int ind_a = cmd.indexOf(F(":"));
      int ind_b = cmd.indexOf(F(":"), ind_a+1);
      int ind_c = cmd.indexOf(F(":"), ind_b+1);
      int ind_d = cmd.indexOf(F(":"), ind_c+1);
      int ind_e = cmd.indexOf(F(":"), ind_d+1);
      int ind_f = cmd.indexOf(F(":"), ind_e+1);
      int ind_g = cmd.indexOf(F("^"));

      if ((ind_a == -1) || (ind_b == -1) || (ind_c == -1) || (ind_d == -1) || (ind_e == -1) || (ind_f == -1) || (ind_g == -1)) return; 

      int id = (cmd.substring(ind_a+1, ind_b)).toInt(); 
      int red = (cmd.substring(ind_b+1, ind_c)).toInt(); 
      int green = (cmd.substring(ind_c+1, ind_d)).toInt(); 
      int blue = (cmd.substring(ind_d+1, ind_e)).toInt();
      int alpha = (cmd.substring(ind_e+1, ind_f)).toInt();
      int sound = (cmd.substring(ind_f+1, ind_g)).toInt();

      //if (alpha != 0) {
        if (LED == 0) LED = 1;
        if (LSW == 0) LSW = 1;
      //}
      
      fadepad_led(id, red, green, blue, alpha, sound);
      return;
    }
    else if (cmd.indexOf(F("LIT^")) != -1) { // GET ALL LED COLORS
      // Example: LIT:1:255:255:255^
      // Example: LIT:2:0:0:0^
      // Example: LIT:3:24:15:136^
      // etc.

      // Transmits all 56 LED RGB values, one at a time
      for (int i = 1; i <= NUM_LEDS; i++) {
        String tx = F("SEND LIT:");
        tx += String(i); // led id
        tx += F(":");
        tx += String(leds[i-1].r); // red
        tx += F(":");
        tx += String(leds[i-1].g); // green
        tx += F(":");
        tx += String(leds[i-1].b); // blue
        
        tx += F("^\n\r"); // terminator

        BC127.print(tx);
        BC127.flush();
        bc127_response();
        delay(50);
      }

      if ((BC127.checkListOverflow()) || (BC127.checkCharOverflow())) BC127.resetCommandList();
      return;
    }
    else if (cmd.indexOf(F("ERS^")) != -1) { // ERASE SKETCH
      if (PANE == 6) {
        home_pane();
      }
      return;
    }
    else if (cmd.indexOf(F("DBR:")) != -1) { // SET DISPLAY BRIGHTNESS
      int val = (cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")))).toInt();
      if ((val >= 0) && (val <= 10)) DBR = val;
      else DBR = 10;
      analogWrite(LCD_BL, analog_convert(DBR)); 
      if (DBA == 1) {
        DBA = 0;
        if ((BLE == 1) || (SPP == 1)) {
          BC127.print(F("SEND DBA:0^\n\r"));
        }
      }
      return;     
    }
    else if (cmd.indexOf(F("DL:")) != -1) { // SET DISPLAY BRIGHTNESS
      int val = (cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")))).toInt();
      if ((val >= 0) && (val <= 10)) DBR = val;
      else DBR = 10;
      analogWrite(LCD_BL, analog_convert(DBR)); 
      if (DBA == 1) {
        DBA = 0;
        if ((BLE == 1) || (SPP == 1)) {
          BC127.print(F("SEND DBA:0^\n\r"));
        }
      }
      return;     
    }
    else if (cmd.indexOf(F("TIM:")) != -1) { // TIME SYNC STRING RECEIVED
      // Ex: RECV BLE 2015-01-31 03:04:55
      int len = cmd.length(); int yr_begin = cmd.indexOf(F(":"));
      String andr_time = cmd.substring(yr_begin, len);
      len = andr_time.length();
      int t_yr, t_mon, t_day, t_hr, t_min, t_sec; int h1, h2, s1, c1, c2;
      int p1 = andr_time.indexOf(F(":")); h1 = andr_time.indexOf(F("-")); h2 = andr_time.indexOf(F("-"), h1+1); s1 = andr_time.indexOf(F(" ")); c1 = andr_time.indexOf(F(":"), s1+1); c2 = andr_time.indexOf(F(":"), c1+1); 
      t_yr = (andr_time.substring(p1+1, h1)).toInt(); t_mon = (andr_time.substring(h1+1, h2)).toInt(); t_day = (andr_time.substring(h2+1, s1)).toInt(); 
      t_hr = (andr_time.substring(s1+1, c1)).toInt(); t_min = (andr_time.substring(c1+1, c2)).toInt(); t_sec = (andr_time.substring(c2+1, len)).toInt();
      Teensy3Clock.compensate(rtcCompensate);
      setTime(t_hr,t_min,t_sec,t_day,t_mon,t_yr);
      Teensy3Clock.set(now()); 
      monthsLast = -1; 
      update_clock(); 
      return;
    }
    else if (cmd.indexOf(F("WTR:")) != -1) { // WEATHER STREAM RECEIVED
      int a1; 
      int a2; 
      int a3; 
      int a4;
      a1 = cmd.indexOf(F(":")); 
      a2 = cmd.indexOf(F("|")); 
      a3 = cmd.indexOf(F("|"), a2+1); 
      a4 = cmd.indexOf(F("^"));
      String tempTmp = cmd.substring(a1+1, a2) + F("º");
      String foreTmp = cmd.substring(a2+1, a3);
      String locTmp = cmd.substring(a3+1, a4);
      
      if ((tempTmp.indexOf(F("RECV")) != -1) || (foreTmp.indexOf(F("RECV")) != -1) || (locTmp.indexOf(F("RECV")) != -1)) return;
 
      temperature = tempTmp;
      forecast = foreTmp;
      location = locTmp;
 
      foreLast = F("-1"); 
      monthsLast = -1;
      
      if (PANE == 1) update_clock();
      
      weather_timer = millis();
      
      //if (STR == 1) {
      //  BC127.print(F("SEND SOC^\n\r")); BC127.flush();
      //}
      return;
    }
    else if (cmd.indexOf(F("GFU^")) != -1) { // FIRMWARE UPDATE MODE
      print_info(F("* please be patient *"));
      print_stream(F("enter blue-light mode"), F(""));
      if (EEPROM.read(199) != '1') EEPROM.write(199, '1');
      if (EEPROM.read(198) != '1') EEPROM.write(198, '1');
      //if (cmd.indexOf(F("SPP")) != -1) {
        //if (EEPROM.read(197) != '1') EEPROM.write(197, '1');
        //BC127.print(F("SET ENABLE_RAW_DATA=ON\r"));
        //BC127.flush();
        // if (DEBUG == 1) MCU.println(F("ENTER DATA MODE TRIAL..."));
      //}
      int c = analog_convert(DBR);
      for (int i = c; i > -1; i--) {
        analogWrite(LCD_BL, i); 
        delay(3);
      }
      dfu_mode();
      return;
    }
    else if (cmd.indexOf(F("SND:")) != -1) { // CHANGE CLASSIC CONNECTION
      if (cmd.indexOf(F("0^")) != -1) {
        if ((SND == 1) || (digitalRead(AMP) != LOW)) {
          SND = 0; 
          digitalWrite(AMP, LOW);
        }
        return;
      }
      else {
        if ((btr_flag == 1) && (BTC == 0)) {
          BC127.print(F("OPEN ") + BTR + F(" HFP\r")); 
          BC127.flush();
          return;
        }
      } 
    }
    else if (cmd.indexOf(F("BTC:")) != -1) { // BLUETOOTH CLASSIC CONNECTION HANDLER
      if (cmd.indexOf(F("0^")) != -1) { // DISCONNECT BTC
        if (BTC != 0) {
          clear_info(); 
          print_info(F("* disconnect audio *")); 
          info_flag = true;
          BC127.print(F("CLOSE 2\r")); 
          BC127.flush(); 
          delay(25);
          BC127.print(F("CLOSE 8\r")); 
          BC127.flush(); 
          delay(25);
          return;
        }
      }
      else if (cmd.indexOf(F("2^")) != -1) { // MAKE DISCOVERABLE
        if ((btr_flag == 1) && (BTC != 1)) {
          clear_info(); 
          print_info(F("* connecting audio *")); 
          info_flag = true;
          BC127.print(F("OPEN ") + BTR + F(" HFP\r")); 
          BC127.flush();
        }
        else {
          if (BTC != 1) {
            clear_info(); 
            print_info(F("* audio connectable *")); 
            info_flag = true;
            BC127.print(F("DISCOVERABLE ON\r"));
          }
        }
        ROTATE.reset();
        return;
      }
      else if (cmd.indexOf(F("3^")) != -1) { // FORGET PAIRED DEVICES
        clear_info(); 
        print_info(F("* unpairing audio *")); 
        info_flag = true;
        BC127.print(F("UNPAIR\r")); 
        BC127.flush(); 
        delay(300); 
        bc127_response();
        BC127.print("SET BLE_AUTO_ADV=ON\r"); 
        BC127.flush(); 
        delay(300); 
        bc127_response();
        BC127.print("SET DISCOVERABLE=2 0\r");
        BC127.flush(); 
        delay(300); 
        bc127_response();
        BC127.print(F("WRITE\r")); 
        BC127.flush();
        delay(300); 
        bc127_response();
        //BC127.print(F("INQUIRY 3\r"); BC127.flush();
        //BC127.print(F("RESET\r")); BC127.flush(); delay(300);
        
        //CLEAR BTR
        BTR = F(""); 
        btr_flag = 0;
        for (int s = 20; s < 32; s++) {
          char btr_char = EEPROM.read(s);
          delay(3);
          BTR += String(btr_char);
        }
        
        if (BTR != F("000000000000")) {
          for (int s = 20; s < 32; s++) {
            EEPROM.write(s, (char)'0');
            BTR += "0";
            delay(6);
          }
          btr_flag = 0;
        }
        
        btr_flag = 0; 
        BTC = 0;
        BC127.print("DISCOVERABLE ON\r"); 
        BC127.flush(); 
        bc127_response(); 
        delay(500);
        print_info(F("* restart to pair *")); 
        info_flag = true;        
        //BTQ = F("INQUIRY 5");
        BC127.print(F("INQUIRY 4\r")); 
        BC127.flush();
        return;     
      }
    }
    else if (cmd.indexOf(F("EVT^")) != -1) { // TRANSMIT EVENT SETTINGS
      return;
    }
    else if (cmd.indexOf(F("EVT.")) != -1) { // RECEIVE EVENT SETTINGS
      return;
    }
    else if (cmd.indexOf(F("VIB:")) != -1) { // ADJUST LED BRIGHTNESS
      uint8_t val = (cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")))).toInt();
      if (val != VIB) {
        adjustBrightness(val);
        VIB = val;
      }
      return; 
    }
    else if (cmd.indexOf(F("VIB^")) != -1) { // GET LED BRIGHTNESS
      String tx = F("SEND VIB:");
      tx += String(VIB); // brightness value
      tx += F("^\n\r"); // terminator

      BC127.print(tx);
      BC127.flush();
      bc127_response();
      return;
    }
    else if (cmd.indexOf(F("CHG^")) != -1) { // GET CHARGER STATE
      String response = F("SEND CHG:") + String(CHG) + F("^\n\r");
      BC127.print(response); return;
    }
    else if ((cmd.indexOf(F("CHG:")) != -1) || (cmd.indexOf(F("CS:")) != -1)) { // CHARGER ON/OFF
      uint8_t val = (cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")))).toInt();
      if ((val == 1) && (CHG != val)) {
        print_info(F("* coils on *")); info_flag = true; ROTATE.reset();
        digitalWrite(COIL_PW, HIGH);
        CHG = 1; return;
      }
      else {
        if (CHG != val) {
          print_info(F("* coils off *")); info_flag = true; ROTATE.reset();
          digitalWrite(COIL_PW, LOW);
          CHG = 0; return;
        }
      }
      return;
    }
    else if (cmd.indexOf(F("MAP:2^")) != -1) { // REATTEMPT MAP CONNECTION
      if ((BTR.length() > 10) && (BTR != F("000000000000")) && (btr_flag == 1) && (MAP != 1)) {
        BC127.print(F("OPEN ") + BTR + F(" MAP\r"));
        BC127.flush();
      }
      else if (MAP == 1) {
        if (BLE == 1) {
          BC127.print(F("SEND MAP:1^\n\r"));
        }
      }
      return;
    }
    else if (cmd.indexOf(F("POW:")) != -1) { // TOGGLE POWER
      if (cmd.indexOf(F("1^")) != -1) {
        if (POW == 0) {
          power_on(); 
          POW = 1; 
          return;
        }
        POW = 1; 
        return;
      }
      else {
        if (POW == 1) {
          power_off(); 
          POW = 0; 
          return;
        } 
        POW = 0; 
        return;
      }
      return;
    }
    else if (cmd.indexOf(F("CGS^")) != -1) { // GET CHARGER STATE
      String response = F("SEND CGS:") + String(CGS) + F("^\n\r");
      BC127.print(response); BC127.flush();
      return;
    }
    else if ((cmd.indexOf(F("CGS:")) != -1) || (cmd.indexOf(F("CS:")) != -1)) { // SMART CHARGE ON/OFF
      uint8_t val = (cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")))).toInt();
      if ((val == 1) && (CGS != val))
      {
        print_info(F("* smart charge on *")); info_flag = true; ROTATE.reset();
        BTQ = F("SEND BAT^");
        CGS = 1;
      }
      else
      {
        if (CGS != val)
        {
          print_info(F("* smart charge off *")); info_flag = true; ROTATE.reset();
          CGS = 0;
          if (CHG == 0)
          {
            digitalWrite(COIL_PW, HIGH); 
            CHG = 1;
          }
        }
      }
      return;
    }
    else if (cmd.indexOf(F("LSW^")) != -1) { // GET LIGHT SYSTEM ON/OFF STATE
      String response = "SEND LSW:" + String(LSW) + "^\n\r";
      BC127.print(response); return;
    }
    else if (cmd.indexOf(F("LSW:")) != -1) { // LEDS ON/OFF
      uint8_t val = (cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")))).toInt();
      if ((val == 0) && (LSW != val))
      {
        leds_off(); leds_icon(-2); leds_icon(-1); LED = 0; LSW = 0; //HUE = 999;
        //COL[0] = 0; COL[1] = 0; COL[2] = 0;
        
        print_info(F("* lights disabled *")); info_flag = true; ROTATE.reset();
        
        //BC127.print(F("SEND LSW:0^\n\r"));
      }
      else
      {
        if (LSW == 0)
        {
          //leds_icon(-2); leds_icon(0);
          print_info(F("* lights enabled *")); info_flag = true; ROTATE.reset();
          if (HUE != 999)
          {
            //FastLED.showColor(CRGB(COL[0], COL[1], COL[2])); leds_icon(1);
            for (int i = 0; i < NUM_LEDS; i++)
            {
              leds[i].setHue(HUE);
            }
            HUE = COL[0];
          }
          else {
            HUE = 999; 
          }
        }
        LED = 1; LSW = 1;
      }
      return;
    }
    else if (cmd.indexOf(F("DBA^")) != -1) { // GET AUTO-BRIGHTNESS STATE
      String response = F("SEND DBA:") + String(DBA) + F("^\n\r");
      BC127.print(response); return;
    }
    else if ((cmd.indexOf(F("DBA:")) != -1) || (cmd.indexOf(F("LS:")) != -1)) { // AUTO-BRIGHTNESS ON/OFF
      uint8_t val = (cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")))).toInt();
      if ((val == 1) && (DBA != val)) {
        DBA = 1;
        print_info(F("* auto-bright on *")); info_flag = true; ROTATE.reset();
      }
      else {
        if (DBA != val) {
          DBA = 0;
          print_info(F("* auto-bright off *")); info_flag = true; ROTATE.reset();
        }
      }
      update_bright();
      return;
    }
    else if (cmd.indexOf(F("MSL^")) != -1) { // GET MUSIC SYNC STATE
      String response = F("SEND MSL:") + String(MSL) + F("^\n\r");
      BC127.print(response); return;
    }
    else if ((cmd.indexOf(F("MSL:")) != -1) || (cmd.indexOf(F("MS:")) != -1)) { // MUSIC SYNC ON/OFF
      if (cmd.indexOf(F("1")) != -1) {
        if ((ANM != -1) || (ANA != 0)) animationStop();
        
        MSL = 1;
        if (BTC == 1) {
          print_info(F("* music sync on *"));
          info_flag = true;
          ROTATE.reset();
        }
        if (music == 1) music_sync_start();
      }
      else {
        MSL = 0;
        music_sync_stop();
        if (BTC == 1) {
          print_info(F("* music sync off *")); 
          info_flag = true;
          ROTATE.reset();
        }
        leds_off();
      }
      return;
    }
    else if (cmd.indexOf(F("ANM:")) != -1) { // LED ANIMATION ON/OFF
      if (cmd.indexOf(F("-1")) != -1) {
        if ((ANM != -1) || (ANA != 0)) {
          animationStop();
          print_info(F("* animation off *")); 
          info_flag = true; 
          ROTATE.reset();
        }
        ANM = -1; 
        ANA = 0;
        return;
      }
      else {
        uint8_t type = -1; 
        String anmType = F("");

        if (cmd.indexOf(F("1")) != -1) { type = 1; anmType = F("glitter"); }
        else if (cmd.indexOf(F("2")) != -1) { type = 2; anmType = F("confetti"); }
        else if (cmd.indexOf(F("3")) != -1) { type = 3; anmType = F("sentry"); }
        else if (cmd.indexOf(F("4")) != -1) { type = 4; anmType = F("showcase"); }
        else if (cmd.indexOf(F("5")) != -1) { type = 5; anmType = F("party"); }
        else if (cmd.indexOf(F("6")) != -1) { type = 6; anmType = F("twinkle"); }
        else if (cmd.indexOf(F("0")) != -1) { type = 0; anmType = F("swirl"); }
        else return;

        if ((type > -1) && (type < 7)) {
          // if ((ANM == -1) || ((ANM != type) && (info_flag != true))) {
          if (ANM != type) {
            if (MSL == 1) music_sync_stop();
            print_info(F("* ") + anmType + F(" *"));
            info_flag = true;
            ROTATE.reset();
          }
          animationStart(type);
        }
      }
      return;
    }
    else if (cmd.indexOf(F("ANM^")) != -1) { // GET LIGHT SHOW STATE 
      String response = F("SEND ANM:") + String(ANM) + F("^\n\r");
      BC127.print(response);
      BC127.flush();
      return;
    }
    else if (cmd.indexOf(F("MST^")) != -1) { // GET DISPLAY SONG INFO STATE
      String response = F("SEND MST:") + String(MST) + F("^\n\r");
      BC127.print(response); 
      return;
    }
    else if ((cmd.indexOf(F("MST:")) != -1) || (cmd.indexOf(F("SM:")) != -1)) { // DISPLAY SONG INFO ON/OFF
      uint8_t val = (cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")))).toInt();
      if (val == 1) MST = 1;
      else MST = 0;
      return;
    }
    else if (cmd.indexOf(F("STR:")) != -1) { // STREAMS ENABLED TOGGLE
      //int str_on = (cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")))).toInt();
      //USER TOGGLED STREAMS ON
      if (cmd.indexOf("1") != -1) {
        if (STR == 0)
        {
          if (PANE != 1)
          {
            home_pane();
          }
          // print_info("* streams syncing *"); info_flag = true;
        }
        /*
        if (music == 1)
        {
          clear_stream();
        }
        */
        //pxs.drawCompressedBitmap(11, 200, streams_icon);
        //String tmp = F("Streams syncing...");
        //pxs.setColor(255,255,255); pxs.setFont(Avenir18a); pxs.print(47, 202, tmp);
        
        STR = 1; str_flag = 1;
        
        if (MSL == 1) {
          if (MSA == 1) {
            music_sync_stop();
          }
          MSA = 0;
          MSL = 0;
        }
        if (ANM != -1) {
          animationStop();
        }
        
        while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
        BC127.print(F("SEND SOC^\n\r")); BC127.flush();
        
        return;
      }
      //USER TOGGLED STREAMS OFF
      else {
      
        if (STR == 1)
        {
          //reset_social();
          //reset_personal();
          personal_count = 0; personal_counter = -1;
          social_count = 0; social_counter = -1;
          //temperature = F(""); location = F(""); forecast = F(""); monthsLast = -1;
          if (PANE != 1)
          {
            home_pane();
            PANE = 1;
          }
          else
          {
            if (STR_ICON == 1)
            {
              pxs.setColor(0,0,0); pxs.fillRectangle(0,201,320,39); pxs.setColor(255,255,255); STR_ICON = 0;
            }
          }
        }
        STR = 0;
      }
      return;
    }   
    else if (cmd.indexOf(F("NOT:")) != -1) { // STREAMS NOTIFICATION RECEIVED
      NOT = 1;
      notificationHandler(cmd);
      BC127.print(F("SEND NOT:1^\n\r")); 
      BC127.flush();
      return;
    }
    else if (cmd.indexOf(F("NTX^")) != -1) { // CLOSE NOTIFICATION
      if (NOT == 1) {
        if (PANE != 1) {
          leds_off();
          home_pane();
          PANE = 1;
        }
      }
      NOT = 0;
      return;
    }
    else if (cmd.indexOf(F("EQZ^")) != -1) { // GET EQUALIZER SETTING
      String response = F("SEND EQZ:") + String(EQZ) + F("^\n\r");
      BC127.print(response); 
      return;
    }
    else if (cmd.indexOf(F("EQZ:")) != -1) { // EQUALIZER
      uint8_t val = (cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")))).toInt();
      if ((val >= 0) && (val <= 4)) EQZ = val;
      else EQZ = 3;
      BC127.print(F("SET MM=1 1 ") + String(EQZ) + F("\r"));
      print_info(F("* eq updated *")); 
      info_flag = true; ROTATE.reset();
      return;
    } 
    else if (cmd.indexOf(F("EQL:")) != -1) { // EQUALIZER CHANGE
      uint8_t val = EQZ;
      
      if (cmd.indexOf(F("-1")) != -1) val = val - 1;
      else val = val + 1;
      
      if (val > 4) val = 0;
      else if (val < 0) val = 4;

      EQZ = val;
      BC127.print(F("SET MM=1 1 ") + String(EQZ) + F("\r"));
      print_info(F("* ") + equalizers[EQZ] + F(" eq *")); 
      info_flag = true;
      ROTATE.reset();
      return;
    } 
    else if (cmd.indexOf(F("PRC^")) != -1) { // GET PRIVACY MODE STATE
      String response = F("SEND PRC:") + String(PRC) + F("^\n\r");
      BC127.print(response); 
      return;
    }
    else if (cmd.indexOf(F("PRC:")) != -1) { // PRIVACY MODE
      uint8_t val = (cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")))).toInt();
      if ((val >= 0) && (val <= 2)) {
        PRC = val;
      }
      else {
        PRC = 0;
      }
      return;
    }
    else if (cmd.indexOf(F("SMS:")) != -1) { // SMS TOGGLE
      if (messageOn == true) {
        if (PANE != 1) {
          leds_off();
          home_pane();
          PANE = 1;
        }
        else {
          BC127.print(F("SEND SMS:0^\n\r"));
          BC127.flush();
        }
      }
      messageOn = false;
      return;
    }
    else if (cmd.indexOf(F("MOB^")) != -1) { // GET MOBILE NOTIFICATIONS STATE
      String response = F("SEND MOB:") + String(MOB) + F("^\n\r");
      BC127.print(response); 
      return;
    }
    else if (cmd.indexOf(F("MOB:")) != -1) { // MOBILE NOTIFICATIONS ON/OFF
      uint8_t val = (cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")))).toInt();
      if (val == 0) {
        MOB = val;
      }
      else {
        MOB = 1;
      }
      return;
    } 
    else if (cmd.indexOf(F("WIF^")) != -1) { // GET WIFI STATE
      String tx = "SEND WIF:";
      tx += String(WIF);
      tx += "^\n\r";
      
      BC127.print(tx); 
      BC127.flush();
      return;
    }
    else if (cmd.indexOf(F("WIF:")) != -1) { // WIFI ON/OFF
      uint8_t val = (cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")))).toInt();
      if (val == 0) {
        WIF = val;
      }
      else {
        // WIF = 1;
        WIF = 0;
        BC127.print(F("SEND WIF:0^\n\r"));
      }
      return;
    }
    else if (cmd.indexOf(F("BAT:")) != -1) { // PHONE CHARGE LEVEL
      int chg_val = ((cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")))).trim()).toInt();
      if (chg_val > BAT) {
        BUP = 1;
      }
      else {
        BUP = 0;
      }
      BAT = chg_val;
      charge_icon(1);
      if (BAT > 99) {
        if (CGS == 1) {
          if (CHG != 0) {
            CHG = 0; 
            digitalWrite(COIL_PW, LOW);
            BC127.print(F("SEND CHG:0^\n\r"));
          }
        }
      }
      else {
        if ((CHG == 0) && (CGS == 1)) {
          charge_icon(1);
          CHG = 1; 
          digitalWrite(COIL_PW, HIGH);
          BC127.print(F("SEND CHG:1^\n\r"));
        }
      }
      return;
    }    
    else if (cmd.indexOf(F("XY:")) != -1) { // DRAW LINE COMMAND
      uint8_t a, b, c, d, e;
      uint16_t x1, y1, x2, y2;
      a = cmd.indexOf(F(":")); 
      b = cmd.indexOf(F(":"), a+1); 
      c = cmd.indexOf(F(":"), b+1); 
      d = cmd.indexOf(F(":"), c+1);
      e = cmd.indexOf(F("^"));
      x1 = (cmd.substring(a+1, b)).toInt(); 
      y1 = (cmd.substring(b+1, c)).toInt();
      x2 = (cmd.substring(c+1, d)).toInt(); 
      y2 = (cmd.substring(d+1, e)).toInt();
      // pxs.setColor(CLR[0], CLR[1], CLR[2]);
      pxs.drawLine(x1, y1, x2, y2);
      return;
    }
    else if (cmd.indexOf(F("XYBEGIN^")) != -1) { // LCD DRAW COMMAND
      if (MSA == 1) {
        MSA = 0; 
        music_sync_stop();
      }
      if (ANM != -1) {
        animationStop();
      }
      sketch_pane();
      return;
    }    
    else if (cmd.indexOf(F("XYEND^")) != -1) { // SKETCH COMPLETE COMMAND
      sketch_flag = 0;
      FastLED.clear(); 
      int millisStart = millis();
      
      if ((MSL == 1) && (music == 1) && (MSA != 1)) {
        music_sync_start(); MSA = 1;
      }
      else {
        do {
          confetti(); 
          FastLED.delay(30);
        } while ((millis() - millisStart) < 5000);
        fadeout_leds();
      }
      return;
    }
    else if (cmd.indexOf(F("RGB:")) != -1) { // NEW PAINTBRUSH COLOR
      int ind_a = cmd.indexOf(F(":")); 
      int ind_b = cmd.indexOf(F(":"), ind_a+1); 
      int ind_c = cmd.indexOf(F(":"), ind_b+1); 
      int ind_d = cmd.indexOf(F("^"));
      
      CLR[0] = (cmd.substring(ind_a+1, ind_b)).toInt(); 
      CLR[1] = (cmd.substring(ind_b+1, ind_c)).toInt(); 
      CLR[2] = (cmd.substring(ind_c+1, ind_d)).toInt();
      pxs.setColor(CLR[0], CLR[1], CLR[2]);
      
      return;
    }
    else if (cmd.indexOf(F("HSV:")) != -1) { // NEW PAINTBRUSH COLOR
      int ind_a = cmd.indexOf(F(":")); 
      int ind_b = cmd.indexOf(F(":"), ind_a+1); 
      int ind_c = cmd.indexOf(F(":"), ind_b+1); 
      int ind_d = cmd.indexOf(F("^"));
      CLR[0] = (cmd.substring(ind_a+1, ind_b)).toInt(); 
      CLR[1] = (cmd.substring(ind_b+1, ind_c)).toInt(); 
      CLR[2] = (cmd.substring(ind_c+1, ind_d)).toInt();
      pxs.setColor(CLR[0], CLR[1], CLR[2]);
      return;
    }
    else if (cmd.indexOf(F("SOC:||")) != -1) { // SOCIAL STREAM RECEIVED
      //String social[15][6]; //0=TYPE; 1=A; 2=B; 3=C; 4=D; 5=STATE
      /*
      social_count = 0;
      while (1) {
        int n1; 
        int n2;
        n1 = cmd.indexOf(F(":||"));
        if ((n1 == -1) || (social_count > 14)) {
          cmd = F(""); 
          if (social_count > 0) {
            streams_banner();
          }
          break;   
        }
        
        n2 = cmd.indexOf(F(":||"), n1+1);
        if (n2 == -1) n2 = cmd.indexOf(F("^"));
        
        int a1 = cmd.indexOf(F("|"), n1+3); 
        int a2 = cmd.indexOf(F("|"), a1+1);
        int a3 = cmd.indexOf(F("|"), a2+1); 
        int a4 = cmd.indexOf(F("|"), a3+1);
        
        if ((cmd.substring(n1+3, a1) == F("N")) || (cmd.substring(n1+3, a1) == F("T"))) {
          social[social_count][0] = cmd.substring(n1+3, a1);
          social[social_count][1] = cmd.substring(a1+1, a2);
          social[social_count][2] = cmd.substring(a2+1, a3);
          social[social_count][3] = cmd.substring(a3+1, a4);
          if (social[social_count][3].indexOf(F(" http://")) != -1) {
            social[social_count][3] = social[social_count][3].substring(0, social[social_count][3].indexOf(F(" http://")));
          }
          else if (social[social_count][3].indexOf(F(" https://")) != -1) {
            social[social_count][3] = social[social_count][3].substring(0, social[social_count][3].indexOf(F(" https://")));
          }
          social[social_count][4] = cmd.substring(a4+1, n2-2);
          social[social_count][5] = F("2"); // CHANGE TO ACTUAL STATE
          social_count++;
        }
        cmd = cmd.substring(n2, cmd.length());
      }
      
      //if (social_timer == 0)
      //{
        //MCU.println(F("***SOCIAL STREAMS***"));
        //int r;
        //for (r = 0; r < social_count; r++) {
        //  MCU.print(F("Type (")); MCU.print(r); MCU.print(F("): ")); MCU.println(social[r][0]);
        //  MCU.print(F("A (")); MCU.print(r); MCU.print(F("): ")); MCU.println(social[r][1]);
        //  MCU.print(F("B (")); MCU.print(r); MCU.print(F("): ")); MCU.println(social[r][2]);
        //  MCU.print(F("C (")); MCU.print(r); MCU.print(F("): ")); MCU.println(social[r][3]);
        //  MCU.print(F("D (")); MCU.print(r); MCU.print(F("): ")); MCU.println(social[r][4]);
        //  MCU.println();
        //}
        
        //while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
        //delay(1000);
        //BC127.resetCommandList();
        social_timer = millis();
        BC127.print(F("SEND PER^\n\r")); 
        BC127.flush();
      //}
      */
      
      return;
      
    }
    else if (cmd.indexOf(F("PER:||")) != -1) { // PERSONAL STREAM RECEIVED
      /*
      // String personal[15][6]; //0=TYPE; 1=A; 2=B; 3=C; 4=D; 5=STATE
      personal_count = 0;
      while (1) {
        int n1; 
        int n2;
        n1 = cmd.indexOf(F(":||"));
        if ((n1 == -1) || (personal_count > 14)) {
          cmd = F("");
          if (personal_count > 0) {
            if (PANE == 1) {
              streams_banner();
            }
          }
          break;   
        }
        
        n2 = cmd.indexOf(F(":||"), n1+1);
        if (n2 == -1) n2 = cmd.indexOf(F("^"));
        
        int a1 = cmd.indexOf(F("|"), n1+3); int a2 = cmd.indexOf(F("|"), a1+1); int a3 = cmd.indexOf(F("|"), a2+1); int a4 = cmd.indexOf(F("|"), a3+1);
        if (cmd.substring(n1+3, a1) == F("E")) {
          personal[personal_count][0] = cmd.substring(n1+3, a1);
          if (BLE == 1) {
            String per_tmp; 
            per_tmp = cmd.substring(a1+1, a2);
            personal[personal_count][1] = per_tmp.substring(0, per_tmp.indexOf(F("<"))-1);
            personal[personal_count][2] = per_tmp.substring(per_tmp.indexOf(F("<"))+1, per_tmp.indexOf(F(">")));
          }
          else {
            personal[personal_count][1] = cmd.substring(a1+1, a2);
            personal[personal_count][2] = cmd.substring(a2+1, a3);
          }
          personal[personal_count][3] = cmd.substring(a3+1, a4);
          personal[personal_count][4] = cmd.substring(a4+1, n2-2);
          personal[personal_count][5] = F("2"); // CHANGE TO ACTUAL STATE
          personal_count++;
        }
        cmd = cmd.substring(n2, cmd.length());
      }
      
      //if (personal_timer == 0)
      //{
        
        MCU.println(F("***PERSONAL STREAMS***"));
        int r;
        for (r = 0; r < personal_count; r++)
        {
          MCU.print(F("Type (")); MCU.print(r); MCU.print(F("): ")); MCU.println(personal[r][0]);
          MCU.print(F("A (")); MCU.print(r); MCU.print(F("): ")); MCU.println(personal[r][1]);
          MCU.print(F("B (")); MCU.print(r); MCU.print(F("): ")); MCU.println(personal[r][2]);
          MCU.print(F("C (")); MCU.print(r); MCU.print(F("): ")); MCU.println(personal[r][3]);
          MCU.print(F("D (")); MCU.print(r); MCU.print(F("): ")); MCU.println(personal[r][4]);
          MCU.println();
        }
        
        //while (BC127.available()) { BC127.read(); }
        //BC127.resetCommandList();
      //}
      personal_timer = millis();
      if ((PANE == 1) && (str_flag == 1) && (social_count > 0)) {
        social_pane(); str_flag = 0; return;
      }
      else if ((PANE == 1) && (str_flag == 1) && (personal_count > 0)) {
        personal_pane(); str_flag = 0; return;
      }
      else {
        str_flag = 0;
      }
      */
      return;
    }
    else if (cmd.indexOf(F("LGN:")) != -1) { // LONG NAME RECEIVED
      LNM = cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")));
      prv_flag = 1;
      while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
      BC127.print(F("SEND LGN:OK^\n\r"));
      BC127.print(F("SET NAME=")); BC127.print(LNM); BC127.print(F("\r")); BC127.flush(); delay(75); bc127_response();
      BC127.print(F("WRITE\r")); BC127.flush(); delay(100); bc127_response();
      if (PRV == 0)
      {
        PRV = 1; char oneChar = '1';
        if (EEPROM.read(1) != oneChar)
        {
          EEPROM.write(1, '1');
        }
      }
      hfp_flag = 0; BTC = 0; HFP = 0; rec_flag = 1;
      BC127.print(F("RESET\r")); BC127.flush(); delay(150);
      while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
      BC127.print(F("STATUS\r")); BC127.flush();
      return;
    }
    else if (cmd.indexOf(F("AMB^")) != -1) { // EQUALIZER GET COMMAND
      update_bright();
      BC127.print(F("SEND AMB:") + String(AMB) + F("^\n\r"));
      return;
    }
    else if (cmd.indexOf(F("COL:")) != -1) { //LIGHT PAD CONTROLLER
      int ind_a = cmd.indexOf(F(":")); int ind_b = cmd.indexOf(F(":"), ind_a+1); int ind_c = cmd.indexOf(F(":"), ind_b+1); int ind_d = cmd.indexOf(F("^"));
      COL[0] = (cmd.substring(ind_a+1, ind_b)).toInt(); COL[1] = (cmd.substring(ind_b+1, ind_c)).toInt(); COL[2] = (cmd.substring(ind_c+1, ind_d)).toInt();
      if ((COL[0] + COL[1] + COL[2]) == 0) {
        HUE = 999;
      }
      else {
        HUE = COL[0];
      }
      
      if (LED == 0) LED = 1;
      if (LSW == 0) LSW = 1;
      
      // leds_icon(1); 
      // FastLED.showColor(CRGB(COL[0], COL[1], COL[2]));
      
      fadepad_leds();
      return;
    }
    else if (cmd.indexOf(F("COL^")) != -1) { // LED COLOR GET COMMAND
      BC127.print(F("SEND COL:") + String(COL[0]) + F(":") + String(COL[1]) + F(":") + String(COL[2]) + F("^\n\r"));
      return;
    }
    else if (cmd.indexOf(F("```")) != -1) { // ENTER DFU MODE
      dfu_mode(); 
      return;
    }
    else if (cmd.indexOf(F("^!^")) != -1) { // RESET TO PLSCO FACTORY SETTINGS
      //MCU.println(F("FACTORY RESET"));
      //EEPROM.write(1, '0'); PRV = 0; 
      //USR.print(F("AT+RELD\r"));
      //BC127.print(F("SET NAME=Glowdeck")); BC127.print(SID); BC127.print(F("\r")); BC127.flush(); delay(150);
      //BC127.print(F("WRITE\r")); BC127.flush(); delay(150);
      //BC127.print(F("RESET\r")); BC127.flush(); delay(150);
      force_factory();
      //dfu_mode(); 
      return;
    }
  }
  //INTERNAL COMMANDS
  else {
    if (cmd.indexOf(F("Melody Audio")) != -1) {
      BTC = 0;
      BLE = 0;
      SPP = 0;
      A2DP = 0;
      AVRCP = 0;
      HFP = 0;
      PBAP = 0;
      MAP = 0;
      btc_icon(0);
      charge_icon(0);
      vol_icon(-1);
      wifi_icon(0);
      ble_advertise();
      bc127_response();
      btc_discoverable();
      bc127_response();
      
      while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
      
      bc127_status();
      
      if ((prv_flag == 1) && ((BLE == 1) || (SPP == 1))) {
        send_init();
        
        if (btr_flag == 1) {
          BTQ = F("OPEN ") + String(BTR) + F( "HFP"); 
          attempts_flag++;
        }
        prv_flag = 0;
      }
      else
      {
        if ((btr_flag == 1) && (BTC == 0)) {
          BTQ = F("OPEN ") + String(BTR) + F(" HFP"); attempts_flag++;
        }
      }
      return; 
    }
    //LINK CLOSED
    else if (cmd.indexOf(F("CLOSE_OK")) != -1) {
      if (cmd.indexOf(F(" BLE")) != -1) { // BLE DISCONNECTED
        btc_icon(0);
        charge_icon(0);
        BLE = 0;
        if (BTC == 0) vol_icon(-1);
        
        if (((BTC == 0) && (A2DP == 0) && (SPP == 0)) || (prv_flag == 1)) {
          BC127.print(F("DISCOVERABLE ON\r")); 
          BC127.flush();
        }
        if (STR == 1) {
          location = F(""); temperature = F(""); forecast = F(""); foreLast = F(""); //dateStringLast = F("");
          monthsLast = -1; 
          update_clock();
          if (STR_ICON == 1) {
            clear_stream(); 
            STR_ICON = 0;
          }
          STR = 0;
        }
        
        ble_advertise();
        return;
      }
      else if (cmd.indexOf(F(" SPP")) != -1) { // SPP DISCONNECTED
        SPP = 0;
        if (((BTC == 0) && (A2DP == 0) && (BLE == 0)) || (prv_flag == 1)) {
          btc_icon(0); 
          btc_discoverable();
        }
        if (STR == 1) {
          location = F(""); 
          temperature = F(""); 
          forecast = F(""); 
          foreLast = F(""); 
          monthsLast = -1;
          update_clock();
          
          if (STR_ICON == 1) {
            clear_stream(); 
            STR_ICON = 0;
          }
          STR = 0;
        }
        return;
      }
      else if (cmd.indexOf(F(" HFP")) != -1) { // HFP DISCONNECTED
        HFP = 0;
        if ((SPP == 0) && (BLE == 0)) {
          btc_icon(0); hfp_flag = 0;
        }
        else {
          if (BTC == 1) {
            BC127.print(F("SEND BTC:0^\n\r"));
          }
        }
        BTC = 0;
        return;
      }
      else if (cmd.indexOf(F(" PBAP")) != -1) { // PBAP DISCONNECTED
        PBAP = 0; 
        return;
      }
      else if (cmd.indexOf(F(" AVRCP")) != -1) { // AVRCP DISCONNECTED
        AVRCP = 0;
        BTC = 0; 
        AVRCP = 0;
        if (SND == 1) {
          digitalWrite(AMP, LOW); 
          SND = 0;
        }
        if (music == 1) {
          music = 0; 
          clear_stream();
        }
        return;
      }
      else if (cmd.indexOf(F(" MAP")) != -1) { // MAP DISCONNECTED
        if (((BLE == 1) || (SPP == 1)) && (MAP == 1)) {
          BC127.print(F("SEND MAP:0^\n\r"));
        }
        MAP = 0; 
        return;
      }
      else if (cmd.indexOf(F(" A2DP")) != -1) { // A2DP DISCONNECTED
        BTC = 0; 
        A2DP = 0;
        if (SND == 1) {
          digitalWrite(AMP, LOW); 
          SND = 0;
        }
        if ((BLE != 1) && (SPP != 1)) {
          btc_icon(0);
        }
        if (music >= 1) {
          music = 0; 
          clear_stream();
        }

        vol_icon(-1);
        btc_discoverable();
        return;
      }
    }
    // LINK ERROR  
    else if (cmd.indexOf(F("OPEN_ERROR")) != -1) {
      if (cmd.indexOf(F(" MAP")) != -1) { // MAP ERROR
        if (MAP != 1) {
          if ((btr_flag == 1) && (map_flag != 1)) {
            BC127.print(F("OPEN ") + BTR + F(" MAP\r")); 
            BC127.flush();
            BTQ = F("OPEN ") + BTR + F(" MAP");
            map_flag = 1;
            remap_flag++;
          }
        }
        return;
      }
    }
    
    else if (cmd.indexOf(F("SCO_OPEN")) != -1) {
      if (SND == 0) {
        digitalWrite(AMP, HIGH); 
        SND = 1;
      }
      voice = 1;
      print_info(F("* voice activated *")); 
      info_flag = true;
      ROTATE.reset();
      if (music == 1) {
        clear_stream();
      }
      return;
    }
   
    else if (cmd.indexOf(F("SCO_CLOSE")) != -1) {
      voice = 0;
      clear_stream();
      if (music == 1) {
        if (MST == 1) {
          if ((artist.length() > 0) && (song.length() > 0)) {
            print_track();
          }
        }
      }
      return;
    }
        
    else if (cmd.indexOf(F("ABS_VOL=")) != -1) {
      int tmpvol = (cmd.substring(((cmd.indexOf(F("="))) + 1), cmd.length())).toInt();
      if (tmpvol != volume) vol_icon(tmpvol);
      volume = tmpvol;
      return;
    }
    
    else if (cmd.indexOf(F("MAP NEW_SMS")) != -1) { // NEW NOTIFICATION
      // time_t t = Teensy3Clock.get();
      // per_id = next_slot(1);
      
      // personal[0] = "h";       // ICON
      // personal[1] = "Message"; // SOURCE
      // personal[4] = String(t); // ELAPSED
      // personal[5] = "2";       // STATE
      
      personal[0] = F("M");       // ICON
      personal[1] = F("Message"); // SOURCE
      personal[4] = F("Now");     // ELAPSED
      personal[5] = F("2");       // STATE
      return;
    }
    
    else if (cmd.startsWith(F("NAME: "))) { //TXT NAME
      int a0;
      int a1; 
      cmd.trim();
      a0 = 6; 
      a1 = cmd.length();      
      personal[1] = cmd.substring(a0, a1);
      return;
    }
    
    else if (cmd.startsWith(F("MSG: "))) { // TXT MSG
      int a0; 
      int a1; 
      cmd.trim();
      a0 = 5; 
      a1 = cmd.length();
      personal[3] = cmd.substring(a0, a1);
      personal_counter = -1;
      if (PANE == 5) {
        personal_next();
      }
      else if (PANE == 8) {
        if (messageOn != true) {
          messageOn = true;
          if (BLE == 1) {
            BC127.print(F("SEND SMS:1^\n\r"));
            BC127.flush();
          }
        }
        print_message();
        msg_pulse();
      }
      else {
        // STREAMS APP METHOD
        if (STR == 1) {
          if ((PRC == 2) && (CHQ != 1))  {
            return;
          }
          else {
            personal_pane(); msg_flag = 1;
            if ((LED != 0) && (NGT != 1)) {
              msg_pulse();
            }
          }
        }
        // GLOWDECK APP METHOD
        else {
          pop_message();
        }
      }
      return;
    }
    
    else if (cmd.indexOf(F("AVRCP_MEDIA ")) != -1) { // SONG UPDATE
      int a0; 
      int a1;
      a0 = ((cmd.indexOf(F(":"))) + 2);
      if (a0 == 1) return;
      a1 = cmd.length();
      
      if (cmd.indexOf(F("TITLE: ")) != -1) {
        song = ((cmd.substring(a0, a1)).replace("Ã©", "é")).toLowerCase(); // Uncomment to lowercase song title
        
        //song = cmd.substring(a0, a1);
        //if (DEBUG == 1) { MCU.print(F("SONG: ")); MCU.println(song); }
        return;
      }
      else if (cmd.indexOf(F("ARTIST: ")) != -1) {
        // artist = cmd.substring(a0, a1); return;
        artist = ((cmd.substring(a0, a1)).replace("Ã©", "é")).toLowerCase(); // Uncomment to lowercase artist name
        //artist = cmd.substring(a0, a1);
        //if (DEBUG == 1) { MCU.print(F("ARTIST: ")); MCU.println(artist); }
        return;
      }
      else if (cmd.indexOf(F("PLAYING_TIME(MS): ")) != -1) { // PLAYING NEW SONG, PRINT STREAM
        if ((music == 1) && (MST == 1)) print_track();
        return;
      }
    }
    /*
    else if (cmd.indexOf(F("A2DP_STREAM_START")) != -1) {
      if (SND == 0) {
        digitalWrite(AMP, HIGH); SND = 1; 
      }
      if ((MSL == 1) && (MSA != 1)) {
        music_sync_start(); 
      }
      return;     
    }
    */
    else if (cmd.indexOf(F("A2DP_STREAM_STOP")) != -1) {
      if ((SND == 1) || (digitalRead(AMP) != LOW)) {
        digitalWrite(AMP, LOW); 
        SND = 0; 
      }
      if ((MSA == 1) || (MSL == 1)) {
        music_sync_stop();
        MSA = 0; MSL = 0;
        FastLED.clear();
        FastLED.show();
      }
      
      return;     
    }
  
    else if (cmd.indexOf(F("AVRCP_PLAY")) != -1) { // PLAYING SONG, PRINT STREAM
      music = 1; 
      if ((SND == 0) || (digitalRead(AMP) == LOW)) {
        digitalWrite(AMP, HIGH); 
        SND = 1;
      }
      if (MST == 1) print_track();
      if (MSL == 1) {
        if (ANM != -1) animationStop();
        music_sync_start();
      }
      
      return;
    }
    
    else if (cmd.indexOf(F("AVRCP_PAUSE")) != -1) { // PAUSED SONG, CLEAR STREAM
      if (music == 1) {
        if (STR_ICON != 1) clear_stream();
      }
      if ((MSL == 1) || (MSA != 0)) music_sync_stop();

      music = 2;
      if ((SND == 1) || (digitalRead(AMP) != LOW)) {
        digitalWrite(AMP, LOW); 
        SND = 0;
      }
      
      return;
    }
    
    else if (cmd.indexOf(F("AVRCP_STOP")) != -1) { // PAUSED SONG, CLEAR STREAM
      if (music == 1) {
        if (STR_ICON != 1) clear_stream();
      }
      music = 0; 
      if ((SND == 1) || (digitalRead(AMP) != LOW)) {
        digitalWrite(AMP, LOW); 
        SND = 0;
      }
      if ((MSL == 1) || (MSA != 0)) music_sync_stop();
      return;
    }    
    
    else if (cmd.indexOf(F("CALL_ACTIVE")) != -1) { // ACTIVE CALL
      call = 1;
      if (MSL == 1) music_sync_stop();
      
      if ((SND == 0) || (digitalRead(AMP) != HIGH)) {
        digitalWrite(AMP, HIGH); 
        SND = 1;
      }
      print_info(F("* active call *")); 
      fadeout_leds(); 
      FastLED.clear();
      return;
    }
    
    else if ((cmd.indexOf(F("HANGUP")) != -1) || (cmd.indexOf(F("CALL_END")) != -1)) { // END CALL
      call = 0;
      // clear_stream();
      monthsLast = -1; 
      foreLast = F("0"); 
      if (PANE == 1) update_clock();
      leds_off();
      return;
    }

    else if ((cmd.indexOf(F("CALL_INCOMING")) != -1) || (cmd.indexOf(F("CALL ")) != -1)) { // INCOMING CALL
    
      if ((MSL == 1) || (MSA != 0)) music_sync_stop();
      if ((SND == 0) || (digitalRead(AMP) != HIGH)) {
        digitalWrite(AMP, HIGH); 
        SND = 1;
      }
      call = 2; 
      print_stream(F("<< ignore | accept >>"), F(""));
      if ((NGT != 1) && (LED != 0) && (LSW != 0)) {
        COL[0] = 0; 
        COL[1] = 255; 
        COL[2] = 255; 
        fadecross_leds();
      }
      return;
    }
    
    else if (cmd.indexOf(F("ROLE_OK ")) != -1) { // NEWLY CONNECTED BTC DEVICE, STORE ADDRESS
      // ROLE_OK F06BCA2AF7A3 S
      int a1 = (cmd.indexOf(F(" "))+1); 
      int a2 = a1+12;
      BTR = cmd.substring(a1, a2); 
      char btr_char;
      btr_char = EEPROM.read(20); 
      
      if (btr_flag == 0) {
        if ((String(btr_char)) != (BTR.substring(0,1))) {
          if (BTR.length() == 12) {
            char BTR_CHR[13];
            BTR.toCharArray(BTR_CHR, 13);
            eeprom_write_string(20, BTR_CHR);
            btr_flag = 1; 
            return;
          }
          else {
            btr_flag = 0; 
            return;
          }
        }
      }
      else {
        if (String(btr_char) != BTR.substring(0,1)) {
          if (BTR.length() == 12) {
            char BTR_CHR[13]; 
            BTR.toCharArray(BTR_CHR, 13);
            eeprom_write_string(20, BTR_CHR);
            btr_flag = 1; 
            return;
          }
        }
      }        
    }
    
    else if (cmd.indexOf(F("PAIR_OK ")) != -1) { // NEWLY PAIRED DEVICE, OPEN MAP PROFILE
      // ROLE_OK F06BCA2AF7A3 S
      print_info(F("* audio paired *")); 
      info_flag = true;
      int a1 = cmd.indexOf(F(" "))+1; 
      int a2 = a1+12;
      BTR = cmd.substring(a1, a2); 
      char btr_char = EEPROM.read(20); 
      
      if (btr_flag == 0) {
        if (String(btr_char) != BTR.substring(0,1)) {
          if (BTR.length() == 12) {
            char BTR_CHR[13]; 
            BTR.toCharArray(BTR_CHR, 12);
            eeprom_write_string(20, BTR_CHR);
            btr_flag = 1;
            BC127.print(F("OPEN ") + BTR + F(" MAP\r")); 
            BC127.flush(); 
            return;
          }
          else {
            btr_flag = 0; 
            return;
          }
        }
      }
      else {
        if (String(btr_char) != BTR.substring(0,1)) {
          if (BTR.length() == 12) {
            char BTR_CHR[13]; 
            BTR.toCharArray(BTR_CHR, 12);
            eeprom_write_string(20, BTR_CHR);
            btr_flag = 1;
            BC127.print(F("OPEN ") + BTR + F(" MAP\r")); 
            BC127.flush(); 
            return;
          }
        }
      }        
      return;
    }
    
  }
}

void colorTone(int r, int g, int b) {
  int v = 255;
  
  if ((r < g) && (r < b)) {
    v = (g+b)/2;
  }
  else if ((g < r) && (g < b)) {
    v = (r+b)/2;
  }
  else {
    v = (r+g)/2;
  }

  String n = F("G");
  
  if ((r+g+b) == 0) {
    n = F("R");
  }
  else if ((r+g+b) >= 400) {
    n = F("G");
  }
  else if ((r+g+b) >= 315) {
    n = F("F");
  }
  else if ((r+g+b) >= 225) {
    n = F("E");
  }
  else if ((r+g+b) >= 175) {
    n = F("D");
  }
  else if ((r+g+b) >= 80) {
    n = F("C");
  }
  else if ((r+g+b) >= 30) {
    n = F("B");
  }
  else {
    n = F("A");
  }
  
  String tn = F("");
  int dec = 20;
  int tim = 0;
  int len = 1;
  int tem = 120;
  int oct = 6;
  tone_play(n, oct, tn, tem, tim, v, dec, len);  
}

void tone_play(String note, int octave, String tnote, int tempo, int timbre, int vol, int dec, int len) { 
  //TEMPO     TE    0-4095
  //TIMBRE    TI    0-7
  //VOLUME    V     0-255
  //DECAY     D     00-FF
  
  String tone_string = F("TONE V ") + String(vol) + F(" TI ") + String(timbre) + /*F(" TE ") + String(tempo) + */ F(" D ") + String(dec) + F(" N ") + note + String(octave) + F(" L ") + String(len) + F("\r");
  
  BC127.print(tone_string);
  return;
}

void bt_state(String cmd) {
  int bt_onflag = 0;
  if ((BTC == 1) || (BLE == 1) || (SPP == 1)) bt_onflag = 1;
  
  
  if (cmd.indexOf(F("STATE CONNECTABLE DISCOVERABLE ADVERTISING")) != -1) //NO BLUETOOTH CONNECTION
  {
    BTC = 0; BLE = 0; SPP = 0; A2DP = 0; AVRCP = 0; HFP = 0; PBAP = 0; MAP = 0;
    if (bt_onflag == 1) {
      btc_icon(0);
      BC127.print(F("ADVERTISING ON\r")); BC127.flush();
      delay(50);
      BC127.print(F("DISCOVERABLE ON\r")); BC127.flush();
    }
    if (PANE != 1) {
      clear_stream();
      home_pane();
    }
    return;
  }
    
  if (cmd.indexOf(F("CONNECTED CONNECTED")) != -1) {
    BTC = 1;
    if (cmd.indexOf(F("ADVERTISING")) != -1) {
      SPP = 1;
      //BT_PREFIX = 14;
      BLE = 0;
    }
    else {
      BLE = 1;
      if (nextGen) {
        BT_PREFIX = 8;
      }
      else {
        BT_PREFIX = 9;
      }
    }
  }
  if (cmd.indexOf(F("ADVERTISING")) != -1) {
    BLE = 0;
    if (SPP == 0) {
      if (PANE != 1) {
        STR = 0;
        clear_stream();
        home_pane();
      }
    }
    //ble_advertise();   
  }
  if (cmd.indexOf(F("CONNECTABLE")) != -1) {
    BTC = 0; SPP = 0;
    if (cmd.indexOf(F("DISCOVERABLE")) == -1) {
      btc_discoverable(); BTQ = F("ADVERTISING ON\r");
    }
  }
  if (cmd.indexOf(F("E CONNECTED")) != -1) {
    BLE = 1; 
    if (nextGen) {
      BT_PREFIX = 8;
    }
    else {
      BT_PREFIX = 9;
    }
    
    if ((btr_flag == 1) && (BTC == 0) && (hfp_flag < 2)) {
      BTQ = F("OPEN ") + BTR + F(" HFP"); 
      BC127.print(F("OPEN ") + BTR + F(" HFP\r")); 
      BC127.flush();
      attempts_flag++; 
      hfp_flag++;
    }
  }
  
  if ((BLE == 1) || (SPP == 1) || (BTC == 1)) {
    if (bt_onflag == 0) {
      if (PANE != 6) {
        btc_icon(1);
      }
    }
  }
  else if ((BLE == 0) && (SPP == 0) && (BTC == 0)) {
    if (bt_onflag == 1) {
      btc_icon(0);
    }
    if (PANE != 1) {
      clear_stream();
      home_pane();
    }
  } 
  return;
}

void bt_change(String cmd) {
  if (cmd.startsWith(F("OPEN_OK"))) {
    if ((cmd.indexOf(F("BLE")) != -1) && (ble_flag != 1)) {
      bleIconLast = F("x");
      BLE = 1;
      if (nextGen) {
        BT_PREFIX = 8;
      }
      else {
        BT_PREFIX = 9;
      }
  
      ble_icon(1);
      monthsLast = -1;
      foreLast = F("");
      map_flag = 0;
      ver_flag = 0;

      String ver_transmit = F("SEND VER:0.") + String(VER) + F("^\n\r");
      while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
      BC127.print(ver_transmit);
      BC127.flush();
      
      print_info(F("* app connected *"));
      info_flag = true;
      ROTATE.reset();

      ledConnectionRoutine();
      
      if (BTC == 0) {
        if (btr_flag == 1) {
          BC127.print(F("OPEN ") + BTR + F(" HFP\r")); 
          BC127.flush();
        }
        //while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
        //BC127.print(F("SEND BTC:0^\n\r"));
        //BC127.flush();
        //bc127_response();
        BC127.print(F("STATUS\r"));
      }
      else {
        BC127.print(F("SEND BTC:1^\n\r")); 
        BC127.flush();
        //bc127_response(); 
        //delay(50);
        
        if (MAP != 1) {
          if (btr_flag == 1) {
            BC127.print(F("OPEN ") + BTR + F(" MAP\r"));
            BC127.flush();
            delay(50);
          }
          //while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
          //BC127.print(F("SEND MAP:0^\n\r")); 
          //BC127.flush(); 
          //bc127_response();
          //delay(75);
        }
        else {
          BC127.print(F("SEND MAP:1^\n\r")); 
          BC127.flush(); 
          //bc127_response(); 
          //delay(75);
        }
      }
      
      ble_flag = 1;

      /*
      if ((BTC == 0) && (btr_flag == 1) && (BTR != F("000000000000"))) {        
        BC127.print(F("OPEN ") + BTR + F(" HFP\r")); 
        BC127.flush(); 
        delay(50);
        bc127_response();
        delay(150);
        
        if ((map_flag != 1) && (MAP != 1)) {
          //print_info(F("* connecting streams *")); 
          //info_flag = true; 
          map_flag = 1;
          BC127.print(F("OPEN ") + BTR + F(" MAP\r")); 
          BC127.flush(); 
          delay(50);
        }
      }
      else if ((BTC == 1) && (MAP != 1) && (map_flag != 1) && (btr_flag == 1)) {
        //print_info(F("* connecting streams *")); 
        //info_flag = true; 
        map_flag = 1;
        BC127.print(F("OPEN ") + BTR + F(" MAP\r")); 
        BC127.flush(); 
        delay(50);          
      }
      */
      

      /*
      if (NGT == 1) {
        BC127.print(F("SEND NGT:1^\n\r")); BC127.flush(); delay(50); bc127_response();
      }
      else if (NGT == 0) {
        BC127.print(F("SEND NGT:0^\n\r")); BC127.flush(); delay(50); bc127_response();
      }
      
      if (LSW == 0) {
        BC127.print(F("SEND LSW:0^\n\r")); BC127.flush(); delay(50); bc127_response();
      }
      else if (LSW == 1) {
        BC127.print(F("SEND LSW:1^\n\r")); BC127.flush(); delay(50); bc127_response();
      }
      */

      if (POW == 0) {
        BC127.print(F("SEND POW:0^\n\r")); 
        BC127.flush(); 
        delay(50); 
        bc127_response();
      }
      else if (POW == 1) {
        BC127.print(F("SEND POW:1^\n\r")); 
        BC127.flush(); 
        delay(50); 
        bc127_response();
      }

      if (MSL == 1) {
        BC127.print(F("SEND MSL:1^\n\r")); BC127.flush(); delay(50); bc127_response();
      }
      else if (MSL == 0) {
        BC127.print(F("SEND MSL:0^\n\r")); BC127.flush(); delay(50); bc127_response();
      }
     
      if (DBA == 1) {
        BC127.print(F("SEND DBA:1^\n\r")); BC127.flush(); delay(50); bc127_response();
      }
      else if (DBA == 0) {
        BC127.print(F("SEND DBA:0^\n\r")); BC127.flush(); delay(50); bc127_response();
      }

      BC127.print(F("SEND DBR:") + String(DBR) + F("1^\n\r")); BC127.flush(); delay(50); bc127_response();
      
      while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
      
      BC127.print(F("SEND SYNC^\n\r")); BC127.flush();
      // BC127.print(F("STATUS\r")); BC127.flush();

      if ((BTR.length() == 12) && (BTR != F("000000000000"))) {
        if (BTC == 0) {
          // while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
          BC127.print(F("OPEN ") + BTR + F(" HFP\r"));
        }
        else {
          if (MAP != 1) {
            BC127.print(F("OPEN ") + BTR + F(" MAP\r"));
          }
        }
      }
    
      ROTATE.reset();

      return;
    }
    else if (cmd.indexOf(F("SPP")) != -1) {
      SPP = 1;
      //BT_PREFIX = 14;
      
      monthsLast = -1; 
      if (PANE != 6) {
        btc_icon(1); 
      }
      print_info(F("* data connected *"));
      info_flag = true;
      ROTATE.reset();
      BC127.print(F("SEND SPP:1^\n\r"));
      
      //startBluetoothScan();
      ledConnectionRoutine();
      BC127.print(F("SEND TIM^\n\r"));
      //return;
    }
    else if (cmd.indexOf(F("MAP")) != -1) {
      MAP = 1;
      map_flag = 1;
      if (BLE == 1) {
        BC127.print(F("SEND MAP:1^\n\r"));
      }
      // clear_stream();
      // print_stream(F("Mobile Notifications"), F("ON"));
    }
    else if (cmd.indexOf(F("A2DP")) != -1) {
      if ((BLE == 1) || (SPP == 1)) {
        BC127.print(F("SEND BTC:1^\n\r"));
      }
      BTC = 1;
      a2dp_flag = 1;
      bleIconLast = F("x");
      ble_icon(1);
    }
  }
  else if (cmd.startsWith(F("CLOSE_OK"))) {
    if (cmd.indexOf(F("BLE")) != -1) {
      BLE = 0; ble_flag = 0; ver_flag = 0; a2dp_flag = 0; map_flag = 0; btcsend_flag = 0; rec_flag = 0;
      
      forecast = F(""); foreLast = F("-1"); temperature = F(""); location = F(""); monthsLast = -1; //dateStringLast = F("");
      CGS = 0;
      if (CHG == 0) {
        CHG = 1;
        digitalWrite(COIL_PW, HIGH);
      }

      if (PANE != 1) {
        home_pane();
      }

     if ((ANM != -1) || (ANA != 0)) {
        animationStop();          
      }
      if ((MSL == 1) || (MSA != 0)) {
        music_sync_stop();
        MSL = 0; MSA = 0;
      }

      ver_flag = 0;
      FastLED.clear(); 
      FastLED.show();
      
      if (BTC == 0) {
        remap_flag = 0;
        btc_icon(0);
        if ((LSW == 0) || (LED == 0)) {
          LSW = 1; 
          LED = 1;
          leds_off();
          FastLED.clear();
          FastLED.show();
          HUE = 999; 
          COL[0] = 0; COL[1] = 0; COL[2] = 0;
        }
 
        print_info(F("* data and audio off *"));
        charge_icon(0);
        info_flag = true;
        ROTATE.reset();
        
        // BC127.print(F("DISCOVERABLE ON\r")); BC127.flush(); 
      }
      else {
        print_info(F("* data off, audio on *"));
        charge_icon(0);
        info_flag = true; 
        ROTATE.reset();
        // BC127.print(F("ADVERTISING ON\r")); 
        // BC127.flush();        
      }
      
      //fadeout_leds(); HUE = 999; COL[0] = 0; COL[1] = 0; COL[2] = 0; STR = 0;
      clear_stream();
    }
    else if (cmd.indexOf(F("SPP")) != -1) {
      SPP = 0;
      forecast = F(""); foreLast = F("-1"); temperature = F(""); location = F(""); 
      monthsLast = -1; 
      btcsend_flag = 0;
      
      CGS = 0;
      if (CHG == 0) {
        CHG = 1;
        digitalWrite(COIL_PW, HIGH);
      }
      if ((LSW == 0) || (LED == 0)) {
        LSW = 1; LED = 1;
        leds_off(); HUE = 999; COL[0] = 0; COL[1] = 0; COL[2] = 0;
      }
      if (PANE != 1) {
        home_pane();
      }
      //clear_stream();
      ver_flag = 0;
      print_info(F("* data off *")); info_flag = true; ROTATE.reset();
      // FastLED.clear(); FastLED.show();
      // BC127.print(F("ADVERTISING ON\r")); BC127.flush();
      // fadeout_leds(); HUE = 999; COL[0] = 0; COL[1] = 0; COL[2] = 0; STR = 0;
      clear_stream();
    }
    else if (cmd.indexOf(F("MAP")) != -1) {
      map_flag = 0; remap_flag = 0; hfp_flag = 0; a2dp_flag = 0; rec_flag = 0; rec_flag = 0;
      if (BLE == 1) {
        BC127.print(F("SEND MAP:0^\n\r"));
      }
      if (PANE == 8) {
        home_pane();
      }
      // clear_stream();
      // print_info(F("* streams off *")); info_flag = true;
     
      MAP = 0;
      if (BLE == 1) {
        BC127.print(F("SEND MAP:0^\n\r")); BC127.flush();
      }
    }
    else {  // ASSUME ALL BTC CLOSED
      btc_flag = 0; map_flag = 0; remap_flag = 0; a2dp_flag = 0; ble_flag = 0; map_flag = 0;
      
      if (BLE == 1) {
        BC127.print(F("SEND BTC:0^\n\r"));
      }
      else {
        btc_icon(0);
        BC127.print(F("ADVERTISING ON\r")); BC127.flush(); delay(50);
        BC127.print(F("DISCOVERABLE ON\r")); BC127.flush();
      }
      BTC = 0;
      artist = F(""); 
      song = F("");
      
      if ((MSA == 1) || (MSL == 1)) {
        music_sync_stop();
        MSL = 0; MSA = 0;
        // if (BLE == 1) BC127.print(F("SEND MSL:0^\n\r"));
      }
      else if ((ANM != -1) || (ANA != 0)) {
        animationStop();
        ANM = -1; ANA = 0;
        // if (BLE == 1) BC127.print(F("SEND ANM:-1^\n\r"));
      }
      leds_off();
      if (SND == 1) {
        SND = 0;
        digitalWrite(AMP, LOW); 
      }
      if (BLE == 0) clear_stream();
    }
  }
  else if (cmd.indexOf(F("OPEN_ERROR MAP")) != -1) {
    if ((map_flag != 1) || (remap_flag < 3)) {
      if (btr_flag == 1) {
        remap_flag++;
        BC127.print(F("OPEN ") + BTR + F(" MAP\r")); 
        BC127.flush();
        
        if (((BLE == 1) || (SPP == 1)) && (map_flag != 1)) {
          BC127.print(F("SEND MAP:0^\n\r"));
        }
        map_flag = 1;
        clear_stream();
        return;
      }
    }
    else {
      if ((BLE == 1) && (remap_flag < 4)) {
        remap_flag++;
        BC127.print(F("SEND MAP:0^\n\r"));
        BC127.flush();
      }
    }
  }
  
  //NEW LINK
  else if (cmd.indexOf(F("OPEN_OK ")) != -1) {
    if (cmd.indexOf(F(" BLE")) != -1) {     // BLE CONNECTED
      if (BTC == 0) {
        btc_icon(1);
        if ((rec_flag == 1) && (BTR.length() > 10) && (BTR != F("000000000000"))) {
          rec_flag = 0; hfp_flag = 1;
          BC127.print(F("OPEN ") + BTR + F(" HFP\r"));
          BC127.flush();
          delay(225);
          BC127.print(F("OPEN ") + BTR + F(" MAP\r"));
          BTQ = F("OPEN ") + BTR + F(" MAP"); 
          attempts_flag++;
        }
       // else if ((rec_flag == 0) && (btr_flag == 1) && (BTR != F("000000000000")) && (MAP != 1)) {
          //rec_flag = -1;
          //BC127.print("OPEN " + BTR + " MAP\r"); BC127.flush(); 
        //}
      }
      BLE = 1;
      if (nextGen) {
        BT_PREFIX = 8;
      }
      else {
        BT_PREFIX = 9;
      }
      
    }
    else if (cmd.indexOf(F(" MAP")) != -1) { // MAP CONNECTED
      if (BTQ.indexOf(F("MAP")) != -1) BTQ = F("");
     
      if ((BLE == 1) || (SPP == 1)) {
        BC127.print(F("SEND MAP:1^\n\r"));
      }
      BTC = 1; MAP = 1; map_flag = 1; remap_flag = 5;
      return;
    }
    else if (cmd.indexOf(F(" SPP")) != -1) {  // SPP CONNECTED (ANDROID/WP)
      if (SPP == 0) {
        if (PANE != 6) btc_icon(1);
        
        if ((btr_flag == 1) && (hfp_flag != 1)) {
          BC127.print(F("OPEN ") + BTR + F(" HFP\r")); BC127.flush();
          //BTQ = F("OPEN ") + String(BTR) + F(" HFP"); 
          hfp_flag = 1; attempts_flag++;
        }
        
      }
      SPP = 1; 
      //BT_PREFIX = 14;

      if ((prv_flag == 1) && (BLE != 1)) {
        send_init(); 
        prv_flag = 0;
      }
      return;
    }
    
    else if (cmd.indexOf(F(" A2DP")) != -1) { // BTC CONNECTED
      if (PANE != 6) {
        btc_icon(1);
      }
      if (((BLE == 1) || (SPP == 1)) && ((BTC == 0))) {
        BC127.print(F("SEND BTC:1^\n\r")); 
        BC127.flush();
      }
      BTC = 1; 
      A2DP = 1;
      bleIconLast = F("x");
      return;
    }
    else if (cmd.indexOf(F(" PBAP")) != -1) { // BTC CONNECTED
      PBAP = 1;
      if ((MAP == 0) && (BTQ == F(""))) {
        if ((btr_flag == 1) && (map_flag < 2)) {
          BTQ = F("OPEN ") + BTR + F(" MAP\r");
          // BC127.print(F("OPEN ") + BTR + F(" MAP\r")); BC127.flush();
          map_flag = (map_flag + 1) % 10;
        }
      }
      return;
    }
  }
  else if (cmd.indexOf(F("CLOSE_OK ")) != -1) {
    if (cmd.indexOf(F(" BLE")) != -1) {      // BLE DISCONNECTED (IOS)
      BLE = 0; ver_flag = 0; time_flag = 0; wtr_flag = 0; ble_flag = 0; bleIconLast = F("x");
      temperature = F(""); forecast = F(""); location = F(""); foreLast = F(""); monthsLast = -1;
      
      if (BTC == 0) {
        btc_icon(0); 
        BC127.print(F("DISCOVERABLE ON\r")); 
        BC127.flush(); 
        delay(25);
      }
      
      if (STR == 1) {
        STR = 0;
        if (PANE != 1) {
          temperature = F(""); forecast = F(""); location = F(""); foreLast = F(""); monthsLast = -1;
          social_count = 0; personal_count = 0; //STR_ICON = 0;
          home_pane();
        }
        else {
          if (STR_ICON == 1) {
            temperature = F(""); forecast = F(""); location = F(""); foreLast = F(""); monthsLast = -1;
            social_count = 0; personal_count = 0; clear_stream(); STR_ICON = 0;
            update_clock();
          }
        }
      }
      else if ((PANE != 1) && (SPP != 1)) {
        temperature = F(""); forecast = F(""); location = F(""); foreLast = F(""); monthsLast = -1;
        social_count = 0; personal_count = 0; home_pane(); STR_ICON = 0;
      }
      return;
    }
    
    else if (cmd.indexOf(F(" MAP")) != -1) {  // MAP DISCONNECTED
      map_flag = 0; remap_flag = 0;
      
      if ((BLE == 1) || (SPP == 1)) {
        BC127.print(F("SEND MAP:0^\n\r")); 
      }
      MAP = 0;
      return;
    }
    else if (cmd.indexOf(F(" SPP")) != -1) { // SPP DISCONNECTED (ANDROID/WP)
      SPP = 0;
      if ((BTC == 0) && (BLE == 0))
      {
        btc_icon(0);
      }
      
      if ((STR == 1) && (BLE != 1))
      {
        STR = 0; 
        if (PANE != 1) {
          temperature = F(""); forecast = F(""); location = F(""); foreLast = F(""); monthsLast = -1;
          social_count = 0; personal_count = 0; //STR_ICON = 0;
          home_pane();
        }
        else {
          if (STR_ICON == 1) {
            clear_stream();
            temperature = F(""); forecast = F(""); location = F(""); foreLast = F(""); monthsLast = -1;
            social_count = 0; personal_count = 0; clear_stream(); update_clock();
          }
        }
      }
      else if (PANE != 1) {
        temperature = F(""); forecast = F(""); location = F(""); foreLast = F(""); monthsLast = -1;
        social_count = 0; personal_count = 0; 
        home_pane(); STR_ICON = 0;
      }
      SPP = 0;
      return;
    }
    else if (cmd.indexOf(F(" PBAP")) != -1) { // BTC DISCONNECTED
      PBAP = 0;
      return;
    }
    else if (cmd.indexOf(F(" A2DP")) != -1) { // BTC DISCONNECTED
      BTC = 0; A2DP = 0; AVRCP = 0; HFP = 0; PBAP = 0; MAP = 0;
      
      // BC127.print("DISCOVERABLE ON\r");
      /*
      if (((BLE == 1) || (SPP == 1)) && (BTC == 1))
      {
        BC127.print(F("SEND BTC:0^\n\r")); 
      }
      else
      {
        btc_icon(0);
      }
      BTC = 0;
      */

      btc_icon(0);
      charge_icon(0);
      
      if (BLE == 1) {
        BC127.print(F("SEND BTC:0^\n\r")); 
        BC127.flush();
      }
      
      if (music == 1) {
        artist = F(""); song = F("");
        clear_stream(); music = 0;
        if ((STR == 1) && ((personal_count+social_count) > 0) && (PANE == 1) && (BLE == 1)) {
          streams_banner();
        }
      }
      else if (music == 2) {
        artist = F(""); song = F("");
        music = 0;
        if ((STR == 1) && ((personal_count+social_count) > 0) && (PANE == 1) && (BLE == 1)) {
          streams_banner();
        }
        else if (STR_ICON != 0) {
          clear_stream();
        }
      }
      return;
    }  
  }
}

void bt_link(String cmd) {
  
  if (cmd.indexOf(F("A2DP")) != -1)
  {
    int inA = cmd.indexOf(F("A2DP")) + 5;
    int inB = inA + 12;
    String btr_now = cmd.substring(inA, inB);
    //MCU.print(F("BTR_NOW: ")); MCU.println(btr_now);
    
    String btr_temp = F(""); char inChr;
    for (int s = 20; s < 32; s++)
    {
      inChr = EEPROM.read(s); delay(6);
      btr_temp += String(inChr);
    }
    
    //MCU.print(F("BTR_PRE: ")); MCU.println(btr_temp);
    
    if (btr_temp != btr_now)
    {
      char btr_array[13];
      btr_now.toCharArray(btr_array, 13);
      eeprom_write_string(20, btr_array);
      BTR = btr_now; btr_flag = 1;    
    }
    else
    {
      btr_flag = 1; BTR = btr_now;
    }
    
    if (BTC != 1)
    {
      if (PANE != 6)
      {
        btc_icon(1);
      }
    }
    
    BTC = 1; A2DP = 1;
    if ((BLE == 1) && (btcsend_flag != 1))
    //if (BLE == 1)
    {
      BC127.print(F("SEND BTC:1^\n\r")); BC127.flush(); btcsend_flag = 1;
    }
    return;
  }
  
  else if (cmd.indexOf(F("PBAP")) != -1)
  {
    if ((MAP != 1) && (map_flag != 1) && (btr_flag == 1))
    {
      PBAP = 1;

      if ((MAP != 1) && (map_flag == 0) && (btr_flag == 1))
      {
        BC127.print(F("OPEN ") + BTR + F(" MAP\r")); BC127.flush();
        map_flag = (map_flag+1) % 10;
        BTQ = F("OPEN ") + BTR + F(" MAP");
      }

      if ((BLE == 1) && (btcsend_flag != 1))
      {
        BC127.print(F("SEND BTC:1^\n\r")); BC127.flush(); btcsend_flag = 1;
      }
      
      return;
    }
    else
    {
      PBAP = 1; 
      if (MAP == 0)
      {
        if (btr_flag == 1) 
        {
          if (map_flag == 0)
          {
            //BTQ = F("OPEN ") + BTR + F(" MAP");
            BC127.print(F("OPEN ") + BTR + F(" MAP\r")); BC127.flush();
            map_flag = (map_flag+1) % 10;
          }
          else
          {
            if (!(remap_flag >= 2))
            {
              remap_flag = 1;
            }
          }
        }
      }
      return;
    }
  }
  
  else if ((cmd.indexOf(F("BLE")) != -1) && (ble_flag != 1))
  {
    int len = cmd.length();
    bleAddr = cmd.substring(len-12, len).trim();
    
    //if ((BLE != 1) && (BTC != 1))
    //{
    //  if (PANE != 6)
    //  {
    //    btc_icon(1);
    //  }
    //}
    BLE = 1;
    if (nextGen) {
      BT_PREFIX = 8;
    }
    else {
      BT_PREFIX = 9;
    }
    ble_flag = 1;
    ble_icon(1);
    /*
    if (bleAddr.length() == 12)
    {
      //char bleChar[12];
      bleAddr.toCharArray(bleAddress, 12);
      
      if ( (EEPROM.read(220) != bleAddress[0]) || (EEPROM.read(231) != bleAddress[11]) )
      {
        for (int g = 220; g < 232; g++)
        {
          EEPROM.write(g, bleAddress[g-220]); 
          delay(5);
        }
      }
    }
    */

    if (BTC == 0) 
    {
      BC127.print(F("SEND BTC:0^\n\r")); BC127.flush(); delay(50); 

      if (btr_flag == 1)
      {
        //MCU.print(F("Attempting to open Audio link: ")); MCU.println(btcAddr);
        //print_info(F("*Connecting Audio*")); info_flag = true; //hfp_flag = 1;
        
        BC127.print(F("OPEN ") + BTR + F(" HFP\r")); BC127.flush(); delay(50);
        
        if ((map_flag == 0) && (MAP != 1))
        {
          //print_info(F("*Connecting Messages*")); info_flag = true; map_flag = 1;
          BC127.print(F("OPEN ") + BTR + F(" MAP\r")); BC127.flush(); delay(50);
          map_flag = (map_flag + 1) % 10;
        }
      }
      else if ((BTC == 1) && (btr_flag == 0))
      {
        return;
        //print_info(F("*Pair Audio in Settings*")); info_flag = true; ROTATE.reset();
      }
    }
    else
    {
      if ((MAP == 1) && (BLE == 1))
      {
        BC127.print(F("SEND MAP:1^\n\r")); BC127.flush(); delay(50);
      }
      else if (BLE == 1)
      {
        BC127.print(F("SEND BTC:1^\n\r")); BC127.flush(); delay(50);
        BC127.print(F("SEND MAP:0^\n\r")); BC127.flush(); delay(50);
      }
    }
    
    return;
  }

  if (cmd.indexOf(F("MAP")) != -1)
  {
    MAP = 1; map_flag = 1; remap_flag = 2;
    if (BTQ.indexOf(F(" MAP")) != -1)
    {
      BTQ = F("");
    }
    if ((BTC == 0) && ((BLE == 0) && (SPP == 0)))
    {
      if (PANE != 6)
      {
        btc_icon(1);
      }
    }
    BTC = 1;
    if (BLE == 1)
    {
      //BC127.print(F("SEND MAP:1^\n\r")); BC127.flush(); delay(100);
    }
    return;
  }
  
  /*
  else if (cmd.indexOf(F("A2DP")) != -1)
  {
    A2DP = 1;
    //int indA = cmd.indexOf(F(" ")); int indB = cmd.indexOf(F(" "), indA+1); int indC = cmd.indexOf(F(" "), indB+1); int indD = cmd.indexOf(F(" "), indC+1); int indE = cmd.indexOf(F(" "), indD+1);
    //String btctmp = (cmd.substring(indD+1, indE)).trim();
    if (BTC == 0)
    {
      BTC = 1; btc_icon(1);
    }
    return;
  }
  */
  
  else if (cmd.indexOf(F("AVRCP")) != -1)
  {
    AVRCP = 1; //BTC = 1;
    if (cmd.indexOf(F("PLAYING")) != -1)
    {
      music = 1;
      if (SND == 0)
      {
        digitalWrite(AMP, HIGH); SND = 1;
      }
      if (MSL == 1) music_sync_start();
    }
    else if (cmd.indexOf(F("PAUSED")) != -1)
    {
      if (music == 1)
      {
        if (STR_ICON != 1)
        {
          clear_stream();
        }
      }
      
      if (MSL == 1) music_sync_stop();
      
      music = 2;
      
      if ((voice < 1) && (call < 1))
      {
        if (SND == 1)
        {
          digitalWrite(AMP, LOW); SND = 0;
        }
      }
    }
    else if (cmd.indexOf(F("STOPPED")) != -1)
    {
      if (music == 1)
      {
        if (STR_ICON != 1)
        {
          clear_stream();
        }
      }
      
      if (MSL == 1) music_sync_stop();
      
      music = 0; 
      
      if ((voice < 1) && (call < 1))
      {
        if (SND == 1)
        {
          digitalWrite(AMP, LOW); SND = 0;
        }
      }          
    }
    return;
  }
  else if (cmd.indexOf(F("HFP")) != -1)
  {     
    HFP = 1;
    if (BTC == 0)
    {
      BTC = 1;
      if (PANE != 6)
      {
        btc_icon(1);
      }
    }
    if (BTQ.indexOf(F(" HFP")) != -1)
    {
      BTQ = F("");
    }
    hfp_flag = 1;
    return;
  }
  else if (cmd.indexOf(F("SPP")) != -1)
  {
    if (SPP == 0)
    {
      if (init_flag == false) {
        send_init(); init_flag = true;
      }
    }
    SPP = 1; 
    // BT_PREFIX = 14;
    /*
    if ((BTC == 0) && (HFP == 0) && (A2DP == 0) && (btr_flag == 1) && (hfp_flag != 1))
    {
      BTQ = F("OPEN ") + BTR + F(" HFP"); attempts_flag++;
      hfp_flag = 1;
    }
    */
    return;
  }
  
  return;
}

void play_tone(int tone_id) {
  //HARDWARE TEST LOOP!
  if (tone_id == 1)
  {
    if (SND == 0)
    {
      digitalWrite(AMP, HIGH); SND = 1;
    }
    while (BC127.available()) { BC127.read(); }
    BC127.print(F("VOLUME HFP=15\r")); 
    while (BC127.available()) { BC127.read(); }
    BC127.print(F("VOLUME A2DP=15\r")); 
    while (BC127.available()) { BC127.read(); }
    BC127.print(F("VOLUME WIRED_A=15\r")); 
    while (BC127.available()) { BC127.read(); }
    BC127.resetCommandList();

    while (1)
    {
      BC127.print(F("TONE TE 400 V 255 TI 0 N C5 L 8 N R0 L 32 N E5 L 8 N R0 L 32 N G5 L 8 N R0 L 32 N B5 L 4 N R0 L 1 N C6 L 2 TN C6 L 8\r")); bc127_response();
      COL[0] = random(0, 255); COL[1] = random(0, 255); COL[2] = random(0, 255);
      FastLED.showColor(CRGB(COL[0], COL[1], COL[2])); leds_icon(1);
      BC127.print(F("TONE V 255 TI 0 N C4 L 8 N D4 L 8 N E4 L 8 N F4 L 8 N G4 L 8 N A4 L 8 N B4 L 8 N C5 L 8\r")); bc127_response();
      BC127.resetCommandList();
      delay(2);
    }
  }
  else if (tone_id == 2)
  {
    /*digitalWrite(AMP, HIGH);*/
    //BC127.print(F("TONE TE 400 V 64 TI 0 N C5 L 8 N R0 L 32 N E5 L 8 N R0 L 32 N G5 L 8 N R0 L 32 N B5 L 4 N R0 L 1 N C6 L 2 TN C6 L 8\r"));
    /*digitalWrite(AMP, LOW);*/
  }
  else if (tone_id == 3)
  {
    /*digitalWrite(AMP, HIGH);*/
    //BC127.print(F("TONE V 64 N C6 L 4 V 128 N C6 L 4 V 255 N C6 L 4 V 128 N C6 L 4 V 64 N C6 L 4\r"));
    /*digitalWrite(AMP, LOW);*/
  }
}

String bt_remote() {
  //LOCAL_ADDR=20FABB00018D
  String result = F("");
  BC127.print(F("GET REMOTE_ADDR\r")); delay(200);
  result = bc127_response(); result.trim();
  if (result.startsWith(F("OK")))
  {
    delay(200); result = bc127_response(); result.trim();
  }
  
  if (result.indexOf(F("REMOTE_ADDR=")) != -1)
  {
    //int len = result.length();
    result = result.substring(12, 24);
    if (result.length() == 12)
    {
      BTR = result;
    }
  }
  return result;
}

String bt_addr() {
  //LOCAL_ADDR=20FABB00018D
  String result = F(""); String tmp; int ok_flag = 1;
  BC127.print(F("GET LOCAL_ADDR\r")); delay(100);
  result = bc127_response(); result.trim();
  if (result.startsWith(F("OK")))
  {
    ok_flag = 0; delay(300); result = bc127_response(); result.trim();
  }
  
  if (result.indexOf(F("LOCAL_ADDR=")) != -1)
  {
    //int len = result.length();
    result = result.substring(11, 23);
    //BTA = result;
  }
  if (ok_flag == 1)
  {
    delay(300); tmp = bc127_response(); tmp = F(""); //GET RID OF OK FROM QUEUE
  } 
  return result;
}

String bt_name() {
  //NAME=BlueCreation-00018D
  String result = F(""); String tmp; int ok_flag = 1;
  BC127.print(F("GET NAME\r")); BC127.flush(); delay(300);
  result = bc127_response(); result.trim();
  if (result.startsWith(F("OK")))
  {
    ok_flag = 0; delay(300); result = bc127_response(); result.trim();
  }
  if (result.indexOf(F("NAME=")) != -1)
  {
    int len = result.length();
    result = result.substring(5, len);
    //LNM = result;
  }
  if (ok_flag == 1)
  {
    delay(300); tmp = bc127_response(); tmp = F(""); //GET RID OF OK FROM QUEUE
  }
  return result;
}

void bc127_status() {
  char bChar; 
  String addr = F(""); 
  String res = F(""); 
   
  while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
  
  BC127.print(F("STATUS\r")); 
  BC127.flush(); 
  delay(20);
  
  //-------------------------------------------------------------------------------------------------------------------------------
  //   BC127 STATUSES - POSSSIBLE RESPONSES:
  //-------------------------------------------------------------------------------------------------------------------------------
  //1. No connections; connectable by paired BTC devices; discoverable by unpaired BTC devices; and connectable by all BLE devices.
  //   STATE CONNECTABLE DISCOVERABLE ADVERTISING (Variation: STATE CONNECTABLE ADVERTISING - correct with DISCOVERABLE ON command)
  //-------------------------------------------------------------------------------------------------------------------------------
  //2. BTC not connected; connectable by paired BTC devices; discoverable by unpaired BTC device; BLE is connected.
  //   STATE CONNECTABLE DISCOVERABLE CONNECTED
  //-------------------------------------------------------------------------------------------------------------------------------  
  //3. BTC is connected and not discoverable, BLE is connectable:
  //   STATE CONNECTED ADVERTISING
  //-------------------------------------------------------------------------------------------------------------------------------
  //4. No connections over any link, connectable by previously-paired devices but not discoverable:
  //   STATE CONNECTABLE ADVERTISING
  //-------------------------------------------------------------------------------------------------------------------------------
  //5. BTC not connected, BLE is connected, connectable over BTC by previously-paired devices but not discoverable:
  //   STATE CONNECTABLE CONNECTED
  //-------------------------------------------------------------------------------------------------------------------------------
  
  /*
  STATE [BTC] [BLE]\r
  
    [BTC]: OFF, CONNECTABLE, DISCOVERABLE, CONNECTABLE, CONNECTED
    [BLE]: OFF, IDLE, ADVERTISING, CONNECTED
  
  LINK [ID] CONNECTED [PROFILE] [ADDRESS] {A2DP CODEC/AVCRP STATE/HFP STATE}
  
    [PROFILE]: HFP, PBAP, MAP, A2DP, AVRCP, SPP, BLE
    [ADDRESS]: 000000000000
    [A2DP CODEC]: APTX, AAC, SBC [44100], MP3
    [AVRCP STATE]: STOPPED, PAUSED, PLAYING
    [HFP STATE]: NB
  
  Example:
  STATE CONNECTED   
  LINK 2 CONNECTED A2DP F06BCA2AF7A3 SBC 44100
  LINK 4 CONNECTED AVRCP F06BCA2AF7A3 PAUSED
  LINK 8 CONNECTED HFP F06BCA2AF7A3 NB
  LINK 10 CONNECTED PBAP F06BCA2AF7A3
  LINK 14 CONNECTED MAP F06BCA2AF7A3
  LINK 0 CONNECTED SPP F06BCA2AF7A3
  */
  
  unsigned long st = millis(); 
  uint8_t err_flag = 0;
  uint8_t change = 0;
  
  while (1) {
    if (BC127.available()) {
      bChar = BC127.read();
      if (bChar == '\r') {
        err_flag = 0;
        break;
      }
      res += bChar;
    }
    if ((millis() - st) > 3750) {
      err_flag = 1;
      break;
    }
  }
  
  if ((err_flag == 0) && (res.startsWith(F("STATE ")))) {
    if (res.indexOf(F("CONNECTED CONNECTED")) != -1) {
      if (BTC != 1) change = 1;
      BTC = 1;
      if (res.indexOf(F("ADVERTISING")) != -1) {
        if ((BLE != 0) || (SPP != 1)) change = 1;
        BLE = 0;
        SPP = 1;
        // BT_PREFIX = 14;
      }
      else {
        if (BLE != 1) change = 1;
        BLE = 1; 
        if (nextGen) {
          BT_PREFIX = 8;
        }
        else {
          BT_PREFIX = 9; 
        }
      }
    }
    
    if (res.indexOf(F("ADVERTISING")) != -1) {
      if (BLE != 0) change = 1;
      BLE = 0;
    }
    
    if (res.indexOf(F("CONNECTABLE")) != -1) {
      if ((BTC != 0) || (MAP != 0) || (SPP != 0)) change = 1;
      
      BTC = 0; 
      SPP = 0; 
      HFP = 0; 
      MAP = 0; 
      A2DP = 0; 
      PBAP = 0; 
      AVRCP = 0;
      
      btcsend_flag = 0; 
      map_flag = 0;
      bleIconLast = F("x");
      vol_icon(-1);
      
      if (BLE == 0) ble_flag = 0;
      else if (BLE == 1) BC127.print(F("SEND BTC:0^\n\r"));
      
      music = 0; 
      artist = F(""); 
      song = F(""); 
      voice = 0; 
      call = 0;
      if (BLE == 0) {
        if (STR_ICON != 1) clear_stream();
      }
    }
    
    if (res.indexOf(F("E CONNECTED")) != -1) {
      if (res.indexOf(F("ADVERTISING")) == -1) {
        if (BLE != 1) change = 1;
        BLE = 1; 
        if (nextGen) {
          BT_PREFIX = 8;
        }
        else {
          BT_PREFIX = 9;
        }
      } 
    }
    else {
      if (BLE != 0) change = 1;
      BLE = 0;
      BC127.print(F("ADVERTISING ON\r"));
    }
    
    if ((BLE != 1) && (res.indexOf(F("ADVERTISING")) == -1)) {
      BC127.print(F("ADVERTISING ON\r")); 
    }
    
    if (change == 1) {
      ble_icon(1);
    }

    /*
    if (res.indexOf(F("CONNECTABLE")) == -1) {
        BC127.print(F("DISCOVERABLE ON\r")); 
        BC127.flush(); 
        bc127_response();
    }
    else {
      if (BLE != 1) {
        BC127.print(F("ADVERTISING ON\r")); 
        BC127.flush(); 
        bc127_response();
      }
      else if (BTC == 0) {
        if (btr_flag != 1) {

        }
        else {
          if (a2dp_flag == 0) {
            BC127.print(F("OPEN ") + BTR + F(" HFP\r")); 
            BC127.flush();
            a2dp_flag = 1;
          }
          else {
            if ((MAP == 0) && (a2dp_flag == 1)) {
              BC127.print(F("OPEN ") + BTR + F(" MAP\r")); 
              BC127.flush();
              a2dp_flag = 2;
            }
          }
        }
      } 
    }
    */
  }
  
  if (BLE == 1) {
    if (BTC == 0) {
      BC127.print(F("SEND BTC:0^\n\r")); 
      BC127.flush();
    }
    else if (BTC == 1) {
      if (MAP == 1) {
        BC127.print(F("SEND MAP:1^\n\r")); 
        BC127.flush();
      }
      else if (MAP == 0) {
        BC127.print(F("SEND MAP:0^\n\r")); 
        BC127.flush();
      }
      else {
        BC127.print(F("SEND BTC:1^\n\r")); 
        BC127.flush();
      }
    }
  }
}

void bt_status() {
  //BTQ = F("STATUS");
  BC127.print(F("STATUS\r"));
}

void ble_advertise() {
  BC127.print(F("ADVERTISING ON\r"));
}

void btc_discoverable() {
  BC127.print(F("DISCOVERABLE ON\r"));
}

void send_init() { 
  
}

/*
void bt_provision()
{
  if (BC127.getCommandCount())
  {
    BC127.pullCommand(buffer2); String tmpuid = (char*)buffer2;
    if (tmpuid.indexOf(F("UID:")) != -1)
    {
      UID = tmpuid.substring((tmpuid.indexOf(F(":"))+1), tmpuid.indexOf(F("^")));
    }
    while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
  }
  
  
  
  if (ssid_id != -1)
  {
    int ssid_num = ssid_id;
    if (WID == F("0"))
    {
      WID = wiscan[ssid_num][0];
    }
    wiscan[ssid_num][4] = WIP;
    
    //SET SSID/CODE
    USR.print(F("AT+WSSSID=") + WID + F("\r")); usr_response(0);
    USR.print(F("AT+WSKEY=")); USR.print(wiscan[ssid_num][1]); USR.print(F(",")); USR.print(wiscan[ssid_num][2]); USR.print(F(",")); USR.print(WIP); USR.print(F("\r")); usr_response(0); delay(100);
    
    //RESET MODULE
    while (USR.available()) { USR.read(); }
    USR.print(F("AT+Z\r")); delay(1050);
    init_usr(); //USR.print(F("AT+E\r")); usr_response(0);
    
    //CHECK CONNECTION STATUS
    while (USR.available()) { USR.read(); }
    USR.print(F("AT+WSLK\r")); String stalink = usr_response(0);
    if (stalink.indexOf(F("Disconnected")) != -1)
    {
      stalink = F(""); delay(1000); 
      USR.print(F("AT+WSLK\r")); stalink = usr_response(0);
      if (stalink.indexOf(F("Disconnected")) != -1)
      {
        stalink = F(""); delay(1000); 
        USR.print(F("AT+WSLK\r")); stalink = usr_response(0);
        if (stalink.indexOf(F("Disconnected")) != -1)
        {
          stalink = F(""); delay(1000); 
          USR.print(F("AT+WSLK\r")); stalink = usr_response(0);
          if (stalink.indexOf(F("Disconnected")) != -1)
          {
            stalink = F(""); delay(1250); 
            USR.print(F("AT+WSLK\r")); stalink = usr_response(0);
            if (stalink.indexOf(F("Disconnected")) != -1)
            {
              stalink = F(""); delay(1500); 
              USR.print(F("AT+WSLK\r")); stalink = usr_response(0);    
              if (stalink.indexOf(F("Disconnected")) != -1)
              {
                WIO = 0; wifi_icon(0);
                while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
                BC127.print(F("SEND WIC:ERR^\n\r"));
                return;
              }
            }
          }
        }
      }
    }
    else
    {
      WIO = 1; wifi_icon(3);
      while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
      BC127.print(F("SEND WIC:OK^\n\r"));
    }
    
    WIO = 0; wifi_icon(0);
    //while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
    BC127.print(F("SEND WIC:OK^\n\r"));
    return;
  //} 
}
*/
/*
void startBluetoothScan()
{
  detach_interrupts();
  //BC127.print(F("SEND BTC:2^\n\r")); BC127.flush();
  print_info(F("AUDIO LINK SCAN"));
  for (int p = 0; p < 15; p++)
  {
    btscan[p][0] = F("");
    btscan[p][1] = F("");
    btscan[p][2] = F("");
  }
  uint8_t btcount = -1;
  BC127.print(F("INQUIRY 10\r")); BC127.flush();
  print_stream(F("Searching"), F("Please wait..."));

  int start_time = millis(); int elapsed = 0;
  while (1)
  {  
    elapsed = millis() - start_time;
    if (elapsed > 10500)
    {
      break;
    }
    
    if (BC127.getCommandCount())
    {
      memset(buffer2, 0, sizeof(buffer2));
      BC127.pullCommand(buffer2);
      String process = (char*)buffer2;
      memset(buffer2, 0, sizeof(buffer2));
      if (process.startsWith(F("INQUIRY ")))
      {
        
        btscan[btcount][0] = process.substring(8, 20);
        btscan[btcount][2] = process.substring(29, 31);
        process = F("");
        btcount++;
        if (btcount > 14)
        {
          btcount = 14; break;
        }
      }
      else
      {
        process = F("");
      }
    }   
  }
  
  if (btcount == -1)
  {
    print_info(F("TO CONNECT AUDIO")); print_stream(F("Settings->Bluetooth"), F("")); delay(2850); 
    clear_info(); clear_stream();
    monthsLast = -1;
    update_clock(); 
    detach_interrupts();
    init_interrupts();
    
    if (EEPROM.read(190) != '1')
    {
      EEPROM.write(190, '1');
    }
    return;
  }

  for (int c = 0; c < btcount; c++)
  {
    BC127.print(F("NAME ")); BC127.print(btscan[c][0]); BC127.print(F("\r")); BC127.flush(); delay(150);
    String tmp = bc127_response();
    if ((tmp.startsWith(F("OK")) || (tmp == F(""))))
    {
      delay(150); tmp = bc127_response();
    }
    if ((tmp.startsWith(F("OK")) || (tmp == F(""))))
    {
      delay(150); tmp = bc127_response();
    }
    tmp.trim();
    if (tmp.startsWith(F("NAME ")))
    {
      int len = tmp.length();
      if (len <= 2)
      {
        btscan[c][1] = F("Unknown Device");
      }
      else
      {
        btscan[c][1] = tmp.substring(18, len);
      }
    }
    else
    {
      btscan[c][1] = F("Unknown Device");
    }
  }

  clear_info(); clear_stream(); int ios_id = 0;
  print_info(btscan[ios_id][1]); print_stream(F("<-PRESS F TO SELECT->"), F(""));
  detach_interrupts(); sub_interrupts(); delay(1000);
  
  
  start_time = millis(); elapsed = 0; int device_num = 0;
  while (click_f != 1)
  {
    elapsed = millis() - start_time;
    if (click_l == 1)
    {
      ios_id--;
      if (device_num < 0) ios_id = btcount;
      print_info(btscan[ios_id][1]);
      click_l = 0;
    }
    else if (click_f == 1) 
    {
      break;
    }
    else if (click_r == 1)
    {
      ios_id++;
      if (device_num > btcount) ios_id = 0;
      print_info(btscan[ios_id][1]);
      click_r = 0;
    }
    if (elapsed > 80000)
    {
      clear_stream();
      print_info(F("settings->bluetooth for audio"));
      print_stream(F("* data-only mode *"), F(""));
      delay(1000);
      clear_info(); clear_stream(); monthsLast = -1; update_clock();
      detach_interrupts(); init_interrupts();
      if (EEPROM.read(190) != '1')
      {
        EEPROM.write(190, '1');
      }
      return;
    }
  }
  click_f = 0; BTR = btscan[ios_id][0]; 
  clear_info(); clear_stream();
  print_info(F("READY TO PAIR"));
  print_stream(F("Select Your Glowdeck..."), F(""));
  BC127.print(F("OPEN ")); BC127.print(BTR); BC127.print(F(" HFP\r")); delay(500); bc127_response(); delay(2250); map_flag = 0;
  BTQ = F("OPEN ") + String(BTR) + F(" MAP");
  print_info(F("PAIR COMPLETE")); delay(650); print_stream(F("All Media Connected!"), F("")); delay(1500);
  
  clear_stream(); delay(250); print_stream(F("Welcome to Glowdeck"), F("")); delay(1100);
  clear_stream(); delay(150); clear_info();
  monthsLast = -1; update_clock(); 
  detach_interrupts(); init_interrupts(); 
  if (EEPROM.read(190) != '1')
  {
    EEPROM.write(190, '1');
  }
  return;
}
*/
/*
void gpio_press(int pio_id, int hold = 99)
{
  digitalWrite(pio_id, HIGH); delay(hold); digitalWrite(pio_id, LOW);
}
*/

















