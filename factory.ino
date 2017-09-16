void init_sys() {
  BC127.begin(9600); delay(5);
  //USR.begin(9600); delay(5);
  
  if (DEBUG == 1) {
    MCU.begin(9600); delay(5);
  }
  
  char sys_flag = EEPROM.read(50); delay(10);
  char dfu_flag = EEPROM.read(0); delay(10);
  char gfu_flag = EEPROM.read(199); delay(10);
  char crisis_flag = EEPROM.read(120); delay(10);
  char baud_flag = EEPROM.read(167); delay(10);
  //char auto_flag = EEPROM.read(121); delay(10);
  //char prv_flag = EEPROM.read(1); delay(10);

  // VERSION BC127...
  while (BC127.available()) {
    BC127.read();
  }
  BC127.resetCommandList();
  BC127.print("VERSION\r"); 
  BC127.flush();

  bool versionSuccess = false;

  unsigned long versionStart = millis();

  String msg;
  
  while (!versionSuccess) {
    
    if (BC127.getCommandCount()) {
      
      memset(buffer2, 0, sizeof(buffer2));
      
      while (BC127.getCommandCount()) {
      
        BC127.pullCommand(buffer2);
        
        msg = (char*)buffer2;
        
        memset(buffer2, 0, sizeof(buffer2));
        
        if (msg.contains("Melody Audio")) {
             
          if (msg.contains("6.1")) {
            
            nextGen = true;
            
            BT_PREFIX = 8;

            versionSuccess = true;

            break;
            
          }
          else {
            
            nextGen = false;
            
            BT_PREFIX = 9;

            versionSuccess = true;

            break;
            
          }
          
        }
     
        process = F("");
        
        if ((BC127.checkListOverflow()) || (BC127.checkCharOverflow())) BC127.resetCommandList();

        if (millis() - versionStart > 5000) {

          nextGen = false;

          versionSuccess = true;
          
          break;
          
        }
        
      }

      if (millis() - versionStart > 5000) {

        nextGen = false;

        versionSuccess = true;
        
        break;
        
      }

    }

  }

  while (BC127.available()) {
    BC127.read();
  }
  BC127.resetCommandList();

 
  if ((crisis_flag != '1') && (crisis_flag != '2')) { 
    PRV = 1; 
    if (EEPROM.read(1) != '1') {
      EEPROM.write(1, '1');
    }
    //usr_setup(); 
    crisis_flag = '1';
    sys_flag = '1';
  }
  
  if (digitalRead(PRESS_R) == LOW) {
    print_stream(F("bypassing restore"), F(""));
    if (sys_flag != '1')
    {
      EEPROM.write(50, '1'); delay(10); sys_flag = '1';
    }
    if (gfu_flag == '1')
    {
      EEPROM.write(199, (char)'0'); delay(10); gfu_flag = '0';
    }
    if ((dfu_flag == 13) || (dfu_flag == '13')) {
      EEPROM.write(0, (char)'0'); dfu_flag = '0';
    }
    return;
    
  }
  else if (digitalRead(PRESS_L) == LOW)
  {
    sys_flag = '1';
  }
  
  if (gfu_flag == '1')
  {
    while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
    int tmp_prv = PRV; PRV = 3;
    if (init_flag == true)
    {
      send_init(); PRV = tmp_prv;
    }
  }  
  
  //1. BC127 UPGRADE
  if (((sys_flag != '2') && (sys_flag != '1')) || (digitalRead(PRESS_F) == LOW))
  {
    print_stream(F("factory reset"), F("wait")); delay(500);
    COL[0] = 255; COL[1] = 48; COL[2] = 25; FastLED.showColor(CRGB(COL[0], COL[1], COL[2])); leds_icon(1);
    while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
    print_stream(F("upgrade bc127"), F("then reboot"));
    int inChar = -1; char dfu_cmd[] = "DFU\r"; int dfu_count = 0;
    
    while (1)
    {
      inChar = MCU.read(); 
      if (inChar != -1) 
      {
        BC127.write(inChar);
        if (dfu_count != -1) {
          if (dfu_cmd[dfu_count] == inChar) {
            dfu_count++;
            if (dfu_count == 4) {
              dfu_count = -1;
              sys_flag = '2'; EEPROM.write(50, '2');
              COL[0] = 0; COL[1] = 0; COL[2] = 0; FastLED.showColor(CRGB(COL[0], COL[1], COL[2])); leds_icon(0);
              leds[26] = CRGB(15, 255, 48); leds[27] = CRGB(15, 255, 48); leds[28] = CRGB(15, 255, 48); leds[29] = CRGB(15, 255, 48);
              FastLED.show(); COL[0] = 15; COL[1] = 255; COL[2] = 48; leds_icon(1);
              BC127.end();
              print_stream(F("reboot when upload done"), F(""));
              BC127.begin(115200, SERIAL_8E1);
            }
          } 
          else {
            dfu_count = 0;
          }
        }
      }
      inChar = BC127.read(); 
      if (inChar != -1) {
        MCU.write((char)inChar);
      }
      
      if (dfu_count != -1) {
        if (digitalRead(PRESS_R) == LOW) {
          if (digitalRead(PRESS_R) == LOW) {
            if (sys_flag != '2') {
              EEPROM.write(50, '2');
            }
            COL[0] = 0; 
            COL[1] = 255; 
            COL[2] = 255; 
            FastLED.showColor(CRGB(COL[0], COL[1], COL[2])); 
            leds_icon(1);
            sys_flag == 3;
            break;
          }
        }
      }
    }
  }

  if (sys_flag == '3') {
    print_stream(F("restart glowdeck"), F("")); COL[0] = 0; COL[1] = 0; COL[2] = 0; FastLED.showColor(CRGB(COL[0], COL[1], COL[2])); leds_icon(0);
    leds[26] = CRGB(143, 64, 15); leds[27] = CRGB(143, 64, 15); leds[28] = CRGB(143, 64, 15); leds[29] = CRGB(143, 64, 15); FastLED.show(); COL[0] = 143; COL[1] = 64; COL[2] = 15; leds_icon(1);
    while (1)
    {
      delay(600); COL[0] = 0; COL[1] = 0; COL[2] = 0; FastLED.showColor(CRGB(COL[0], COL[1], COL[2])); leds_icon(0); 
      delay(300); COL[0] = random(0, 255); COL[1] = random(0, 255); COL[2] = random(0, 255); leds[24] = CRGB(COL[0], COL[1], COL[2]); leds[25] = CRGB(COL[0], COL[1], COL[2]); leds[26] = CRGB(COL[0], COL[1], COL[2]); FastLED.show(); leds_icon(1);
    }
  }
    
  if ((sys_flag != '2') && (digitalRead(PRESS_L) == LOW)) {
    sys_flag = '2'; print_stream(F("system reset"), F("depress L")); delay(1000);
  }
  if (((sys_flag == '1') && (crisis_flag == '1')) || (gfu_flag == '1')) {
    sys_flag = '2'; print_stream(F("verifying glowdeck"), F("")); delay(1000); PRV = 1;
  }

  if (sys_flag == '2') {
    BTR = F("");
    for (int s = 20; s < 32; s++) {
      char btr_char = EEPROM.read(s);
      BTR += String(btr_char); delay(15);
    }
    if ((BTR.length() != 12) || (BTR == F("000000000000"))) {
      BTR = F("000000000000"); btr_flag = 0;
    }
    else {
      btr_flag = 1;
    }
 
    SID = get_sid(); MCU.println(SID);
    LNM = F("Glowdeck") + String(SID); LNM.trim();
    String nm_short = F("GD") + String(SID);


      
    if ((gfu_flag != '1') || (crisis_flag == '1'))
    {
      PRV = 1; ONB = 1;
      
      if (EEPROM.read(1) != (char)'0')
      {
        EEPROM.write(1, (char)'0'); delay(35);
      }
      String UID = F("00000");
      char SID_CHR[6]; char UID_CHR[6]; char BTR_CHR[13];
      BTR = F("000000000000");
      btr_flag = 0;
      SID.toCharArray(SID_CHR, 6); 
      UID.toCharArray(UID_CHR, 6); 
      BTR.toCharArray(BTR_CHR, 13);
      eeprom_write_string(2, SID_CHR); delay(100); 
      eeprom_write_string(7, UID_CHR); delay(100); 
      eeprom_write_string(20, BTR_CHR);

      print_stream(F("hardware reset"), F("wait"));  
      while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
      BC127.print("SET ENABLE_ANDROID_BLE=ON\r"); BC127.flush(); delay(300); bc127_response();
      BC127.print("SET SPP_TRANSPARENT=OFF\r"); BC127.flush(); delay(300); bc127_response();
      BC127.print("SET BLE_AUTO_ADV=ON\r"); BC127.flush(); delay(300); bc127_response();
      BC127.print("SET GPIOCONTROL=ON\r"); BC127.flush(); delay(300); bc127_response();
      BC127.print("SET ENABLE_LED=OFF\r"); BC127.flush(); delay(300); bc127_response();
      BC127.print("SET MM=1 1 3 0 0 0 0 0\r"); BC127.flush(); delay(300); bc127_response();
      BC127.print("SET REMOTE_ADDR=0\r"); BC127.flush(); delay(300); bc127_response();
      BC127.print("SET ENABLE_BATT_IND=OFF\r"); BC127.flush(); delay(300); bc127_response();
      BC127.print("SET WIRED=15 15 1 1\r"); BC127.flush(); delay(300); bc127_response();
      BC127.print("SET INPUT_GAIN=15\r"); BC127.flush(); delay(300); bc127_response();
      BC127.print("SET NAME_SHORT=GD" + String(SID) + "\r"); BC127.flush(); delay(300); bc127_response();
      BC127.print("SET CLASSIC_ROLE=0 0\r"); BC127.flush(); delay(300); bc127_response();
      BC127.print("SET AUTOCONN=0\r"); BC127.flush(); delay(150); bc127_response();
      BC127.print("SET DISCOVERABLE=2 0\r"); BC127.flush(); delay(300); bc127_response();
      BC127.print("SET NAME=" + String(LNM) + "\r"); BC127.flush(); delay(300); bc127_response();
      
      //***EXPERIMENTAL MFI CONFIG BLOCK***
      //BC127.print("SET BLE_ROLE=0\r"); BC127.flush(); delay(300); bc127_response();
      //BC127.print("SET HID_ROLE=0\r"); BC127.flush(); delay(300); bc127_response();
      /*
      BC127.print("SET ENABLE_IAP=ON\r"); BC127.flush(); delay(300);MCU.println(bc127_response());
      BC127.print("IAP ACCESSORY_NAME=MELODY\r"); BC127.flush(); delay(300); MCU.println(bc127_response());
      BC127.print("IAP MANUFACTURER_NAME=BLUE CREATION\r"); BC127.flush(); delay(300); MCU.println(bc127_response());
      BC127.print("IAP MODEL_NAME=MELODY AUDIO\r"); BC127.flush(); delay(300); MCU.println(bc127_response());
      BC127.print("IAP SERIAL_NO=V5.2\r"); BC127.flush(); delay(300); MCU.println(bc127_response());
      BC127.print("IAP SEED_ID=1234567890\r"); BC127.flush(); delay(300); MCU.println(bc127_response());
      BC127.print("IAP PROTOCOL_STRING=com.blue-creation.Melody\r"); BC127.flush(); delay(300); MCU.println(bc127_response());    
      BC127.print("IAP HARDWARE_VER=4 0 0\r"); BC127.flush(); delay(300); MCU.println(bc127_response());
      BC127.print("IAP FIRMWARE_VER=6 0 0\r"); BC127.flush(); delay(300); MCU.println(bc127_response());
      //BC127.print("SET DEVICE_ID=0204 0201 0202 0203 0305 0306 0307 0308\r"); BC127.flush(); delay(300); MCU.println(bc127_response());
      BC127.print("SET DEVICE_ID=0101 020 0303 0404 0505 0606 0707 0808\r"); BC127.flush(); delay(300); MCU.println(bc127_response());
      //BC127.print("SET ENABLE_IAP_V2=ON\r"); BC127.flush(); delay(300); MCU.println(bc127_response());
      BC127.print("WRITE\r"); BC127.flush(); delay(300); MCU.println(bc127_response());
      BC127.print("RESET\r"); BC127.flush(); delay(300); MCU.println(bc127_response());
      */
      //***EXPERIMENTAL MFI CONFIG BLOCK***
      
      
      /*  ***UNDO MFI BLOCK***
      BC127.print("SET ENABLE_IAP=OFF\r"); BC127.flush(); delay(300); bc127_response();
      BC127.print("SET ENABLE_IAP_V2=OFF\r"); BC127.flush(); delay(300); bc127_response();
      BC127.print("SET BLE_ROLE=1\r"); BC127.flush(); delay(300); bc127_response();
      */ //***UNDO MFI BLOCK***
      
      
      
      /*
      if (btr_flag == 1)
      {
        BC127.print("SET REMOTE_ADDR=" + String(BTR) + "\r"); BC127.flush(); delay(300); bc127_response();
      }
      else
      {
        BC127.print("SET REMOTE_ADDR=0\r"); BC127.flush(); delay(300); bc127_response();
      }
      */
      
      if ((gfu_flag != '1') && (EEPROM.read(133) != '1'))
      {
        BC127.print("UNPAIR\r"); BC127.flush(); delay(300); bc127_response();
        for (int g = 220; g < 232; g++)
        {
          EEPROM.write(g, (char)'0');
          EEPROM.write(g+12, (char)'0');
        }
        
        if (EEPROM.read(190) == '1')
        {
          EEPROM.write(190, (char)'0');
        }
        print_stream(F("device reset"), F("unpair phone")); delay(2500);
      }
      
      if (EEPROM.read(133) == '1')
      {
        EEPROM.write(133, '2');
      }
      
      BC127.print("WRITE\r"); BC127.flush(); delay(300); bc127_response();
      print_stream(F("restarting modules"), F(""));
      BC127.print("RESET\r"); delay(300);
      while (BC127.available()) { BC127.read(); } BC127.resetCommandList();      
    }
    
    sys_flag = '1'; EEPROM.write(50, '1'); delay(10);
    if (crisis_flag == '1')
    {
      crisis_flag = '2';
      EEPROM.write(120, '2'); delay(10);
    }
    if (gfu_flag == '1')
    {
      gfu_flag = '0';
      if (EEPROM.read(199) != (char)'0')
      { 
        EEPROM.write(199, (char)'0'); delay(10);
      }
    }
        
    PRV = 1;
    if (EEPROM.read(1) != '1')
    {
      delay(20); EEPROM.write(1, '1');
    }
    EEPROM.write(1, '1'); delay(10);
    
    if (EEPROM.read(198) == '1')
    {
      print_stream(F("update success"), F("")); delay(1625);
      return;
    }
    
    print_stream(F("power-cycle glowdeck"), F("")); COL[0] = 0; COL[1] = 0; COL[2] = 0; FastLED.showColor(CRGB(COL[0], COL[1], COL[2])); leds_icon(0);
    leds[26] = CRGB(143, 64, 15); leds[27] = CRGB(143, 64, 15); leds[28] = CRGB(143, 64, 15); leds[29] = CRGB(143, 64, 15); FastLED.show(); COL[0] = 143; COL[1] = 64; COL[2] = 15; leds_icon(1);
    while (1)
    {
      delay(600); COL[0] = 0; COL[1] = 0; COL[2] = 0; FastLED.showColor(CRGB(COL[0], COL[1], COL[2])); leds_icon(0); 
      delay(300); COL[0] = random(0, 255); COL[1] = random(0, 255); COL[2] = random(0, 255); leds[26] = CRGB(COL[0], COL[1], COL[2]); leds[27] = CRGB(COL[0], COL[1], COL[2]); leds[28] = CRGB(COL[0], COL[1], COL[2]); leds[29] = CRGB(COL[0], COL[1], COL[2]); 
      FastLED.show(); leds_icon(1);
    }
  }
  
  
  if (gfu_flag == '1')
  {
    if (crisis_flag == '1')
    {
      crisis_flag = '2';
      EEPROM.write(120, '2'); delay(15);
    }
    if (EEPROM.read(50) != '1')
    {
      delay(10);
      EEPROM.write(50, '1'); sys_flag = '1'; delay(15);
    }
    
    gfu_flag = '0';
    EEPROM.write(199, (char)'0');
    PRV = 1;
    print_stream(F("power-cycle glowdeck"), F("")); 
    
    while (1)
    {
      for (int j = 0; j < 255; j++)
      {
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1; /* SPEED */
        FillLEDsFromPaletteColors(startIndex);
        FastLED.show();
        COL[0] = leds[28].r; COL[1] = leds[28].g; COL[2] = leds[28].b; leds_icon(1);
        FastLED.delay(1000/100);
      }
    }
  }
  
}

