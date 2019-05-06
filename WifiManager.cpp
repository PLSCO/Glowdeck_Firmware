//
//  WifiManager.cpp
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//

#include "WifiManager.h"

extern GlowdeckManager      glowdeckManager;
extern BootloaderManager    bootloaderManager;
extern SerialManager        serialManager;
extern BluetoothManager     bluetoothManager;
extern LEDManager           ledManager;
extern DisplayManager       displayManager;


WifiManager::WifiManager(HardwareSerial1 *ser) {
  
  this->wi = wifi;
  
  recvPrefixLength = 9;
  
  wi.begin(9600);
  
}


void WifiManager::setup() {
  
  wifiBuffer = "";
  
  clear();

  while (wi.available()) { wi.read(); }

  commandMode();
  
  wi.print("AT+E\r"); getResponse(0);
  
  while (wi.available()) { wi.read(); }

  // GIVE USR GLOWDECK MODEL AND UDID VALUES FOR WEB CLIENT
  wi.print("AT+GDMODEL=Beta\r");
  
  getResponse(0);
  
  String udid = "00000"; // glowdeckManager.udid;
  
  String longName = "Glowdeck"; // glowdeckManager.longName;

  
  wi.print("AT+GDUDID=GD" + udid + "\r");
  getResponse(0);
  
  wi.print("AT+GDLGN=" + longName + "\r");
  getResponse(0);
  
  wi.print(F("AT+GDSSID="));
  
  if ((accessPointName != "0") && (accessPointName != "MyRouter")) {
    
     wi.print(accessPointName); 
     
  }
  else {
    
    wi.print("MyRouter");
    
  }
  
  wi.print("\r"); 
  getResponse(0);
  
  wi.print(F("AT+TCPDISB=off\r")); 
  getResponse(0);
  
  // SET HTTP COMMAND CONSTANTS HERE
  wi.print(F("AT+TCPDIS=off\r"));
  getResponse(0);
  
  wi.print(F("AT+HTTPURL=streams.io,443\r"));
  getResponse(0);  //CLEAR RESPONSE (Expecting: +ok)
  
  wi.print(F("AT+HTTPTP=POST\r"));
  getResponse(0);  //CLEAR RESPONSE (Expecting: +ok)
  
  wi.print(F("AT+HTTPCN=keep-alive\r"));
  getResponse(0);  //CLEAR RESPONSE (Expecting: +ok)
  
  wi.print(F("AT+HTTPUA=lwip1.3.2\r"));  //TO DO: CHANGE UA TO UDID
  getResponse(0);  //CLEAR RESPONSE (Expecting: +ok)
  
  wi.print(F("AT+TCPDIS=on\r"));
  getResponse(0);
  
  // ssid();
  
  // ip();
  
}

void WifiManager::loop() {

  if (wi.available()) {
    
    unsigned long st = millis();
      
    while (wi.available()) {
      
      char inChar = wifi.read();
      
      wifiBuffer += inChar;
      
      if ((wifiBuffer.contains("\r")) || (wifiBuffer.contains("\n"))) {
        
        handleMessage(wifiBuffer);
        
        wifiBuffer = "";
        
        break;
        
      }
      
      if ((millis() - st) > 5000) {
        
        wifiBuffer = "";
        
        break;
        
      }
      
    }
    
  }
  
}

void WifiManager::clear() {
  
  wi.flush();
  
  if (wi.available()) {
    
    while (wi.available()) {
      
      wi.read();
      
    }
    
  }
  
}

