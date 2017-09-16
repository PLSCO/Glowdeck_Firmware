void init_settings() {
  // while (USR.available()) { USR.read(); }
  //while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
  //BC127.print(F("SET BLE_AUTO_ADV=ON\r")); BC127.flush(); delay(5);
  //while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
  // BC127.print("SET AUTOCONN=2\r"); BC127.flush(); delay(5);
  //while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
  //BC127.print(F("SET DISCOVERABLE=1 0\r")); BC127.flush(); delay(15);
  while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
  // BC127.print(F("WRITE\r")); BC127.flush(); delay(15);
  // while (BC127.available()) { BC127.read(); } BC127.resetCommandList();

  int anmComp = 3500;

  anmDelays[0] = 17250 + anmComp;
  anmDelays[1] = 20000 + anmComp;
  anmDelays[2] = 45000 + anmComp;
  anmDelays[3] = 26000 + anmComp;
  anmDelays[4] = 35000 + anmComp;
  anmDelays[5] = 30000 + anmComp;
  anmDelays[6] = 40000 + anmComp;

  equalizers[0] = F("flat");
  equalizers[1] = F("bass boost");
  equalizers[2] = F("treble boost");
  equalizers[3] = F("rock");
  equalizers[4] = F("jazz");
  
  //if (DEBUG == 1) {
  //  MCU.print(F("initializing glowdeck | v0.")); 
  //  MCU.println(VER);
  //}
  
  for (int i = 1; i < 13; i++) {
    char eepChar = EEPROM.read(i); delay(25);
    EEP[i] = eepChar; //eepString += eepChar;

    if ( (i == 1) && ((eepChar != '0') && (eepChar != '1')) )  // EEPROM NEVER FORMATTED, SET ALL TO 0
    {
      for (int r = 1; r < 13; r++) {
        delay(25);
      }
      eepChar = '0';
      PRV = 1; 
      ONB = 1;
    }
    
    if (i == 1) {
      if (eepChar == '1') {
        PRV = 1;
        REG = 1;
      }
      else {
        PRV = 1;
      }
    }
    
    if (i == 12) {
      if (eepChar == '1') {
        ONB = 1; //onboard = 1;
        EEP[12] = '1';
      }
      else {
        ONB = 0; //onboard = 0;
        EEP[12] = '0';
      }
    }
  }
  
    PRV = 1; // (String(EEP[1])).toInt();
    //if (DEBUG == 1) {
    //  MCU.print(F("PRV: ")); MCU.println(PRV);
    //}
    
    if (PRV == 1) {
      REG = 1;
    }
    
    int uid_begin = 0; String UID = F("");
    for (int p = 7; p < 12; p++)
    {
      if (String(EEP[p]) != F("0"))
      {
        uid_begin = 1;
      }
      if (uid_begin == 1)
      {
        UID += String(EEP[p]);
      }
    }
    
    if (DEBUG == 1) {
      MCU.print(F("UID: ")); 
      if (uid_begin == 0)
      {
        MCU.println(F("00000"));
      }
      else
      {
        MCU.println(UID);
      }
    }
    
    SID = String(EEP[2]) + String(EEP[3]) + String(EEP[4]) + String(EEP[5]) + String(EEP[6]);
    //MCU.print(F("SID: ")); MCU.println(SID);
       
    //BTA = bt_addr(); BTA.trim();
    BTR = F("");
    for (int s = 20; s < 32; s++) {
      char btr_char = EEPROM.read(s);
      BTR += String(btr_char);
    }
   
    if ((BTR != F("000000000000")) && (BTR.length() == 12)) {
      btr_flag = 1;
    }
    else {
      BTR = F("000000000000");
      btr_flag = 0;
    }
    
    BTR = bt_remote(); BTR.trim();
    if (DEBUG == 1) {
      MCU.print(F("BTR: ")); MCU.println(BTR);
    }

    if ((BTR != F("000000000000")) && (BTR.length() == 12)) {
      btr_flag = 1;
    }
    else {
      BTR = F("000000000000");
      btr_flag = 0;
    }
    
    LNM = bt_name(); LNM.trim();
    //MCU.print(F("LNM: ")); MCU.println(LNM);

    BC127.print("SET SPP_TRANSPARENT=OFF\r"); BC127.flush(); delay(300); bc127_response();
    BC127.print("WRITE\r"); BC127.flush(); delay(50); bc127_response();

}

void init_adc() {
  pinMode(AUDIO_INPUT_PIN, INPUT); 
  pinMode(AUDIO_INPUT_PIN2, INPUT);
  analogReadResolution(ANALOG_READ_RESOLUTION); 
  analogReadAveraging(ANALOG_READ_AVERAGING); 
  memset(commandBuffer, 0, sizeof(commandBuffer));
  spectrumSetup();
  samplingBegin();
}

void init_usb() {
  pinMode(USB_IN, INPUT); 
  digitalRead(USB_IN);
}

void update_usb() {
  if (digitalRead(USB_IN) == LOW) {
    clear_stream(); print_stream(F("Entering DFU..."), F("3"));
    delay(1000);
    if (digitalRead(USB_IN) == LOW) {
      print_stream(F("Entering DFU..."), F("2"));
    }
    else {
      clear_stream(); return;
    }
    delay(1000);
    if (digitalRead(USB_IN) == LOW) {
      print_stream(F("Entering DFU..."), F("1"));
    }
    else {
      clear_stream(); return;
    }
    delay(1000);
    if (digitalRead(USB_IN) == LOW) {
      print_stream(F("Entering DFU..."), F("0"));
      delay(250); dfu_mode(); return;
    }
    else {
      clear_stream(); return;
    }
  }
}