void force_factory()
{
  SID = get_sid(); //MCU.println(SID);
  LNM = F("Glowdeck") + String(SID); LNM.trim();
  String nm_short = F("GD") + String(SID);
    
  PRV = 1; ONB = 1;
  EEPROM.write(1, '1'); delay(35); 
  String UID = F("00000");
  char SID_CHR[6]; char UID_CHR[6]; char BTR_CHR[13];
  BTR = F("000000000000");
  btr_flag = 0;
  SID.toCharArray(SID_CHR, 6); 
  UID.toCharArray(UID_CHR, 6); 
  BTR.toCharArray(BTR_CHR, 13);
  eeprom_write_string(2, SID_CHR); delay(100); 
  eeprom_write_string(7, UID_CHR); delay(100); 
  eeprom_write_string(20, BTR_CHR);

  print_stream(F("configuring bt..."), F("wait"));  
  while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
  BC127.print("SET ENABLE_ANDROID_BLE=ON\r"); BC127.flush(); delay(300); bc127_response();
  BC127.print("SET BLE_AUTO_ADV=ON\r"); BC127.flush(); delay(300); bc127_response();
  BC127.print("SET GPIOCONTROL=ON\r"); BC127.flush(); delay(300); bc127_response();
  BC127.print("SET ENABLE_LED=OFF\r"); BC127.flush(); delay(300); bc127_response();
  BC127.print("SET MM=1 1 3 0 0 0 0 0\r"); BC127.flush(); delay(300); bc127_response();
  BC127.print("SET REMOTE_ADDR=0\r"); BC127.flush(); delay(300); bc127_response();
  BC127.print("SET ENABLE_BATT_IND=OFF\r"); BC127.flush(); delay(300); bc127_response();
  BC127.print("SET WIRED=15 15 1 1\r"); BC127.flush(); delay(300); bc127_response();
  BC127.print("SET NAME_SHORT=GD" + String(SID) + "\r"); BC127.flush(); delay(200); bc127_response();
  BC127.print("SET CLASSIC_ROLE=0 0\r"); BC127.flush(); delay(150); bc127_response();
  BC127.print("SET DISCOVERABLE=2 0\r"); BC127.flush(); delay(300); bc127_response();
  BC127.print("SET AUTOCONN=1\r"); BC127.flush(); delay(150); bc127_response();
  BC127.print("SET NAME=" + String(LNM) + "\r"); BC127.flush(); delay(300); bc127_response();
  //BC127.print("UNPAIR\r"); BC127.flush(); delay(300); bc127_response();
  //print_stream(F("unpaired"), F("forget devices")); delay(2500);
  if (EEPROM.read(190) == '1')
  {
    EEPROM.write(190, '0');
  }
  BC127.print("WRITE\r"); BC127.flush(); delay(300); bc127_response();
  print_stream(F("restarting modules"), F(""));
  BC127.print("RESET\r"); delay(325);
  while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
  
  PRV = 1;
  if (EEPROM.read(1) != '1')
  {
    delay(20); EEPROM.write(1, '1');
  }
  print_stream(F("power-cycle glowdeck"), F("")); COL[0] = 0; COL[1] = 0; COL[2] = 0; FastLED.showColor(CRGB(COL[0], COL[1], COL[2])); leds_icon(0);
  leds[26] = CRGB(143, 64, 15); leds[27] = CRGB(143, 64, 15); leds[28] = CRGB(143, 64, 15); leds[29] = CRGB(143, 64, 15); FastLED.show(); COL[0] = 143; COL[1] = 64; COL[2] = 15; leds_icon(1);
  while (1)
  {
    delay(400); COL[0] = 0; COL[1] = 0; COL[2] = 0; FastLED.showColor(CRGB(COL[0], COL[1], COL[2])); leds_icon(0); 
    delay(200); COL[0] = random(0, 255); COL[1] = random(0, 255); COL[2] = random(0, 255); leds[26] = CRGB(COL[0], COL[1], COL[2]); leds[27] = CRGB(COL[0], COL[1], COL[2]); leds[28] = CRGB(COL[0], COL[1], COL[2]); leds[29] = CRGB(COL[0], COL[1], COL[2]); FastLED.show(); leds_icon(1);
  }
}