void WifiManager::handleMessage(String cmd) {
  
  cmd = cmd.replace("\r\n\r\n", "");
  
  cmd = cmd.trim(); 

  // #if defined DEBUG
    main.println("[WI] " + cmd);
  // #endif

  if (!(cmd.length() > 1)) return;

  if ((cmd == "+ok") || (cmd == "+ERR")) return;
  
  if (cmd.startsWith(F("+ok="))) {
    
    int tmp_len = cmd.length(); 
    
    cmd = cmd.substring(4, tmp_len);
    
  }
  
  if (cmd.indexOf(F("Ch,SSID,BSSID,")) != -1) { // AT+WSCAN RESPONSE DATA (SSIDs AND AUTH/ENCRYPTION MODES)
    
    for (int i = 0; i < 20; i++) {
      
      for (int j = 0; j < 4; j++) {
        
        wiscan[i][j] = F("");
        
      }
      
    }
    
    ssid_count = -1; 
    
    return;
    
  }
  
  else if ((cmd.indexOf(F("WPA2PSK/AES")) != -1) || (cmd.indexOf(F("OPEN/NONE")) != -1) || (cmd.indexOf(F("WPAPSK/TKIPAES")) != -1) || (cmd.indexOf(F("WPAPSKWPA2PSK/TKIPAES")) != -1)) { // SSID IN-RANGE LINE
    
    logSsid(cmd);
    
    return;
    
  }
  
  else if (cmd.indexOf(F("RF Off")) != -1) { // USR MODULE OFF
    
    if (wifiOn == 1) {
      
      wifiOn = 0; 
      
      displayManager.printWifi(0);
      
    }
    
    wifiOn = 0; 
    
    return;
    
  }   
  
  else if (cmd.indexOf(F("Disconnected")) != -1) { // NOT CONNECTED
    
    if (wifiOn == 1) {
      
      wifiOn = 0; displayManager.printWifi(0);
      
    }
    
    wifiOn = 0;
    
    main.println(F("STA LINK DISCONNECTED"));
    
    return;
    
  }
  
  else if (cmd.indexOf(accessPointName + F("(")) != -1)  { // STA LINK SUCCESSFULLY ESTABLISHED
    
    if (wifiOn == 0) {
      
      wifiOn = 1; 
      
      displayManager.printWifi(1);
      
      wi.print(F("AT+WANN\r")); 
      
      handleMessage(getResponse(0));
      
    }
    
    wifiOn = 1; 
    
    main.println(F("STA LINK ESTABLISHED"));
    
    return;
    
  }
  //else if (cmd.indexOf(F("DHCP,")) != -1) {
  //  int indA = cmd.indexOf(F(",")); int indB = cmd.indexOf(F(","), indA+1);
  //  if (indB != -1)
  //  {
  //    sta_ip = cmd.substring(indA+1, indB);
  //  }
  //  if (sta_ip == F("0.0.0.0"))
  //  {
  //    sta_ip = F("10.10.100.254");
  //  }
  //  main.print(F("STA_IP: ")); main.println(sta_ip); main.println();
  //  return;
  //}
  
  else if (cmd.endsWith(F("^"))) {
    
    if (cmd.startsWith(F("LGN:"))) { 
      
      String tmp_lnm = cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")));
      
      //if (glowdeckManager.longName != tmp_lnm) {
        
        //glowdeckManager.longName = tmp_lnm; 
        
        tmp_lnm = F("");
        
        bluetoothManager.clear();
        
        if ((bluetoothManager.bleConnected == 1) || (bluetoothManager.sppConnected == 1)) {
          
          bluetoothManager.send(F("SEND LGN:OK^\n\r"));
          
          delay(100);
          
        }
        /*
        bluetoothManager.send(F("SET NAME=")); 
        bluetoothManager.send(glowdeckManager.longName); 
        bluetoothManager.send(F("\r")); 
        delay(150);
        
        bluetoothManager.send(F("WRITE\r")); 
        delay(150);
        
        // bluetoothManager.send(F("RESET\r"));
        */  
      //}
      
      //if (PRV == 0)
      //{
      //  PRV = 1; EEPROM.write(1, '1');
      //}
      //wi.print(F("AT+GDLGN=") + glowdeckManager.longName + F("\r")); getResponse(0);
      
      return;
      
    }
    
    else if (cmd.startsWith(F("SSID:"))) { // USER SSID
    
      accessPointName = cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")));
      
      //wi.print(F("AT+GDSglowdeckManager.udid=") + accessPointName + F("\r")); getResponse(0);
      
      return;
      
    }
    
    else if (cmd.startsWith(F("PASS:"))) { //USER SSID
  
      password = cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")));
      
      status(); //sync_counter = 1;
      
      //while (wi.available()) { wi.read(); }
      //wi.print(F("AT+Z\r")); delay(1250); wifiSetup(); delay(600);
      //wi.print(F("AT+GDSSID=") + accessPointName + F("\r")); getResponse(0);
      //wi.print(F("AT+GDKEY=") + password + F("\r")); getResponse(0);
      //wi.print(F("AT+GDLGN=") + glowdeckManager.longName + F("\r")); getResponse(0);
      //wi.print(F("AT+Z\r")); delay(500);
      //TEST WIFI CONNECTION
      
      //wi.print(F("AT+WMODE=STA\r")); getResponse(0);
      //wi.print(F("AT+Z\r")); delay(500);
      //while (wi.available()) { wi.read(); }
      //wi.print(F("AT+E\r")); getResponse(0);
      //boolean wifi_success = false;
      //wi.print(F("AT+WSLK\r")); String resp = getResponse(0);
      //if (resp.indexOf(F("Disconnected")) != -1)
      //{
      //  int wifi_timeout = 6000; int wifi_start = millis();
      //  while (1)
      //  {
      //    wi.print(F("AT+WSLK\r")); String resp = getResponse(0);
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
      //  wifiOn = 1;
      //  displayManager.printWifi(3);
      //}
      //else
      //{
      //  wifiOn = 1;
      //  displayManager.printWifi(0);
      //  wi.print(F("AT+WMODE=AP\r")); getResponse(0);
      //  wi.print(F("AT+Z\r")); delay(100);
      //  while (wi.available()) { wi.read(); }
      //  //wi.print(F("AT+E\r")); getResponse(0);
      //}
      
      return;
      
    }
    
    else if (cmd.startsWith(F("UNM:"))) { // STREAMS USERNAME
    
      // UNM = cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")));
      
      return;
      
    }
/*
    else if (cmd.startsWith(F("PSW:"))) { //STREAMS PASSWORD
        
      PSW = cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")));
      
      //SPECIAL CODE FOR MANUALLY ADDING UID
      if (PSW == F("!!!")) {
        
        if ((UNM.length() >= 1) && (UNM.length() <= 5)) {
          
          UID = UNM; 
          
          PRV = 1;
          
          EEPROM.write(1, '1');

          if (UNM.length() == 1) {
            UNM = F("0000") + UNM;
          }
          else if (UNM.length() == 2) {
            UNM = F("000") + UNM;
          }
          else if (UNM.length() == 3) {
            UNM = F("00") + UNM;
          }
          else if (UNM.length() == 4) {
            UNM = F("0") + UNM;
          }
          
          char UID_CHR[10];
          
          UID.toCharArray(UID_CHR, 6);
          
          eeprom_write_string(7, UID_CHR);
          
          print_info(F("Streams Enabled!"));
          
        }
        
      }
      else {
        
        busy_icon(1);
        
        get_stream(F("a"));
        
        if ((UID != F("")) && (UID != F("0")) && (UID != F("00000"))) {
          
          get_stream(F("p"));
          
          print_info(F("Streams Enabled!"));
          
          get_stream(F("w"));
          
        }
        
        busy_icon(0);
        
      }
      
      return;
      
    }
*/

    else if (cmd.startsWith(F("DL:"))) { // DISPLAY BRIGHTNESS SLIDER
 
      int user_val = (cmd.substring((cmd.indexOf(F(":"))+1), cmd.indexOf(F("^")))).toInt();
      
      if (user_val == 11) { // USER ACTUALLY PRESSED DFU BUTTON
        
        bootloaderManager.enterBootloaderMode();
        
        return;
        
      }
      else if ((user_val >= 0) && (user_val <= 10)) {
        
        displayManager.brightness = user_val;
        
      }
      else {
        
        displayManager.brightness = 10;
        
      }
      
      /// analogWrite(LCD_BL, scaleBrightness(displayManager.brightness)); 
      
      if (bluetoothManager.sppConnected == 1) {
             
        bluetoothManager.send("DBR:" + String(displayManager.brightness) + "^");
        
        if (displayManager.autoBrightness == 1) {
                  
          bluetoothManager.send("DBA:0^");
          
        }
        
      }
      else if (bluetoothManager.bleConnected == 1) {
        
        bluetoothManager.send("DBR:" + String(displayManager.brightness) + "^");
        
        if (displayManager.autoBrightness == 1) {
        
          bluetoothManager.send("DBA:0^");
          
        }        
        
      }
      
      displayManager.autoBrightness = 0;
      
      return;
      
    }
    /*
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
        
        if (bluetoothManager.bleConnected == 1) {
          
          bluetoothManager.send(F("SEND NGT:1^\n\r")); 

        }
        
        return;
        
      }
      else if (NGT == 1) {
        
        NGT = 0;
        
        if (bluetoothManager.bleConnected == 1) {
          
          bluetoothManager.send(F("SEND NGT:0^\n\r")); 

        }
        
        return;
        
      }
      
      return;
      
    }
 
    else if (cmd.startsWith(F("CLR:"))) { // COLOR PICKER COMMAND
      
      int ind_a = cmd.indexOf(F(":")); 
      
      int ind_b = cmd.indexOf(F(","), ind_a+1); 
      
      int ind_c = cmd.indexOf(F(","), ind_b+1);
      
      int ind_d = cmd.indexOf(F("^"));
      
      if (ind_c != -1) {
        
        COL[0] = (cmd.substring(ind_a+1, ind_b)).toInt();
        
        COL[1] = (cmd.substring(ind_b+1, ind_c)).toInt();
        
        COL[2] = (cmd.substring(ind_c+1, ind_d)).toInt();
        
        FastLED.showColor(CRGB(COL[0], COL[1], COL[2]));
        
      }
      
      return;
      
    }
    */
    
  }
  
}

