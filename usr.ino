/*
void init_usr() {
  while (USR.available()) { USR.read(); }
  USR.print(F("AT+E\r")); usr_response(0);
  while (USR.available()) { USR.read(); }

  //GIVE USR GLOWDECK MODEL AND UDID VALUES FOR WEB CLIENT
  USR.print(F("AT+GDMODEL=Beta\r"));
  usr_response(0);
  USR.print("AT+GDUDID=GD" + SID + "\r");
  usr_response(0);
  USR.print("AT+GDLGN=" + LNM + "\r");
  usr_response(0);
  USR.print(F("AT+GDSSID="));
  if ((WID != F("0")) && (WID != F("MyRouter"))) {
     USR.print(WID); 
  }
  else {
    USR.print(F("MyRouter"));
  }
  USR.print(F("\r")); 
  usr_response(0);
  
  USR.print(F("AT+TCPDISB=off\r")); 
  usr_response(0);
  
  // SET HTTP COMMAND CONSTANTS HERE
  USR.print(F("AT+TCPDIS=off\r"));
  usr_response(0);
  USR.print(F("AT+HTTPURL=streams.io,443\r"));
  usr_response(0);  //CLEAR RESPONSE (Expecting: +ok)
  USR.print(F("AT+HTTPTP=POST\r"));
  usr_response(0);  //CLEAR RESPONSE (Expecting: +ok)
  USR.print(F("AT+HTTPCN=keep-alive\r"));
  usr_response(0);  //CLEAR RESPONSE (Expecting: +ok)
  USR.print(F("AT+HTTPUA=lwip1.3.2\r"));  //TO DO: CHANGE UA TO UDID
  usr_response(0);  //CLEAR RESPONSE (Expecting: +ok)
  USR.print(F("AT+TCPDIS=on\r"));
  usr_response(0);
  
  //usr_ssid(); 
  //usr_ip();
  
}

void usr_config() { 
  //USR.print(F("AT+E\r"));
  //usr_response(0);
  //USR.print(F("AT+WSLK\r"));
  //usr_response(0);
  
  //if (rspn.length() <= 2)
  //{
  //  baud_change = 1;
  //  USR.end(); USR.begin(115200); delay(500);
  //  while (USR.available()) { USR.read(); }
  //  USR.print(F("AT+E\r")); usr_response(0);
  //}
   
  //CREATE TEMPORARY NAME STRING
  String tmp_name = F("Glowdeck") + SID;
  
  //SET MODULE LANGUAGE TO ENGLISH
  USR.print(F("AT+PLANG=EN\r"));
  usr_response(0);  //CLEAR RESPONSE (Expecting: +ok)
 
  //SET MODULE ID TO "Glowdeck + SID" (Ex: Glowdeck15FC8)
  //USR.print(F("AT+WRMID=")); USR.print(tmp_name); USR.print(F("\r"));
  //usr_response(0);  //CLEAR RESPONSE (Expecting: +ok)

  //SET MODULE AP TO "Glowdeck + SID" (Ex: Glowdeck15FC8),
  //AND SET MODULE AP TO 11BGN, CH1
  USR.print("AT+WAP=11BGN," + LNM + ",AUTO\r");
  usr_response(0);  //CLEAR RESPONSE (Expecting: +ok)

  //SET MODULE AP USERNAME AND PASSWORD TO EMPTY/EMPTY
  // empty not allowed, make u/p "glowdeck/glowdeck" for now!
  USR.print(F("AT+WEBU=none\r"));
  usr_response(0);  //CLEAR RESPONSE (Expecting: +ok)

  //SET MODULE TO TCP CLIENT MODE
  //SET MODULE TO CONNECT TO OUR SERVER (glowdeck.com:80)
  USR.print(F("AT+NETP=TCP,CLIENT,443,streams.io\r"));
  usr_response(0);  //CLEAR RESPONSE (Expecting: +ok)

  //SET MODULE TCP TIMEOUT TO 500MS
  USR.print(F("AT+TCPTO=600\r")); 
  usr_response(0);  //CLEAR RESPONSE (Expecting: +ok)

  //SET MODULE TO DUAL AP+STA MODES
  USR.print(F("AT+WMODE=APSTA\r"));
  usr_response(0);  //CLEAR RESPONSE (Expecting: +ok)
  
  USR.print(F("AT+WSSSID=MyRouter\r")); // + tmp_name + F("\r"));
  usr_response(0);
  
  USR.print(F("AT+TCPDISB=off\r")); 
  usr_response(0);

  USR.print(F("AT+HTTPURL=glowdeck.com,80\r"));
  usr_response(0);  //CLEAR RESPONSE (Expecting: +ok)
  USR.print(F("AT+HTTPTP=POST\r"));
  usr_response(0);  //CLEAR RESPONSE (Expecting: +ok)
  USR.print(F("AT+HTTPCN=keep-alive\r"));
  usr_response(0);  //CLEAR RESPONSE (Expecting: +ok)
  USR.print(F("AT+HTTPUA=lwip1.3.2\r"));  //TO DO: CHANGE UA TO UDID
  usr_response(0);  //CLEAR RESPONSE (Expecting: +ok)
  
  
  //SET MODULE UART TO 9600 BAUD
  USR.print(F("AT+UART=9600,8,1,NONE,NFC\r")); 
  usr_response(0);  //CLEAR RESPONSE (Expecting: +ok)

  //SET MODULE TO AUTO-SWITCH TO AP MODE ON BAD/NO STA CONNECTION
  USR.print(F("AT+MDCH=auto\r"));
  usr_response(0);  //CLEAR RESPONSE (Expecting: +ok)

  //MAKE NEW SETTINGS MODULE FACTORY DEFAULT SETTINGS
  USR.print(F("AT+CFGTF\r"));
  usr_response(0);  //CLEAR RESPONSE (Expecting: +ok)

  //RESET WIFI MODULE
  USR.print(F("AT+Z\r")); //delay(200);
  usr_response(0);
  delay(100);  //WAIT 100MS?

  //CLOSE UART AT 115200, OPEN NEW UART AT 9600
  //USR.end(); USR.begin(9600);
  //delay(450);  //WAIT 1150MS (IS THIS DELAY NECESSARY?)
  //REMOVE ANY LEFTOVER MODULE DATA FROM UART TO AVOID ERRORS
  //while (USR.available()) { USR.read(); }
  //RESET WIFI MODULE
  //USR.print(F("AT+E\r")); 
  //usr_response(0);
  init_usr();
}

int usr_setup() {
  int result = 0; //0 = DO NOT CONFIG, 1 = CONFIG, -1 = ERROR ENTERING CMD MODE
  bool usr_success = false;  //ENTER CMD MODE RESULT FLAG (=TRUE IF SUCCESS)
  int retries = 0;  //COUNTER FOR # OF FAILED ATTEMPTS TO INITIALIZE USR MODULE
  int bothbaud = 0;  //=0 IF WE DID NOT TRY 115200 YET, =1 IF WE'VE TRIED 115200
  char usr_done = '1';
  
  if (EEPROM.read(41) == usr_done) {
    result = 1;
    return result;
  }
  
  USR.begin(115200);
  
  delay(5);  
  print_stream(F("* starting wifi *"), F(""));
  
  int startClock = millis();
  do {
    if (retries > 3) {
      if (bothbaud == 0) {
        if (DEBUG == 1) MCU.println(F("USR Baud: 9600!"));
        USR.end(); 
        delay(5); 
        USR.begin(9600); 
        delay(5);
        retries = 0; 
        bothbaud = 1; 
        result = 1;
      }
      else {
        if (DEBUG == 1) MCU.println(F("USR CMD Mode Fail!"));
        USR.end(); 
        result = -1;
        return result;
      }
    }
    
    if (millis() - startClock > 6500) {
      USR.end(); result = -1;
      return result;
    }
    
    usr_success = usr_cmdmode();
    retries += 1;
  } while (!usr_success);

  USR.print(F("a")); usr_response(0);
  USR.print(F("AT+E\r")); usr_response(0);
  delay(75);

  while (USR.available()) { USR.read(); }
  USR.print(F("AT+PLANG=EN\r")); delay(50);
  while (USR.available()) { USR.read(); }
  USR.print(F("AT+WMODE=STA\r")); delay(50);
  while (USR.available()) { USR.read(); }
  USR.print(F("AT+MDCH=Off\r")); delay(50);
  while (USR.available()) { USR.read(); }    
  if (bothbaud != 1) {
    USR.print(F("AT+UART=9600,8,1,NONE,NFC\r")); 
    delay(50);
  }
  
  //while (USR.available()) { USR.read(); }
  //baud_flag = '1'; EEPROM.write(167, baud_flag);

  USR.print(F("AT+WIFI=DOWN\r")); 
  delay(50);
  USR.flush(); 
  USR.print(F("AT+Z\r")); 
  USR.end();
  EEPROM.write(41, usr_done);
  return result;
}

boolean usr_cmdmode() {
  //WAIT
  delay(75);  // WHAT DELAY IS NEEDED HERE IF ANY?
  //CLEAR ANY UNKNOWN BYTES FROM UART TO AVOID ERRORS
  while (USR.available()) { USR.read(); } 
  //FOLLOW USR INSTRUCTIONS TO ENTER COMMAND MODE
  USR.write("+++");
  //SET TIMEOUT TO 4000MS FOR CMD MODE ENTRY
  unsigned long timeout = millis() + (4000);
  //WAIT FOR MODULE TO RESPOND WITH "a" IF SUCCESS, 
  //IF CMD MODE FAILS, MODULE WILL NOT WORK UNTIL USER POWERS ON AGAIN!
  while (!USR.available()) {
    delay(1);
    if (millis() > timeout) { return false; }
  }
  char resp = USR.read();
  if (resp == 'a') {
    return true;
  }
  
  return false;
} 

String usr_shorten(String cmd) {
  String result = F("");
  cmd.replace(F("\r\n\r\n"), F("")); 
  cmd.trim(); 
  
  if ((cmd == F("+ok")) || (cmd == F("+ERR"))) {
    cmd = F("");
    return result;
  }
  if (cmd.startsWith(F("+ok="))) {
    int tmp_len = cmd.length(); 
    result = cmd.substring(4, tmp_len);
  }
  return result;
}

void usr_handler(String cmd) {
  cmd.replace(F("\r\n\r\n"), F("")); 
  cmd.trim(); 
  
  if ((cmd == F("+ok")) || (cmd == F("+ERR"))) {
    cmd = F(""); 
    return;
  }
  if (cmd.startsWith(F("+ok="))) {
    int tmp_len = cmd.length(); 
    cmd = cmd.substring(4, tmp_len);
  }
  
  if (DEBUG == 1) MCU.println(cmd);  // PRINT NORMALIZED RESPONSE FROM USR IN SERIAL MONITOR
  
  
  if (cmd.indexOf(F("Ch,SSID,BSSID,")) != -1) { //AT+WSCAN RESPONSE DATA (SSIDs AND AUTH/ENCRYPTION MODES)
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 4; j++) {
        wiscan[i][j] = F("");
      }
    }
    ssid_count = -1; return;
  }
  
  else if ((cmd.indexOf(F("WPA2PSK/AES")) != -1) || (cmd.indexOf(F("OPEN/NONE")) != -1) || (cmd.indexOf(F("WPAPSK/TKIPAES")) != -1) || (cmd.indexOf(F("WPAPSKWPA2PSK/TKIPAES")) != -1)) { // SSID IN-RANGE LINE
    ssid_log(cmd); 
    return;
  }
  
  else if (cmd.indexOf(F("RF Off")) != -1) { // USR MODULE OFF
    if (WIO == 1) {
      WIO = 0; wifi_icon(0);
    }
    WIO = 0; return;
  }   
  
  else if (cmd.indexOf(F("Disconnected")) != -1) { // NOT CONNECTED
    if (WIO == 1) {
      WIO = 0; wifi_icon(0);
    }
    WIO = 0;
    if (DEBUG == 1) MCU.println(F("STA LINK DISCONNECTED"));
    return;
  }
  
  else if (cmd.indexOf(WID + F("(")) != -1)  { // STA LINK SUCCESSFULLY ESTABLISHED
    if (WIO == 0)
    {
      WIO = 1; wifi_icon(1);
      USR.print(F("AT+WANN\r")); usr_handler(usr_response(0));
    }
    WIO = 1; 
    if (DEBUG == 1) MCU.println(F("STA LINK ESTABLISHED"));
    return;
    
  }
  
  
  //else if (cmd.indexOf(F("DHCP,")) != -1)
  //{
  //  int indA = cmd.indexOf(F(",")); int indB = cmd.indexOf(F(","), indA+1);
  //  if (indB != -1)
  //  {
  //    sta_ip = cmd.substring(indA+1, indB);
  //  }
  //  if (sta_ip == F("0.0.0.0"))
  //  {
  //    sta_ip = F("10.10.100.254");
  //  }
  //  MCU.print(F("STA_IP: ")); MCU.println(sta_ip); MCU.println();
  //  return;
  //}
  
  else if (cmd.endsWith(F("^")))
  {
    if (cmd.startsWith(F("LGN:")))
    { 
      String tmp_lnm = cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")));
      if (LNM != tmp_lnm)
      {
        LNM = tmp_lnm; tmp_lnm = F("");
        while (BC127.available()) { BC127.read(); }
        if ((BLE == 1) || (SPP == 1))
        {
          BC127.print(F("SEND LGN:OK^\n\r")); delay(100); bc127_response();
        }
        BC127.print(F("SET NAME=")); BC127.print(LNM); BC127.print(F("\r")); delay(150); bc127_response();
        BC127.print(F("WRITE\r")); delay(150); bc127_response(); 
        //BC127.print(F("RESET\r")); BC127.flush();
      }
      //if (PRV == 0)
      //{
      //  PRV = 1; EEPROM.write(1, '1');
      //}
      //USR.print(F("AT+GDLGN=") + LNM + F("\r")); usr_response(0);
      return;
    }
    
    else if (cmd.startsWith(F("SSID:"))) //USER SSID
    {
      WID = cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")));
      //USR.print(F("AT+GDSSID=") + WID + F("\r")); usr_response(0);
      return;
    }
    
    else if (cmd.startsWith(F("PASS:"))) //USER SSID
    { 
      WIP = cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")));
      wifi_status(); //sync_counter = 1;
      //while (USR.available()) { USR.read(); }
      //USR.print(F("AT+Z\r")); delay(1250); init_usr(); delay(600);
      //USR.print(F("AT+GDSSID=") + WID + F("\r")); usr_response(0);
      //USR.print(F("AT+GDKEY=") + WIP + F("\r")); usr_response(0);
      //USR.print(F("AT+GDLGN=") + LNM + F("\r")); usr_response(0);
      //USR.print(F("AT+Z\r")); delay(500);
      //TEST WIFI CONNECTION
      
      //USR.print(F("AT+WMODE=STA\r")); usr_response(0);
      //USR.print(F("AT+Z\r")); delay(500);
      //while (USR.available()) { USR.read(); }
      //USR.print(F("AT+E\r")); usr_response(0);
      //boolean wifi_success = false;
      //USR.print(F("AT+WSLK\r")); String resp = usr_response(0);
      //if (resp.indexOf(F("Disconnected")) != -1)
      //{
      //  int wifi_timeout = 6000; int wifi_start = millis();
      //  while (1)
      //  {
      //    USR.print(F("AT+WSLK\r")); String resp = usr_response(0);
      //    if ( (resp.indexOf(F("(")) != -1) && (resp.indexOf(F(":")) != -1) )
      //    {
      //      wifi_success = true;
      //    }
      //    if (millis() - wifi_start > wifi_timeout)
      //    {
      //      break;
      //    }
      //    delay(200);
      //  }
      //}
      //else if ( (resp.indexOf(F("(")) != -1) && (resp.indexOf(F(":")) != -1) )
      //{ 
      //  wifi_success = true;
      //}
      //
      //if (wifi_success == true)
      //{
      //  WIO = 1;
      //  wifi_icon(3);
      //}
      //else
      //{
      //  WIO = 1;
      //  wifi_icon(0);
      //  USR.print(F("AT+WMODE=AP\r")); usr_response(0);
      //  USR.print(F("AT+Z\r")); delay(100);
      //  while (USR.available()) { USR.read(); }
      //  //USR.print(F("AT+E\r")); usr_response(0);
      //}
      return;
    }
    
    else if (cmd.startsWith(F("UNM:")))  //STREAMS USERNAME
    { 
      //UNM = cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")));
      return;
    }

    else if (cmd.startsWith(F("PSW:")))  //STREAMS PASSWORD
    { 
      
      PSW = cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")));
      //SPECIAL CODE FOR MANUALLY ADDING UID
      if (PSW == F("!!!"))
      {
        if ((UNM.length() >= 1) && (UNM.length() <= 5))
        {
          UID = UNM; PRV = 1;
          EEPROM.write(1, '1');

          if (UNM.length() == 1)
          {
            UNM = F("0000") + UNM;
          }
          else if (UNM.length() == 2)
          {
            UNM = F("000") + UNM;
          }
          else if (UNM.length() == 3)
          {
            UNM = F("00") + UNM;
          }
          else if (UNM.length() == 4)
          {
            UNM = F("0") + UNM;
          }
          char UID_CHR[10];
          UID.toCharArray(UID_CHR, 6);
          eeprom_write_string(7, UID_CHR);
          print_info(F("Streams Enabled!"));
        }
      }
      else
      {   
        busy_icon(1);
        get_stream(F("a"));
        if ((UID != F("")) && (UID != F("0")) && (UID != F("00000")))
        {
          get_stream(F("p"));
          print_info(F("Streams Enabled!"));
          get_stream(F("w"));
        }
        busy_icon(0);
      }
      return;
    }

    else if (cmd.startsWith(F("DL:"))) //DISPLAY BRIGHTNESS SLIDER
    { 
      int user_val = (cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")))).toInt();
      
      if (user_val == 11) // USER ACTUALLY PRESSED DFU BUTTON
      {
        dfu_mode(); return;
      }
      else if ((user_val >= 0) && (user_val <= 10))
      {
        DBR = user_val;
      }
      else
      {
        DBR = 10;
      }
      analogWrite(LCD_BL, analog_convert(DBR)); 
      
      if (SPP == 1)
      {
        BC127.print(F("SEND DL:") + String(DBR) + F("^\n\r")); BC127.flush();
        if (DBA == 1)
        {
          BTQ = F("SEND LS:0^");
        }
      }
      else if (BLE == 1)
      {
        BC127.print(F("SEND DBR:") + String(DBR) + F("^\n\r")); BC127.flush();
        if (DBA == 1)
        {
          BTQ = F("SEND DBA:0^");
        }        
        
      }
      DBA = 0;
      return;
    }
    
    else if (cmd.startsWith(F("PB:"))) { // NIGHT MODE SWITCH
      int nt_mode = (cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")))).toInt();
      if (nt_mode == 1) {
        if (SND == 1) {
          digitalWrite(AMP, LOW); 
          SND = 0;
          leds_off();
        }
      }
      
      if (NGT == 0) {
        NGT = 1;
        if (BLE == 1) {
          BC127.print(F("SEND NGT:1^\n\r")); 
          BC127.flush();
        }
        return;
      }
      else if (NGT == 1) {
        NGT = 0;
        if (BLE == 1) {
          BC127.print(F("SEND NGT:0^\n\r")); 
          BC127.flush();
        }
        return;
      }
      return;
    }
 
    else if (cmd.startsWith(F("CLR:"))) { // COLOR PICKER COMMAND
      int ind_a = cmd.indexOf(F(":")); int ind_b = cmd.indexOf(F(","), ind_a+1); 
      int ind_c = cmd.indexOf(F(","), ind_b+1); int ind_d = cmd.indexOf(F("^"));
      if (ind_c != -1) {
        COL[0] = (cmd.substring(ind_a+1, ind_b)).toInt();
        COL[1] = (cmd.substring(ind_b+1, ind_c)).toInt();
        COL[2] = (cmd.substring(ind_c+1, ind_d)).toInt();
        FastLED.showColor(CRGB(COL[0], COL[1], COL[2]));
      }
      return;
    }
  }
}

void ssid_log(String cmd) {
  ssid_count++;
  if (ssid_count > 19) {
    ssid_count = 19; 
    return;
  }
  else {
    int a0; int a1; int b0; int b1; int c0; int c1;
    a0 = cmd.indexOf(',');
    a1 = cmd.indexOf(',', a0+1);
    
    if ((a0 == -1) || ((a1 - a0) <= 1)) { // EITHER NO COMMA DELIMITERS PRESENT OR SSID NAME IS 0 CHARS LONG SO SKIP
      ssid_count--; 
      return;
    }
    
    b0 = cmd.indexOf(',', a1+1); 
    b1 = cmd.indexOf(',', b0+1); 
    c0 = b1+1; 
    c1 = cmd.length();
    
    //[0] SSID
    wiscan[ssid_count][0] = cmd.substring(a0+1, a1);
    
    //[1] AUTHENTICATION TYPE | [2] ENCRYPTION MODE
    String auth_tmp = cmd.substring(b0+1, b1); 
    int auth_len = auth_tmp.length(); 
    int auth_div = auth_tmp.indexOf(F("/"));
    
    if (auth_div != -1) {  // FOUND DIV
      wiscan[ssid_count][1] = auth_tmp.substring(0, auth_div); //AUTHENTICATION TYPE
      wiscan[ssid_count][2] = auth_tmp.substring(auth_div+1, auth_len); //ENCRYPTION MODE
      
      // VALIDATION FOR MIXED MODE ENCRYPTION/AUTHENTICATION ROUTER MODES
      if (wiscan[ssid_count][1] == F("WPAPSKWPA2PSK")) {      
        wiscan[ssid_count][1] = F("WPA2PSK");
      }
      if (wiscan[ssid_count][2] == F("TKIPAES")) {
        wiscan[ssid_count][2] = F("AES");
      } 
    }
    else { // ERROR...MARK AS OPEN FOR NOW
      wiscan[ssid_count][1] = F("OPEN");
      wiscan[ssid_count][2] = F("NONE");
    }
    
    //[3] STORE SIGNAL STRENGTH
    wiscan[ssid_count][3] = cmd.substring(c0, c1);
    
    //[4] CLEAR PASSWORD ENTRY IF ONE EXISTS
    wiscan[ssid_count][4] = F("");
  }
}

void parse_ssid(String cmd) {
  int len = cmd.length();
  //CLEAR WISCAN ARRAY
  if (len < 10) {
    cmd = F(""); 
    return;
  }
  
  if (ssid_count != -1) {
    for (int r = 0; r < 15; r++) {
      for (int c = 0; c < 5; c++) {
        //wiscan[r][c] = F("");
      }
    }
    ssid_count = -1;
  }
  
  int aInd = 0;
  
  while (ssid_count < 15) {
    int nInd = cmd.indexOf('\n', aInd);
    if (nInd == -1) {
      break;  // No SSIDs (or no more)
    }
    else {
      String res = cmd.substring(aInd, nInd);
      if ((res.indexOf(F("WPA2PSK/AES")) != -1) || (res.indexOf(F("OPEN/NONE")) != -1) || (res.indexOf(F("WPAPSK/TKIPAES")) != -1) || (res.indexOf(F("WPAPSKWPA2PSK/TKIPAES")) != -1)) { // SSID IN-RANGE LINE
        ssid_count++;
        if (ssid_count > 19) {
          ssid_count = 19; 
          cmd = F(""); 
          return;
        }

        int a0; int a1; int b0; int b1; int c0; int c1; //int d0; int d1; int e0; int e1;
        a0 = res.indexOf(','); a1 = res.indexOf(',', a0+1);
        if ((a0 == -1) || ((a1 - a0) <= 1)) { // EITHER NO COMMA DELIMITERS PRESENT OR SSID NAME IS 0 CHARS LONG SO SKIP
          ssid_count--;
          cmd = F(""); 
          return;
        }
        b0 = res.indexOf(',', a1+1); 
        b1 = res.indexOf(',', b0+1); 
        c0 = b1+1; 
        c1 = res.length();
        
        //[0] SSID
        wiscan[ssid_count][0] = res.substring(a0+1, a1);
        
        //[1] AUTHENTICATION TYPE | [2] ENCRYPTION MODE
        String auth_tmp = res.substring(b0+1, b1); 
        int auth_len = auth_tmp.length(); 
        int auth_div = auth_tmp.indexOf(F("/"));
        
        if (auth_div != -1) { // FOUND DIV
          wiscan[ssid_count][1] = auth_tmp.substring(0, auth_div); //AUTHENTICATION TYPE
          wiscan[ssid_count][2] = auth_tmp.substring(auth_div+1, auth_len); //ENCRYPTION MODE
          
           //VALIDATION FOR MIXED MODE ENCRYPTION/AUTHENTICATION ROUTER MODES
          if (wiscan[ssid_count][1] == F("WPAPSKWPA2PSK")) {
            wiscan[ssid_count][1] = F("WPA2PSK");
          }
          if (wiscan[ssid_count][2] == F("TKIPAES")) {
            wiscan[ssid_count][2] = F("AES");
          }
        }
        else { // ERROR...MARK AS OPEN FOR NOW
          wiscan[ssid_count][1] = F("OPEN");
          wiscan[ssid_count][2] = F("NONE");
        }
        
        // [3] STORE SIGNAL STRENGTH
        wiscan[ssid_count][3] = res.substring(c0, c1);
        
        // [4] CLEAR PASSWORD ENTRY IF ONE EXISTS
        wiscan[ssid_count][4] = F("");
        
        // MOVE INDEX FORWARD
        aInd = nInd + 1; res = F(""); 
      }
      aInd = nInd + 1; res = F(""); continue;
      
    }
  }
  cmd = F("");
}

void wifi_connect(int router_id) {
  String response;
  while (USR.available()) { USR.read(); }
  USR.print(F("AT+WSSSID="));
  USR.print(wiscan[router_id][0]); 
  USR.print(F("\r")); 
  usr_response(0); 
  delay(50);
  
  USR.print(F("AT+WSKEY=")); 
  USR.print(wiscan[router_id][1]); 
  USR.print(F(",")); 
  USR.print(wiscan[router_id][2]); 
  USR.print(F(",")); 
  USR.print(wiscan[router_id][4]); 
  USR.print(F("\r"));
  usr_response(0);
  
  delay(2500);
  wifi_status();
}

void wifi_status() {
  USR.print(F("AT+WSLK\r")); 
  String response = usr_response(0);
  if (response.indexOf(F("Disconnected")) != -1) {
    if (WIO == 1) {
      WIO = 0;
      wifi_icon(0);
      USR.print(F("AT+WMODE=APSTA\r")); usr_response(0);
      USR.print(F("AT+Z\r")); delay(150);
      init_usr();
    }
    if (DEBUG == 1) MCU.print(response); 
    return;
  }
  
  else {
    if (WIO == 0) {
      int wifi_lev = 3;
      WIO = 1;
      wifi_icon(wifi_lev);
      int indA = response.indexOf(F("=")); int indB = response.indexOf(F("("));
      if (indB != -1) {
        WID = response.substring(indA+1, indB);
      }
    }    
    else {
      response = F("");
      //while (USR.available()) { USR.read(); }
      USR.print(F("AT+WSLQ\r")); 
      response = usr_response(0);
      int wifi_lev = 3;
      if (response.indexOf(F("Poor")) != -1) {
        wifi_lev = 2;
      }
      else if (response.indexOf(F("Fair")) != -1) {
        wifi_lev = 2;
      }
      else {
        wifi_lev = 3;
      }
      WIO = 1;
      wifi_icon(wifi_lev);
    }
    
    MCU.print(response); 
    response = F(""); 
    return;
  }
  
  WIO = 0;
  wifi_icon(0);
  
  // IF IOS STILL WAITING FOR STATUS
  if (wic_flag == 1) {
    if (WIO == 1) {
      BC127.print(F("SEND WIC:OK^\n\r"));
    }
    else {
      BC127.print(F("SEND WIC:OK^\n\r")); //SHOULD BE ERR
    }
    wic_flag = 0;
  }
}

void usr_ssid() {
  USR.print(F("AT+WSSSID\r"));
  String WSSSID = usr_response(0);
  int mac_ind = WSSSID.indexOf(F("("));
  MCU.print(F("SSID (WID): "));
  if (mac_ind != -1) {
    WID = WSSSID.substring((WSSSID.indexOf(F("="))+1), mac_ind);
  }
  else {
    WID = F("");
  }
  USR.print(F("AT+GDSSID=") + WID + F("\r")); 
  usr_response(0);
  MCU.println(WID);
  return;
}

void usr_ip() {
  USR.print(F("AT+WANN\r")); 
  
  String WANN = usr_response(0);
  if (WANN.indexOf(F("DHCP,")) != -1) {
    int indA = WANN.indexOf(F(",")); 
    int indB = WANN.indexOf(F(","), indA+1);
    if (indB != -1) {
      sta_ip = WANN.substring(indA+1, indB);
    }
    if (sta_ip == F("0.0.0.0")) {
      sta_ip = F("10.10.100.254");
    }
    MCU.print(F("STA IP: ")); 
    MCU.println(sta_ip);
    return;
  }
}

void wifi_reset() {
  USR.print(F("AT+Z\r")); return;
}

boolean usr_link() {
  unsigned long waitTimeout = 1000; // wait 1 second to connect
  unsigned long start = millis();
  char lastCmdResultChar;
 
  USR.print(F("AT+TCPLKB\r"));
    
  while (millis() - start < waitTimeout) {
    if (USR.available()) { // Output should be "+ok=on"
      lastCmdResultChar = USR.read();
      //uncomment this for debugging. Will show "+ok=off" until a connection is made
      //MCU.write(lastCmdResultChar);
    }
    if (lastCmdResultChar == 'n') { // Command Returned "+ok=on"
      MCU.println(F("Link Time: ") + (String)(millis() - start) + F(" ms"));
      return true;
    }
    else if (lastCmdResultChar == 'f') { // Command Returned "+ok=off"
      lastCmdResultChar = 'x'; // Reset the character so we don't issue this command repeatedly
      USR.print(F("AT+TCPLKB\r"));
    }
  }
  return false;
}

boolean usr_txrx(String command) {
  USR.print(command + "\r"); 
  delay(50);
  if(USR.find("+ok")) {
    return true;
  }
  else
  {
    //MCU.println("Error! No +ok received from command!");
    return false;
  }
}
*/