String get_sid()
{
  //LOCAL_ADDR=20FABB00018D
  char bChar; String addr = F(""); String result = F(""); String tmp;
  while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
  BC127.print(F("GET LOCAL_ADDR\r")); BC127.flush();
  //int st = millis();
  //LOCAL_ADDR=20FABB016006
  while (1)
  {
    if (BC127.available())
    {
      bChar = BC127.read();
      if (bChar == '\r')
      {
        break;
      }
      result += bChar;
    }
  }
  while (BC127.available()) { BC127.read(); } BC127.resetCommandList();
  
  int eqInd = result.indexOf(F("=")); int slen = result.length();
  if (eqInd != -1)
  {
    addr = result.substring(eqInd+8, slen); addr.trim();
    return addr;
  }
  else
  {
    return F("00000");
  }
}

void usr_atmode() {
  bool ATsuccess = false;
  int retries = 0; int bothbaud = 0;

  do {
    if (retries > 2) {
      if (bothbaud == 0) {
        USR.end(); USR.begin(9600);
        retries = 0; bothbaud = 1;
      }
      else {
        return;
      }
    }
    ATsuccess = startATSequence();
    retries += 1;
    
  } while (!ATsuccess);

  USR.print(F("a")); usr_response(0);
  USR.print(F("AT+E\r")); usr_response(0);
}