String WifiManager::send(String raw) {

  raw = raw.trim();
  
  if (!raw.contains("AT+")) {
    raw = "AT+" + raw + "\r";
  }
  else {
    raw = raw + "\r";
  }
  
  wi.print(raw);
  
  wi.flush();

  return "SENT";
}

String WifiManager::getResponse(int contentLength) {
  
  // 0 = cmd, 1 = header, 2 = body
  
  String buffer1 = F("");
  
  char inByte;
  int rCount = 0;
  int nCount = 0;
  int curLength = 0;
   
  bool end = false;
  
  wi.flush();
  
  unsigned long st = millis();

  while (!end) {
    
    if (wi.available()) {
      
      inByte = wi.read();
      
      curLength++;

      if (contentLength == 0) {
        
        if ((inByte == '\n') && (rCount == 2) && (nCount == 1)) {
          
          end = true;
          
          int strLength = buffer1.length() - 3;
          
          buffer1 = buffer1.substring(0, strLength);
          
        }
        else if (inByte == '\r') {
          
          rCount++;
          
        }
        else if (inByte == '\n') {
          
          nCount++;
          
        }
        else {
          
          rCount = 0;
          
          nCount = 0;
          
        }
        
      }
      else if (curLength >= contentLength) {
        
        end = true;
        
      }
      
      buffer1 += inByte;
      
    }
    else {
      
      if ((millis() - st) > (5500)) {
        
        break;
        
      }
      
    }
    
  }
  
  if (contentLength != 0) {
    
    if (wi.available()) {
      
      unsigned long st = millis(); 
      
      bool end = false;
      
      while (!end) {
        
        while (wi.available()) {
          
          wi.read();
          
        }
        
        delay(50);
        
        if ((!wi.available()) || ((millis() - st) > 1000)) {
          
          end = true;
        
        }
      
      }
    
    }
  
  }
  
  return buffer1;
  
}