void init_audio() {
  pinMode(AMP, OUTPUT); 
  digitalWrite(AMP, LOW); 
  SND = 0;
}

void init_interrupts() {
  attachInterrupt(PRESS_F, ISR_F, FALLING);
  attachInterrupt(PRESS_L, ISR_L, FALLING);
  attachInterrupt(PRESS_R, ISR_R, FALLING);
  digitalWrite(PRESS_F, HIGH); 
  digitalWrite(PRESS_L, HIGH);
  digitalWrite(PRESS_R, HIGH);
}

void sub_interrupts() {
  attachInterrupt(PRESS_F, ISRSUB_F, FALLING); delay(10);
  attachInterrupt(PRESS_L, ISRSUB_L, FALLING); delay(10);
  attachInterrupt(PRESS_R, ISRSUB_R, FALLING); delay(10);
  digitalWrite(PRESS_F, HIGH); delay(10);
  digitalWrite(PRESS_L, HIGH); delay(10);
  digitalWrite(PRESS_R, HIGH); delay(10);
}

void detach_interrupts() {
  detachInterrupt(PRESS_F); 
  detachInterrupt(PRESS_L); 
  detachInterrupt(PRESS_R);
  
  if (MSA == 1) {
    MSA = 2;
    music_sync_timer.end();
  }
  if (ANA == 1) {
    ANA = 2;
    animationTimer.end();
  }
}

void reattach_interrupts() {
  digitalWrite(PRESS_F, HIGH);
  digitalWrite(PRESS_L, HIGH); 
  digitalWrite(PRESS_R, HIGH);
  attachInterrupt(PRESS_F, ISR_F, FALLING); 
  attachInterrupt(PRESS_L, ISR_L, FALLING); 
  attachInterrupt(PRESS_R, ISR_R, FALLING);

  if ((MSA == 2) && (MSL == 1)) {
    music_sync_timer.begin(music_sync, 12500);
    MSA = 1;
  }
  else if (MSL == 0) {
    MSA = 0; 
  }

  if ((ANA == 2) && (ANM != -1)) {
    animationTimer.begin(ledsAnimate, anmDelays[ANM]);
    ANA = 1;
  }
  else if (ANM == -1) {
    ANA = 0;
  }
}

void dfu_mode() {
  if ((MSL == 1) || (MSA != 0)) {
    MSL = 0;
    MSA = 0;
    music_sync_timer.end();
  }
  if ((ANM != -1) || (ANA != 0)) {
    ANM = -1;
    ANA = 0;
    animationTimer.end();
  }  
  
  char gf_flag = '1';
    
  if (EEPROM.read(133) != gf_flag) EEPROM.write(133, gf_flag);
  delay(10);
  
  EEPROM.write(0, 13);
  delay(25);
  
  #define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
  #define CPU_RESTART_VAL 0x5FA0004
  #define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);
  
  BC127.flush(); 
  MCU.flush();
  while (MCU.available()) { MCU.read(); }
  BC127.resetCommandList();
  while (BC127.available()) { BC127.read(); }
  BC127.resetCommandList();

  CPU_RESTART;
  
}

void init_containers() {
  personal[0] = F("X");
  personal[1] = F("");
  personal[2] = F("");
  personal[3] = F("");
  personal[4] = F("");
  personal[5] = F("");
  
  bleAddr = F(""); 
  btcAddr = F("");
  for (int u = 0; u < 12; u++) {
    bleAddress[u] = (char)'0';
    btcAddress[u] = (char)'0';
  }
  
  for (int g = 220; g < 232; g++) {
    bleAddress[g-220] = EEPROM.read(g);
    bleAddr += bleAddress[g-220]; delay(5);
    btcAddress[g-220] = EEPROM.read(g+12);
    btcAddr += btcAddress[g-220]; delay(5);
  }
}

void power_off() {
  clear_info(); 
  print_info(F("* going to sleep *")); 
  info_flag = true;
  delay(275);
  
  // clear_stream(); 
  // print_stream(F("* press front to wake *"), F("")); 
  // delay(1200);
 
  delay(800);
  // CHG = 0; 
  // digitalWrite(COIL_PW, LOW);
  SND = 0; 
  digitalWrite(AMP, LOW);

  if (MSL == 1) {
    MSL = 0;
    MSA = 0;
    music_sync_timer.end();
  }
  if (ANM != -1) {
    ANM = -1;
    ANA = 0;
    animationTimer.end();
  }

  if ((HUE != 999) || ((COL[0] + COL[1] + COL[2] > 0))) {
    fadeout_leds(); 
    COL[0] = 0; COL[1] = 0; COL[2] = 0; 
  }
  HUE = 999;
  NGT = 1; 
  LED = 0; 
  LSW = 0;
  
  FastLED.clear();
  
  int i;
  int c = analog_convert(DBR);
  DBA = 0;
  DBR = 0; 
  for (i = c; i > -1; i--) {
    analogWrite(LCD_BL, i); 
    delay(2);
  }
  POW = 0;
  return;
}

void power_on() {
  clear_info(); 
  print_info(F("* waking up *")); 
  info_flag = true;
 
  // CHG = 1; 
  // digitalWrite(COIL_PW, HIGH);
  //SND = 0; digitalWrite(AMP, LOW);

  HUE = 999;
  ANM = -1;
  ANA = 0;
  MSL = 0;
  MSA = 0;
  NGT = 0; 
  LED = 1; 
  LSW = 1;
  FastLED.clear();
  
  int i;
  DBR = 10; 
  int c = analog_convert(DBR);
  c = analog_convert(DBR);
  for (i = 0; i < c; i++) {
    analogWrite(LCD_BL, i); 
    delay(4);
  }
  POW = 1;
  return;
}

