boolean startATSequence() {
  delay(50);
  while (USR.available()) { USR.read(); } 
  USR.write("+++");
  unsigned long timeout = millis() + (4 * 1000);
  while (!USR.available()) {
    delay(1);
    if (millis() > timeout) {
      return false;
    }
  }
  
  char resp = USR.read();

  if (resp == 'a') {
    return true;
  }

  return false;
}

void bc127_firmer() {
  int inChar = -1; char dfu_cmd[] = "DFU\r"; int dfu_count = 0; 
  
  inChar = MCU.read(); 
  if (inChar != -1) 
  {
    BC127.write(inChar);
   
    if (dfu_count != -1) 
    {
      if(dfu_cmd[dfu_count] == inChar) 
      {
        dfu_count++;
        if (dfu_count == 4) 
        {
          dfu_count = -1;
          BC127.end();
          BC127.begin(115200, SERIAL_8E1);
        } 
      }
      else 
      {
        dfu_count = 0;
      }
    }
  } 
  inChar = BC127.read(); 
  if (inChar != -1) 
  {
    MCU.write((char)inChar);
  }  
}

void force_provision()
{
  char EEP_CHR[13];
  String EEPString = F("1") + SID + F("00957") + F("0");
  EEPString.toCharArray(EEP_CHR, 12);
  eeprom_write_string(1, EEP_CHR);
  PRV = 1; 
  //UID = F("957"); 
  ONB = 0; //onboard = 0;
  MCU.println(F("provisioning complete"));
}