String WifiManager::sendMessage(String msg, uint8_t timeout) {
  
  // main.println("[sendMessage] " + msg);
  
  wi.print(msg);
  
  wi.flush();
  
  return "SENT";
  
}

void WifiManager::enterCommandMode() {
  
  bool success = false;
  
  int retries = 0;
  
  int bothbaud = 0;

  do {
    
    if (retries > 2) {
      
      if (bothbaud == 0) {
        
        wi.end();
        
        wi.begin(9600);
        
        retries = 0;
        
        bothbaud = 1;
        
      }
      else {
        
        return;
        
      }
      
    }
    
    success = startATSequence();
    
    retries += 1;
    
  } while (!success);

  wi.print(F("a")); 
  
  getResponse(0);
  
  wi.print("AT+E\r");

  getResponse(0);
  
}

boolean WifiManager::startATSequence() {
  
  delay(50);
  
  while (wi.available()) { wi.read(); }
  
  wi.write("+++");
  
  unsigned long timeout = millis() + (4 * 1000);
  
  while (!wi.available()) {
    
    delay(1);
    
    if (millis() > timeout) {
      
      return false;
      
    }
    
  }
  
  char resp = wi.read();

  if (resp == 'a') {
    
    return true;
    
  }

  return false;
  
}

void WifiManager::config() {
  
  //wi.print(F("AT+E\r"));
  //getResponse(0);
  //wi.print(F("AT+WSLK\r"));
  //getResponse(0);
  
  //if (rspn.length() <= 2)
  //{
  //  baud_change = 1;
  //  wi.end(); wi.begin(115200); delay(500);
  //  while (wi.available()) { wi.read(); }
  //  wi.print(F("AT+E\r")); getResponse(0);
  //}
   
  //CREATE TEMPORARY NAME STRING
  String tmp_name = glowdeckManager.longName; // F("Glowdeck") + glowdeckManager.udid;
  
  //SET MODULE LANGUAGE TO ENGLISH
  wi.print(F("AT+PLANG=EN\r"));
  getResponse(0);  //CLEAR RESPONSE (Expecting: +ok)
 
  //SET MODULE ID TO "Glowdeck + glowdeckManager.udid" (Ex: Glowdeck15FC8)
  //wi.print(F("AT+WRMID=")); wi.print(tmp_name); wi.print(F("\r"));
  //getResponse(0);  //CLEAR RESPONSE (Expecting: +ok)

  //SET MODULE AP TO "Glowdeck + glowdeckManager.udid" (Ex: Glowdeck15FC8),
  //AND SET MODULE AP TO 11BGN, CH1
  wi.print("AT+WAP=11BGN," + glowdeckManager.longName + ",AUTO\r");
  getResponse(0);  //CLEAR RESPONSE (Expecting: +ok)

  //SET MODULE AP USERNAME AND PASSWORD TO EMPTY/EMPTY
  // empty not allowed, make u/p "glowdeck/glowdeck" for now!
  wi.print(F("AT+WEBU=none\r"));
  getResponse(0);  //CLEAR RESPONSE (Expecting: +ok)

  //SET MODULE TO TCP CLIENT MODE
  //SET MODULE TO CONNECT TO OUR SERVER (glowdeck.com:80)
  wi.print(F("AT+NETP=TCP,CLIENT,443,streams.io\r"));
  getResponse(0);  //CLEAR RESPONSE (Expecting: +ok)

  //SET MODULE TCP TIMEOUT TO 500MS
  wi.print(F("AT+TCPTO=600\r")); 
  getResponse(0);  //CLEAR RESPONSE (Expecting: +ok)

  //SET MODULE TO DUAL AP+STA MODES
  wi.print(F("AT+WMODE=APSTA\r"));
  getResponse(0);  //CLEAR RESPONSE (Expecting: +ok)
  
  wi.print(F("AT+WSSSID=MyRouter\r")); // + tmp_name + F("\r"));
  getResponse(0);
  
  wi.print(F("AT+TCPDISB=off\r")); 
  getResponse(0);

  wi.print(F("AT+HTTPURL=glowdeck.com,80\r"));
  getResponse(0);  //CLEAR RESPONSE (Expecting: +ok)
  
  wi.print(F("AT+HTTPTP=POST\r"));
  getResponse(0);  //CLEAR RESPONSE (Expecting: +ok)
  
  wi.print(F("AT+HTTPCN=keep-alive\r"));
  getResponse(0);  //CLEAR RESPONSE (Expecting: +ok)
  
  wi.print(F("AT+HTTPUA=lwip1.3.2\r"));  //TO DO: CHANGE UA TO UDID
  getResponse(0);  //CLEAR RESPONSE (Expecting: +ok)
  
  //SET MODULE UART TO 9600 BAUD
  wi.print(F("AT+UART=9600,8,1,NONE,NFC\r")); 
  getResponse(0);  //CLEAR RESPONSE (Expecting: +ok)

  //SET MODULE TO AUTO-SWITCH TO AP MODE ON BAD/NO STA CONNECTION
  wi.print(F("AT+MDCH=auto\r"));
  getResponse(0);  //CLEAR RESPONSE (Expecting: +ok)

  //MAKE NEW SETTINGS MODULE FACTORY DEFAULT SETTINGS
  wi.print(F("AT+CFGTF\r"));
  getResponse(0);  //CLEAR RESPONSE (Expecting: +ok)

  //RESET WIFI MODULE
  wi.print(F("AT+Z\r")); //delay(200);
  getResponse(0);
  
  delay(100);  //WAIT 100MS?

  //CLOSE UART AT 115200, OPEN NEW UART AT 9600
  //wi.end(); wi.begin(9600);
  //delay(450);  //WAIT 1150MS (IS THIS DELAY NECESSARY?)
  //REMOVE ANY LEFTOVER MODULE DATA FROM UART TO AVOID ERRORS
  //while (wi.available()) { wi.read(); }
  //RESET WIFI MODULE
  //wi.print(F("AT+E\r")); 
  //getResponse(0);
  
  wifiSetup();
  
}

