//
//  SerialManager.cpp
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//

#include "SerialManager.h"


extern LEDManager         ledManager;
extern DisplayManager     displayManager;
extern BluetoothManager   bluetoothManager;
extern WifiManager        wifiManager;


SerialManager::SerialManager(usb_serial_class *ser) {
  
  this->sp = main;

  // MOVE ALL THESE TO RIGHT PLACE...
  pinMode(AMP, OUTPUT);
  digitalWrite(AMP, LOW);

  pinMode(COIL_PW, OUTPUT);
  pinMode(COILS, INPUT);
  digitalWrite(COIL_PW, HIGH);
  // MOVE ALL THESE TO RIGHT PLACE...

  main.begin(9600);
}

void SerialManager::setup() {
  channel = WIFI;
  mainBuffer = ""; 
}

void SerialManager::loop() {
  
  if (main.available()) {
          
    unsigned long st = millis();
    
    while (main.available()) {
      
      char inChar = main.read();
      
      mainBuffer += inChar;
      
      if (this->mainBuffer.indexOf("\r") != -1) {
        
        DisplayManager::printStream("general", mainBuffer, "MAIN", "");
        
        mainHandler(mainBuffer);
        
        mainBuffer = "";
        
        break;
        
      }
      
      if ((millis() - st) > 3000) {
        
        mainBuffer = "";
        
        break;
        
      }
      
    }
    
  }

}


void SerialManager::setSendChannel(sendChannel newChannel) {
  channel = newChannel;
}

void SerialManager::btleSend(String tx) {
  bluetoothManager.send(tx);
}

// Clears out all data from main port
void SerialManager::mainClear() {
  if (main) {
    main.flush();
    while (main.available()) {  
      main.read();
    }
  }
}


/*
// Clears out all data from wifi port
void SerialManager::wifiClear() {
  wifi.flush();
  while (wifi.available()) {  
    wifi.read();
  }
}
*/

// Clears out all data from all serial ports
void SerialManager::clearAll() {
  //btleClear();
  //wifiClear();
  mainClear();
}

// Debugging
void SerialManager::logEvent(String event) {
  main.println("[LOG] " + event);
}

// Handles input from main poer
void SerialManager::mainHandler(String raw) {
  if ((raw.indexOf("GFU^") != -1) || (raw.startsWith("```"))) {
    BootloaderManager::enterBootloaderMode();
    return;
  }
  else if (raw.indexOf("CH:") != -1) {
    if (raw.indexOf("WIFI") != -1) {
      channel = WIFI;
    }
    else {
      channel = BLUETOOTH;
    }
    return;
  }

  // raw.trim();

  String response;

  switch (channel) {
    case WIFI:
      response = wifiManager.send(raw);
      break;
    case BLUETOOTH:
      response = bluetoothManager.send(raw);
      break;
    default:
      response = bluetoothManager.send(raw);
  }

  main.println("[RESPONSE] " + response);
  
  /*
  if (raw.indexOf("^") != -1) {
      raw.replace("\n\r", "");
      raw.replace("\r", "");
      raw.replace("\n", "");
      raw.replace("SEND ", "");
      raw = "SEND " + raw + "\n\r";
  }
  else {
    if (raw.indexOf("\r") == -1) {
      raw = raw + "\r";
    }
  }
  
  btle.print(raw);
  btle.flush();
  */
  
}

/*
// Handles input from btle port
void SerialManager::btleHandler(String raw) {


}

// Handles input from wifi port
void SerialManager::wifiHandler(String raw) {
  main.println("[WIFI] " + raw);
  
}
*/