/*
void onboard()
{
    print_info(F("OK, LET'S GET YOU SETUP!")); delay(1050); clear_info(); delay(100);
    String device_type[3]; device_type[0] = F("iPhone"); device_type[1] = F("Android"); device_type[2] = F("Windows Phone");
    int device_num = 0;
    print_info(device_type[device_num]); print_stream(F("<- L/R SCROLLS | F PICKS ->"), F(""));
    
    int start_time = millis(); int elapsed = 0; int press_r = 0;
    while (press_f != 1)
    {
      elapsed = millis() - start_time;
      if (press_l == 1)
      {
        device_num--;
        if (device_num < 0) device_num = 2;
        print_info(device_type[device_num]);
        press_l = 0;
      }
      
      if (press_r == 1)
      {
        device_num++;
        if (device_num > 2) device_num = 0;
        print_info(device_type[device_num]);
        press_r = 0;      
      }
      
      if ((held_l == 1) || (held_r == 1) || (held_f == 1))
      {
        held_l = 0; held_r = 0; held_f = 0;
      }

      if ((elapsed > msg_limit) && (msg_flag == 0))
      {
        print_stream(F("<- CLICK LEFT/RIGHT TO NAVIGATE ->"), F("")); msg_flag = 1;
      }
      if ((elapsed > flash_limit) && (flash_flag == 0))
      {
        print_stream(F("<- HOLD FRONT TO EXIT ->"), F("")); flash_flag = 1;
      }  
  
      if (elapsed > 80000)
      {
        clear_stream();
        print_info(F("We'll do this next time..."));
        print_stream(F("* Exiting Setup *"), F(""));
        fade_out(5);
        inboard = 0; onboard = 0;
        delay(2250);
        clear_info(); clear_stream(); update_clock();
        return;
      }
    }
    
    held_r = 0; press_r = 0; held_f = 0; press_f = 0; held_l = 0; press_l = 0; 
    clear_info(); clear_stream();
    if (device_num != 0)
    {
      String device_err = F("We're working on the ") + device_type[device_num] + F(" app.");
      
      print_info(device_err); delay(200); print_stream(F("Check soon for an update!"), F("")); delay(1050); clear_info(); delay(100);
      inboard = 0; onboard = 0; update_clock(); fade_out(1); return;
    }
    else
    {
      print_info(F("LET'S SETUP YOUR iPHONE")); delay(1050); clear_info(); clear_stream(); delay(150);
      print_info(F("OPEN SETTINGS->BLUETOOTH")); print_stream(F("Press front when there..."), F(""));
      
      start_time = millis(); elapsed = 0;
      while (press_f != 1)
      {
        elapsed = millis() - start_time;
  
        if ((press_l == 1) || (press_r == 1) || (held_l == 1) || (held_r == 1) || (held_f == 1))
        {
          press_l = 0; press_r = 0; held_l = 0; held_r = 0; held_f = 0;
        }
        if ((elapsed > msg_limit) && (msg_flag == 0))
        {
          print_stream(F("LOOK FOR THE GEAR ICON"), F("")); msg_flag = 1;
        }
        if (elapsed > 180000)
        {
          clear_stream();
          print_info(F("We'll do this next time..."));
          print_stream(F("* Exiting Setup *"), F(""));
          fade_out(5);
          inboard = 0; onboard = 0;
          delay(1000);
          clear_info(); clear_stream(); update_clock();
          return;
        }
      }
      
      held_r = 0; press_r = 0; held_f = 0; press_f = 0; held_l = 0; press_l = 0; 
      clear_info(); clear_stream();
      print_info(F("LOOKING FOR YOUR iPHONE")); 
      for (int p = 0; p < 15; p++)
      {
        btscan[p][0] = F("");
        btscan[p][1] = F("");
        btscan[p][2] = F("");
      }
      btcount = -1;
      BC127.print(F("INQUIRY 10\r")); BC127.flush();
      print_stream(F("SCANNING"), F("Please be patient..."));
      
      start_time = millis(); elapsed = 0;
      while (1)
      {  
        elapsed = millis() - start_time;
        if (elapsed > 10000)
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
            btcount++;
            if (btcount > 14)
            {
              btcount = 14; break;
            }
            btscan[btcount][0] = process.substring(8, 20);
            btscan[btcount][2] = process.substring(29, 31);
            process = F("");
          }
          else
          {
            process = F("");
          }
        }   
      }
      
      if (btcount == -1)
      {
        print_info(F("NO DEVICES FOUND!")); delay(2000); clear_info(); clear_stream(); fade_out(1);
        onboard = 0; inboard = 0; update_clock(); 
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
      print_info(btscan[ios_id][1]); print_stream(F("<-PRESS F ON YOUR PHONE->"), F(""));
      
      start_time = millis(); elapsed = 0;
      while (press_f != 1)
      {
        elapsed = millis() - start_time;
        if (press_l == 1)
        {
          ios_id--;
          if (device_num < 0) ios_id = btcount;
          print_info(btscan[ios_id][1]);
          press_l = 0; held_l = 0;
        }
        else if (press_r == 1)
        {
          ios_id++;
          if (device_num > btcount) ios_id = 0;
          print_info(btscan[ios_id][1]);
          press_r = 0; held_r = 0;
        }
        else if ((held_l == 1) || (held_r == 1) || (held_f == 1))
        {
          held_l = 0; held_r = 0; held_f = 0;
        }
        if (elapsed > 120000)
        {
          clear_stream();
          print_info(F("We'll do this next time..."));
          print_stream(F("* Exiting Setup *"), F(""));
          fade_out(5);
          inboard = 0; onboard = 0;
          delay(1000);
          clear_info(); clear_stream(); update_clock();
          return;
        }
      }
      
      press_f = 0; BTR = btscan[ios_id][0]; 
      MCU.print(F("User selected: ")); MCU.print(btscan[ios_id][1]); MCU.print(F(" (")); MCU.print(btscan[ios_id][0]); MCU.println(F(")"));
      clear_info(); clear_stream();
      print_info(F("GOT IT! NOW PAIR."));
      print_stream(F("Click Glowdeck from the list..."), F(""));
      BC127.print(F("SET REMOTE_ADDR=")); BC127.print(BTR); BC127.print(F("\r")); delay(200); bc127_response(); 
      BC127.print(F("SET AUTOCONN=2\r")); BC127.flush(); delay(200); bc127_response(); delay(200); BC127.print(F("WRITE\r")); bc127_response(); delay(300);
      BC127.print(F("OPEN ")); BC127.print(BTR); BC127.print(F(" HFP\r")); bc127_response(); delay(2250); bc127_response();
      BC127.print(F("OPEN ")); BC127.print(BTR); BC127.print(F(" MAP\r")); bc127_response();
      print_info(F("PERFECT!")); print_stream(F("LAUNCH STREAMS TO PROVISION..."), F("")); delay(1500);
      clear_stream(); delay(150); clear_info();
      inboard = 0; onboard = 0; EEPROM.write(12, '0'); update_clock(); return;
    }
    inboard = 0; onboard = 0; fade_out(1);
    return;
  } 
}
*/


