int WifiManager::wifiSetup() {
  
  int result = 0; // 0 = DO NOT CONFIG, 1 = CONFIG, -1 = ERROR ENTERING CMD MODE
  
  bool usr_success = false;  //E NTER CMD MODE RESULT FLAG (=TRUE IF SUCCESS)
  
  int retries = 0;  // COUNTER FOR # OF FAILED ATTEMPTS TO INITIALIZE USR MODULE
  
  int bothbaud = 0;  // =0 IF WE DID NOT TRY 115200 YET, =1 IF WEVE TRIED 115200
  
  // char usr_done = '1';

  /*
  if (EEPROM.read(41) == usr_done) {
    
    result = 1;
    
    return result;
    
  }
  */
  
  wi.begin(115200);
  
  delay(5);
  
  // displayManager.printStream(F("starting wifi"), F(""));
  
  int startClock = millis();
  
  do {
    
    if (retries > 3) {
      
      if (bothbaud == 0) {
        
        main.println(F("USR Baud: 9600!"));
        
        wi.end(); 
        
        delay(5); 
        
        wi.begin(9600); 
        
        delay(5);
        
        retries = 0; 
        
        bothbaud = 1; 
        
        result = 1;
        
      }
      else {
        
        main.println(F("USR CMD Mode Fail!"));
        
        wi.end(); 
        
        result = -1;
        
        return result;
        
      }
      
    }
    
    if (millis() - startClock > 6500) {
      
      wi.end();
      
      result = -1;
      
      return result;
      
    }
    
    usr_success = commandMode();
    
    retries += 1;
    
  } while (!usr_success);

  wi.print(F("a")); 
  getResponse(0);
  
  wi.print(F("AT+E\r")); 
  getResponse(0);
  delay(75);

  while (wi.available()) { wi.read(); }
  wi.print(F("AT+PLANG=EN\r")); delay(50);
  
  while (wi.available()) { wi.read(); }
  wi.print(F("AT+WMODE=STA\r")); delay(50);
  
  while (wi.available()) { wi.read(); }
  wi.print(F("AT+MDCH=Off\r")); delay(50);
  
  while (wi.available()) { wi.read(); }    
  
  if (bothbaud != 1) {
   
    wi.print(F("AT+UART=9600,8,1,NONE,NFC\r")); 
 
    delay(50);
 
  }
  
  //while (wi.available()) { wi.read(); }
  
  //baud_flag = '1'; EEPROM.write(167, baud_flag);

  wi.print(F("AT+WIFI=DOWN\r")); 
  
  delay(50);
  
  wi.flush(); 
  
  wi.print(F("AT+Z\r")); 
  
  wi.end();
  
  // EEPROM.write(41, usr_done);
  
  return result;
  
}

boolean WifiManager::commandMode() {
  
  //WAIT
  delay(75);  // WHAT DELAY IS NEEDED HERE IF ANY?
  
  //CLEAR ANY UNKNOWN BYTES FROM UART TO AVOID ERRORS
  while (wi.available()) { wi.read(); } 
  
  //FOLLOW USR INSTRUCTIONS TO ENTER COMMAND MODE
  wi.write("+++");
  
  //SET TIMEOUT TO 4000MS FOR CMD MODE ENTRY
  unsigned long timeout = millis() + (4000);
  
  //WAIT FOR MODULE TO RESPOND WITH "a" IF SUCCESS, 
  //IF CMD MODE FAILS, MODULE WILL NOT WORK UNTIL USER POWERS ON AGAIN!
  while (!wi.available()) {
    
    delay(1);
    
    if (millis() > timeout) { return false; }
    
  }
  
  char resp = wi.read();
  
  if (resp == 'a') {
    
    return true;
    
  }
  
  return false;
  
} 

String WifiManager::shorten(String cmd) {
  
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

void WifiManager::logSsid(String cmd) {
  
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
    
    // [0] SglowdeckManager.udid
    wiscan[ssid_count][0] = cmd.substring(a0+1, a1);
    
    // [1] AUTHENTICATION TYPE | [2] ENCRYPTION MODE
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

void WifiManager::parseSsid(String cmd) {
  
  int len = cmd.length();
  
  // CLEAR WISCAN ARRAY
  if (len < 10) {
    
    cmd = F("");
    
    return;
    
  }
  
  if (ssid_count != -1) {
    
    for (int r = 0; r < 15; r++) {
      
      for (int c = 0; c < 5; c++) {
        
        // wiscan[r][c] = F("");
        
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

        int a0, a1, b0, b1, c0, c1; 
        //int d0; int d1; int e0; int e1;
        
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
      
      aInd = nInd + 1; res = F(""); 
      
      continue;
      
    }
    
  }
  
  cmd = F("");
  
}

void WifiManager::connect(int router_id) {
  
  String response;
  
  while (wi.available()) { wi.read(); }
  
  wi.print(F("AT+WSSSID="));
  
  wi.print(wiscan[router_id][0]); 
  
  wi.print(F("\r")); 
  
  getResponse(0); 
  
  delay(50);
  
  wi.print(F("AT+WSKEY=")); 
  wi.print(wiscan[router_id][1]); 
  wi.print(F(",")); 
  wi.print(wiscan[router_id][2]); 
  wi.print(F(",")); 
  wi.print(wiscan[router_id][4]); 
  wi.print(F("\r"));
  
  getResponse(0);
  
  delay(2500);
  
  status();
  
}

void WifiManager::status() {
  
  wi.print(F("AT+WSLK\r"));
  
  String response = getResponse(0);
  
  if (response.indexOf(F("Disconnected")) != -1) {
    
    if (wifiOn == 1) {
      
      wifiOn = 0;
      
      displayManager.printWifi(0);
      
      wi.print(F("AT+WMODE=APSTA\r"));
      
      getResponse(0);
      
      wi.print(F("AT+Z\r")); 
      
      delay(150);
      
      wifiSetup();
      
    }
    
    main.print(response);
    
    return;
    
  }
  else {
    
    if (wifiOn == 0) {
      
      int signalStrength = 3;
      
      wifiOn = 1;
      
      displayManager.printWifi(signalStrength);
      
      int indA = response.indexOf(F("="));
      
      int indB = response.indexOf(F("("));
      
      if (indB != -1) {
        
        accessPointName = response.substring(indA+1, indB);
        
      }
      
    }    
    else {
      
      response = F("");
      
      //while (wi.available()) { wi.read(); }
      
      wi.print(F("AT+WSLQ\r")); 
      
      response = getResponse(0);
      
      int signalStrength = 3;
      
      if (response.indexOf(F("Poor")) != -1) {
        
        signalStrength = 2;
        
      }
      else if (response.indexOf(F("Fair")) != -1) {
        
        signalStrength = 2;
        
      }
      else {
        
        signalStrength = 3;
        
      }
      
      wifiOn = 1;
      
      displayManager.printWifi(signalStrength);
      
    }
    
    main.print(response); 
    
    response = F(""); 
    
    return;
    
  }
  
  wifiOn = 0;
  
  displayManager.printWifi(0);

  /*
  // IF IOS STILL WAITING FOR STATUS
  if (wic_flag == 1) {
    if (wifiOn == 1) {
      bluetoothManager.send(F("SEND WIC:OK^\n\r"));
    }
    else {
      bluetoothManager.send(F("SEND WIC:OK^\n\r")); //SHOULD BE ERR
    }
    wic_flag = 0;
  }
  */
  
}

void WifiManager::ssid() {
  
  wi.print(F("AT+WSSSID\r"));
  
  String WSSSID = getResponse(0);
  
  int mac_ind = WSSSID.indexOf(F("("));
  
  main.print(F("SSID (WID): "));
  
  if (mac_ind != -1) {
    
    accessPointName = WSSSID.substring((WSSSID.indexOf(F("="))+1), mac_ind);
    
  }
  else {
    
    accessPointName = F("");
    
  }
  
  wi.print(F("AT+GDLNM=") + accessPointName + F("\r"));
  
  getResponse(0);
  
  main.println(accessPointName);
  
  return;
  
}

void WifiManager::ip() {
  
  wi.print(F("AT+WANN\r")); 
  
  String WANN = getResponse(0);
  
  if (WANN.indexOf(F("DHCP,")) != -1) {
    
    int indA = WANN.indexOf(F(",")); 
    
    int indB = WANN.indexOf(F(","), indA+1);
    
    if (indB != -1) {
      
      sta_ip = WANN.substring(indA+1, indB);
      
    }
    
    if (sta_ip == F("0.0.0.0")) {
      
      sta_ip = F("10.10.100.254");
      
    }
    
    main.print(F("STA IP: ")); 
    
    main.println(sta_ip);
    
    return;
    
  }
  
}

void WifiManager::reset() {
  
  wi.print(F("AT+Z\r"));
  
  return;
  
}

boolean WifiManager::link() {
  
  unsigned long waitTimeout = 1000; // wait 1 second to connect
  
  unsigned long start = millis();
  
  char lastCmdResultChar;
 
  wi.print(F("AT+TCPLKB\r"));
    
  while (millis() - start < waitTimeout) {
    
    if (wi.available()) { 
      
      // Output should be "+ok=on"
      lastCmdResultChar = wi.read();
      
      // uncomment this for debugging. Will show "+ok=off" until a connection is made
      
      //main.write(lastCmdResultChar);
      
    }
    
    if (lastCmdResultChar == 'n') { // Command Returned "+ok=on"
      
      main.println(F("Link Time: ") + (String)(millis() - start) + F(" ms"));
      
      return true;
      
    }
    else if (lastCmdResultChar == 'f') { // Command Returned "+ok=off"
      
      lastCmdResultChar = 'x'; // Reset the character so we don't issue this command repeatedly
      
      wi.print(F("AT+TCPLKB\r"));
      
    }
    
  }
  
  return false;
  
}

boolean WifiManager::txrx(String command) {
  
  wi.print(command + "\r"); 
  
  delay(50);
  
  if (wi.find("+ok")) {
    
    return true;
    
  }
  else {
    
    //main.println("Error! No +ok received from command!");
    
    return false;
    
  }
  
}
