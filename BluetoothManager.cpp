//
//  BluetoothManager.cpp
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//

#include "BluetoothManager.h"

extern GlowdeckManager glowdeckManager;
extern BootloaderManager bootloaderManager;
extern SerialManager serialManager;
extern LEDManager ledManager;
extern DisplayManager displayManager;

BluetoothManager::BluetoothManager(HardwareSerial2 *ser) {
  this->bt = btle;
  recvPrefixLength = 8;
  melodyVersion = "6.1.5";
  nextGen = true;
  _numAddresses = -1;
  endOfLine = String("\n\r");
  // melodyVersion = "5.7.14";

  bleConnected = false;
  sppConnected = false;
  mapConnected = false;
  hfpConnected = false;
  
  pinMode(PIO0, OUTPUT);
  pinMode(PIO1, OUTPUT);
  pinMode(PIO2, OUTPUT);
  pinMode(PIO4, OUTPUT);
  pinMode(PIO5, OUTPUT);

  bt.begin(9600);
}

void BluetoothManager::setup() {
  memset(recvBuffer, 0, sizeof(recvBuffer));
  recvProcess = "";
  clear();
  // send("VERSION");
}

void BluetoothManager::loop() {
  if (bt.getCommandCount()) {
    memset(recvBuffer, 0, sizeof(recvBuffer));
    
    while (bt.getCommandCount()) {
      bt.pullCommand(recvBuffer);
      recvProcess = (char*)recvBuffer;
      memset(recvBuffer, 0, sizeof(recvBuffer));
      // main.println(recvProcess);
      
      if (recvProcess.contains("RECV ")) {
        if (!recvProcess.contains("^")) {
          if (recvProcess.length() >= (unsigned)(recvPrefixLength + 19)) {
            String appended;

            if (nextGen) {
              String tempProcess = recvProcess.substring(recvPrefixLength, recvProcess.length());
              appended = tempProcess.substring(tempProcess.indexOf(" ") + 1, tempProcess.length());
            } else {
              appended = recvProcess;
            }
            
            recvProcess = "";
            appended.trim();
            unsigned long st = millis();
            
            while (millis() - st < 5000) {
              if (bt.getCommandCount()) {
                bt.pullCommand(recvBuffer);
                recvProcess = (char*)recvBuffer;
                memset(recvBuffer, 0, sizeof(recvBuffer));
                
                if (recvProcess.contains("RECV ")) {
                  uint8_t len = recvProcess.length();

                  if (nextGen) {
                    String temp = recvProcess.substring(recvPrefixLength, len);
                    appended = appended + temp.substring(temp.indexOf(" ") + 1, temp.length());
                  } else {
                    appended = appended + recvProcess.substring(recvPrefixLength, len);
                  } 
                                  
                  recvProcess = "";
                  appended.trim();
                  
                  if (appended.contains("^")) {
                    recvProcess = appended;
                    appended = "";
                    break;
                  }
                }    
              }
            }
          } else {
            if (nextGen) {
              String temp = recvProcess.substring(recvPrefixLength, recvProcess.lastIndexOf("^") + 1);
              recvProcess = temp.substring(temp.indexOf(" ") + 1, temp.length());
            } else {
              recvProcess = recvProcess.substring(recvPrefixLength, recvProcess.lastIndexOf("^") + 1);
            }
            
            recvProcess.trim();
            handleMessage(recvProcess);
            recvProcess = "";

            if ((bt.checkListOverflow()) || (bt.checkCharOverflow())) bt.resetCommandList();
            continue;
          }
        } else {
          if (nextGen) {
            String temp = recvProcess.substring(recvPrefixLength, recvProcess.length());
            recvProcess = temp.substring(temp.indexOf(" ") + 1, temp.length());
          } else {
            recvProcess = recvProcess.substring(recvPrefixLength, recvProcess.length());
          }
          
          recvProcess.trim();
          handleMessage(recvProcess);
          recvProcess = "";
                      
          if ((bt.checkListOverflow()) || (bt.checkCharOverflow())) bt.resetCommandList();
          continue;
        }  
      }
     
      handleMessage(recvProcess);
      recvProcess = "";

      if ((bt.checkListOverflow()) || (bt.checkCharOverflow())) bt.resetCommandList();
      continue;
    }
  }
}

void BluetoothManager::clear() {
  bt.flush();
  if (bt.available()) {
    while (bt.available()) {
      bt.read();
    }
  }
  bt.resetCommandList();
  memset(recvBuffer, 0, sizeof(recvBuffer));
  recvProcess = "";
}

void BluetoothManager::handleMessage(String msg) {
  if ((msg == " ") || (msg.length() < 2)) return;
  
  #if defined DEBUG
    main.println("[BT] " + msg);
  #endif

  displayManager.debugPrint(msg);

  if (msg.contains("Melody Audio")) {
    String verString = msg.replace("Melody Audio V", "");
    melodyVersion = verString;
    if (melodyVersion.contains("6.1")) {
      nextGen = true;
      recvPrefixLength = 8;
    }
    else {
      nextGen = false;
      recvPrefixLength = 9;
    }
  }
  else if (msg.contains("OPEN_OK ")) {
    if (msg.contains("BLE")) {
      bleConnected = true;
      if (nextGen) {
        recvPrefixLength = 8;
      }
      else {
        recvPrefixLength = 9;
      }
    }
    else if (msg.contains("SPP")) {
      sppConnected = true;
      if (!nextGen) {
        recvPrefixLength = 14;
      }
      else {
        recvPrefixLength = 8;
      }
    }
  } else if (msg.contains("CLOSE_OK ")) {
    if (msg.contains("BLE")) {
      bleConnected = false;
    }
    else if (msg.contains("SPP")) {
      sppConnected = false;
    }
  } else if ((msg.contains("GFU^")) || (msg.contains("```"))) {
    BootloaderManager::enterBootloaderMode();
  }
  else if (msg.contains("ANM:")) {
    if (msg.contains("-1")) {
      ledManager.ledState = -1;
    } else {
      ledManager.ledState = 2;
    }
  } else if (msg.contains("TEST^^^")) {
    unitTest();
  }
}

String BluetoothManager::send(String raw) {
  raw = raw.trim();
  
  if (raw.contains(":")) {
    if (!raw.contains("^")) raw += "^";
    raw = "SEND " + raw + "\n\r";
  }
  else {
    raw = raw + "\r";
  }

  bt.print(raw);
  bt.flush();

  return "SENT";
}

void BluetoothManager::unitTest() {
  int connectionResult = 0;
  
  main.print("Inquiry result: "); 
  
  main.println(inquiry(10));
  
  String address;
  
  for (byte i = 0; i < 5; i++) {
    
    if (getAddress(i, address)) {
      
      main.print("BT device found at address ");
      
      main.println(address);
      
      if (address.startsWith("20FABB")) {
        
        main.print("BC127 found at index ");
         
        main.println(i);
        
        main.print("Connect result: ");
        
        connectionResult = connect(address, BluetoothManager::SPP);
        
        main.println(connectionResult);
        
        break;
        
      }
      
    }
    
  }
  
  if (connectionResult == 0) {
    
    main.println("No BC127 modules found!");
    
  }
  else {
    
    if (connectionResult == 1) {
      
      main.print("Entering data mode...");
      
      if (enterDataMode()) {
        
        main.println("OK!");
        
        bt.println("Hello, world!");
        
        bt.flush();
        
        delay(500);
        
        main.print("Exiting data mode...");
        
        if (exitDataMode()) main.println("OK!");
        
        else main.println("Failure!");
        
      }
      else main.println("Failure!");
      
    }
    
  }
  
}


String BluetoothManager::sendMessage(String msg, uint8_t timeout) {

  main.println("[sendMessage] " + msg);

  bt.print(msg);
  bt.flush();

  uint8_t tempBuffer[256];
  String  tempProcess = "";

  unsigned long timeoutMillis = timeout * 1000;
  unsigned long startTime     = millis();
  
  while ((startTime + timeoutMillis) > millis()) {

    if (bt.getCommandCount()) {
  
      memset(tempBuffer, 0, sizeof(tempBuffer));   
  
      while (bt.getCommandCount()) {
        
        bt.pullCommand(tempBuffer);
        
        tempProcess = (char*)tempBuffer;
        
        memset(tempBuffer, 0, sizeof(tempBuffer));
        
        if (tempProcess.contains("RECV ")) {
          
          if (tempProcess.contains("^")) {
            
            if (tempProcess.length() >= (unsigned)(recvPrefixLength + 19)) {
              
              String appended = tempProcess;
              
              tempProcess = "";
             
              appended.trim();
                            
              while ((startTime + timeoutMillis) > millis()) {
                
                if (bt.getCommandCount()) {
                  
                  bt.pullCommand(tempBuffer);
                  
                  tempProcess = (char*)tempBuffer;
                  
                  memset(tempBuffer, 0, sizeof(tempBuffer));
                  
                  if (tempProcess.contains("RECV ")) {
                    
                    int len = tempProcess.length();
                    
                    appended = appended + tempProcess.substring(recvPrefixLength, len);
                    
                    tempProcess = "";
                    
                    appended.trim();
                    
                    if (appended.contains("^")) {
                      
                      tempProcess = appended;
                      
                      appended = "";
                      
                      break;
    
                    }
                    
                  }    
                  
                }
                
              }
              
            }
            else {
              
              tempProcess = tempProcess.substring(recvPrefixLength, tempProcess.length());
              
              tempProcess.trim();
              
              if ((bt.checkListOverflow()) || (bt.checkCharOverflow())) bt.resetCommandList();
  
              return tempProcess;
                            
            }
            
          }
          
        }
            
      }

    }

  }   

  return tempProcess;
}

BluetoothManager::btResult BluetoothManager::knownStart() {

  String buffer = "";
  
  bt.print("\r");
  bt.flush();


  unsigned long startTime = millis();
  
  while (buffer.endsWith(endOfLine) != true) {
    
    if (bt.available() > 0) {
    
      buffer.concat(char(bt.read()));
      
      startTime = millis();
      
    }
    
    if ((startTime + 1000) < millis()) return TIMEOUT_ERROR;
    
  }

  if (buffer.startsWith("ERR")) return SUCCESS;
  
  else return SUCCESS;
 
}

BluetoothManager::btResult BluetoothManager::addressQuery(String &address) {
 
  return stdGetParam("LOCAL_ADDR", &address);
  
}

BluetoothManager::btResult BluetoothManager::setBaudRate(baudRates newSpeed) {
  // int intSpeed;
  String stringSpeed;
  
  // Convert enum values into strings
  switch(newSpeed) {
    case BluetoothManager::s9600bps:
      stringSpeed = "9600";
      break;
    case BluetoothManager::s19200bps:
      stringSpeed = "19200";
      break;
    case BluetoothManager::s38400bps:
      stringSpeed = "38400";
      break;
    case BluetoothManager::s57600bps:
      stringSpeed = "57600";
      break;
    case BluetoothManager::s115200bps:
      stringSpeed = "115200";
      break;
    default:
      return INVALID_PARAM;
  }
  
  // Valid input to the function, send it to set parameter function
  // intSpeed = stringSpeed.toInt();  

  
  //  SUCCESS         indicates baud rate set to the current baud rate
  //  MODULE_ERROR    indicates something happened to string pre-transmission
  //  TIMEOUT_ERROR   indicates wrong baud rate
  return stdSetParam("BAUD", stringSpeed);
}

BluetoothManager::btResult BluetoothManager::stdCmd(String command) {
  String buffer;
 
  String EOL = String("\n\r");
  
  // Clear serial buffer
  knownStart(); 
  serialManager.btleSend(command + "\r");

  // Timeout loop...
  unsigned long startTime = millis();

  while ((startTime + 3000) > millis()) {
    // Grow the current buffered data, until we receive the EOL string.    
    if (bt.available() > 0) buffer.concat(char(bt.read()));

    if (buffer.endsWith(EOL)) {
      if (buffer.startsWith("ER")) return MODULE_ERROR;
      if (buffer.startsWith("OK")) return SUCCESS;
      buffer = "";
    }    
  }

  return TIMEOUT_ERROR;
  
}

BluetoothManager::btResult BluetoothManager::stdSetParam(String command, String param) {
 
  String buffer;
    
  knownStart();

  serialManager.btleSend("SET " + command + "=" + param + "\r");
  
  // Timeout loop...
  unsigned long startTime = millis();
  
  while ((startTime + 2000) > millis()) {
    // Grow the current buffered data, until we receive the EOL string.    
    if (bt.available() >0) buffer.concat(char(bt.read()));

    if (buffer.endsWith(endOfLine)) {
      if (buffer.startsWith("ER")) return MODULE_ERROR;
      else if (buffer.startsWith("OK")) return SUCCESS;
      buffer = "";
    }    
  }

  return TIMEOUT_ERROR;

}

BluetoothManager::btResult BluetoothManager::stdGetParam(String command, String *param) {
  
  String buffer;
   
  knownStart();
  
  bt.print("GET ");
  bt.print(command);
  bt.print("\r");
  bt.flush();
  
  // We're going to use the internal timer to track the elapsed time since we
  //  issued the get command. Bog-standard Arduino stuff.
  unsigned long loopStart = millis();
  
  // This is our timeout loop. We'll give the module 2 seconds to get the value.
  while (loopStart + 2000 > millis()) {
    // Grow the current buffered data, until we receive the EOL string.    
    if (bt.available() > 0) buffer.concat(char(bt.read()));

    if (buffer.endsWith(endOfLine)) {
      if (buffer.startsWith("ER")) return MODULE_ERROR;
      if (buffer.startsWith("OK")) return SUCCESS;
      if (buffer.startsWith(command)) {
        (*param) = buffer.substring(command.length()+1);
        (*param).trim();
      }
      buffer = "";
    }    
  }
  return TIMEOUT_ERROR;
}

BluetoothManager::btResult BluetoothManager::BLEDisable() {
  return stdSetParam("BLE_ROLE", "0");
}

BluetoothManager::btResult BluetoothManager::BLECentral() {
  return stdSetParam("BLE_ROLE", "2");
}

BluetoothManager::btResult BluetoothManager::BLEPeripheral() {
  return stdSetParam("BLE_ROLE", "1");
}

BluetoothManager::btResult BluetoothManager::restore() {
  return stdCmd("RESTORE");
}

BluetoothManager::btResult BluetoothManager::writeConfig() {
  return stdCmd("WRITE");
}

BluetoothManager::btResult BluetoothManager::reset() {
  
  String buffer;
  
  knownStart();

  bt.print("RESET");
  bt.print("\r");
  bt.flush();
  
  unsigned long resetStart = millis();
  
  while ((resetStart + 2000) > millis()) {
  
    if (bt.available() > 0) {
      
      char temp = bt.read();
      
      buffer.concat(temp);
      
    }
    
    if (buffer.endsWith(endOfLine)) {
     
      if (buffer.startsWith("ER")) return MODULE_ERROR;
      
      if (buffer.startsWith("Re")) return SUCCESS;
      
      buffer = "";
      
    }

  }

  return TIMEOUT_ERROR;
  
}

BluetoothManager::btResult BluetoothManager::musicCommands(audioCmds command) {
  
  switch(command) {
    case PAUSE:
      return stdCmd("MUSIC PAUSE");
    case PLAY:
      return stdCmd("MUSIC PLAY");
    case FORWARD:
      return stdCmd("MUSIC FORWARD");
    case BACK:
      return stdCmd("MUSIC BACKWARD");
    case STOP:
      return stdCmd("MUSIC STOP");
    case UP:
      return stdCmd("VOLUME UP");
    case DOWN:
      return stdCmd("VOLUME DOWN");
    default:
      return INVALID_PARAM;
  }
  
}

BluetoothManager::btResult BluetoothManager::setClassicSource() {
  
  return stdSetParam("CLASSIC_ROLE", "1");
  
}

BluetoothManager::btResult BluetoothManager::setClassicSink() {
  
  return stdSetParam("CLASSIC_ROLE", "0");
  
}

BluetoothManager::btResult BluetoothManager::BLEAdvertise() {
  
  return stdCmd("ADVERTISING ON");
  
}

BluetoothManager::btResult BluetoothManager::BLENoAdvertise() {
  
  return stdCmd("ADVERTISING OFF");
  
}

BluetoothManager::btResult BluetoothManager::BLEScan(int timeout) {
 
  int result = 0;
  
  String buffer = "";
  
  String addressTemp;
  
  for (byte i = 0; i < 5; i++) _addresses[i] = "";
  
  _numAddresses = 0;
    
  knownStart();
  bt.print("SCAN "); bt.print(timeout); bt.print("\r");
  bt.flush();
  
  unsigned long loopStart = millis();
  unsigned long loopTimeout = timeout * 1300;
  
  while (loopStart + loopTimeout > millis()) {
  
    if (bt.available() >0) buffer.concat(char(bt.read()));
    
    if (buffer.endsWith(endOfLine)) {
      
      if (buffer.startsWith("OK")) return (btResult)result;
      
      if (buffer.startsWith("ER")) return MODULE_ERROR;
      
      if (buffer.startsWith("SC")) {
        
        addressTemp = buffer.substring(5,17);
       
        buffer = "";
        
        if (_numAddresses == 0) {
          
          _addresses[0] = addressTemp;
          
          _numAddresses++;
         
          result = (btResult)1;
       
        }
        else {
          
          for (uint8_t i = 0; i < _numAddresses; i++) {
           
            if (addressTemp == _addresses[i]) {
          
              addressTemp = "x";
          
              break;
         
            }
         
          }
          
          if (addressTemp != "x") {
          
            _addresses[uint8_t(_numAddresses++)] = addressTemp;
          
            result++;
         
          }
          
          if (_numAddresses == 5) return (btResult)result;
        
        }
      
      }
   
    }
 
  }
  
  return TIMEOUT_ERROR;

}

BluetoothManager::btResult BluetoothManager::enterDataMode() {

  return stdCmd("ENTER_DATA");

}

BluetoothManager::btResult BluetoothManager::exitDataMode(int guardDelay) {
  String buffer;
 
  delay(guardDelay);
  
  bt.print("$$$$");
 
  bt.flush();
  
  unsigned long loopStart = millis();
  
  while (loopStart + 2000 > millis()) {  
  
    if (bt.available() > 0) buffer.concat(char(bt.read()));

    if (buffer.endsWith(endOfLine)) {
     
      if (buffer.startsWith("OK")) return SUCCESS;
     
      buffer = "";
    
    }    
  
  }
  
  return TIMEOUT_ERROR;

}

BluetoothManager::btResult BluetoothManager::connect(char index, profiles connection) {
 
  if (index >= _numAddresses) return INVALID_PARAM;
 
  else return connect(_addresses[uint8_t(index)], connection);

}

BluetoothManager::btResult BluetoothManager::connect(String address, profiles connection) {

  if (address.length() != 12) return INVALID_PARAM;

  String buffer;
  
  switch(connection) {
    case SPP:
      buffer = " SPP";
      break;
    case BLE:
      buffer = " BLE";
      break;
    case A2DP:
      buffer = " A2DP";
      break;
    case AVRCP:
      buffer = " AVRCP";
      break;
    case HFP:
      buffer = " HFP";
      break;
    case PBAP:
      buffer = " PBAP";
      break;
    case MAP:
      buffer = " MAP";
      break;
    default:
      buffer = " SPP";
      break;
  }
  
  knownStart();
  
  bt.print("OPEN "); 
  bt.print(address);
  bt.print(buffer);
  bt.print("\r");
  bt.flush();
  
  unsigned long connectStart = millis();
  
  buffer = "";

  while (connectStart + 5000 > millis()) {
  
    if (bt.available() > 0) buffer.concat(char(bt.read()));

    if (buffer.endsWith(endOfLine)) {
      
      if (buffer.startsWith("ERROR")) return MODULE_ERROR;
      
      if (buffer.startsWith("OPEN_ERROR")) return CONNECT_ERROR;
      
      if (buffer.startsWith("PAIR_ERROR")) return REMOTE_ERROR;
      
      if (buffer.startsWith("OPEN_OK")) return SUCCESS;
     
      buffer = "";    
   
    }
  
  }
  
  return TIMEOUT_ERROR;
}

BluetoothManager::btResult BluetoothManager::inquiry(int timeout) {
  // Runs the "INQUIRY" command, with user defined timeout. Returns the number of
  //  devices found, up to 5. The response expected looks like this:
  //    INQUIRY 20FABB010272 240404 -37db
  //    INQUIRY A4D1D203A4F4 6A041C -91db
  
  // int result = 0;
  String buffer = "";
  String addressTemp;
  
  for (byte i = 0; i < 5; i++) _addresses[i] = "";
  
  _numAddresses = -1;
    
  // knownStart();

  send("INQUIRY " + String(timeout) + "\r"); 
  // bt.print(timeout);
  // bt.print("\r");
  // bt.flush();

  /*
  unsigned long loopStart = millis();
  unsigned long loopTimeout = timeout * 1300;
  
  while (loopStart + loopTimeout > millis()) {

    if (bt.available() > 0) buffer.concat(char(bt.read()));
    
    if (buffer.endsWith(endOfLine)) {
      
      if (buffer.startsWith("OK")) return (btResult)result;
      
      if (buffer.startsWith("ER")) return MODULE_ERROR;
      
      if (buffer.startsWith("IN")) {
     
        addressTemp = buffer.substring(8,20);
     
        buffer = "";

        if (_numAddresses == -1) {
       
          _addresses[0] = addressTemp;
        
          _numAddresses = 1;
       
          result = 1;
        
        }
        else {
        
          for (char i = 0; i <= _numAddresses; i++) {
        
            if (addressTemp == _addresses[i]) {
        
              addressTemp = "x";
        
              break;
       
            }
       
          }
       
          if (addressTemp != "x") {
       
            _addresses[_numAddresses++] = addressTemp;
      
            result++;
       
          }
        
          if (_numAddresses == 5) return (btResult)result;
       
        }
     
      }
   
    }
 
  }
  */
  return TIMEOUT_ERROR;
}

BluetoothManager::btResult BluetoothManager::getAddress(char index, String &address) {
  
  if (index + 1 > _numAddresses) {
   
    String tempString = "";
    
    address = tempString;
   
    return INVALID_PARAM;
 
  }
  else address = _addresses[uint8_t(index)];

  return SUCCESS;
  
}

BluetoothManager::btResult BluetoothManager::connectionState() {
  
  String buffer;
  
  btResult retVal = TIMEOUT_ERROR;
  
  knownStart();
  
  bt.print("STATUS");
  bt.print("\r");
  bt.flush();
  
  unsigned long startTime = millis();
  
  while ((startTime + 500) > millis()) {  
    
    while (bt.available() > 0) {
   
      char temp = bt.read();
  
      buffer.concat(temp);
  
      if (temp == '\r') break;
    }

    if (buffer.endsWith(endOfLine)) {
     
      if (buffer.startsWith("ST")) {
      
        if (buffer.substring(13, 15) == "ED") retVal = SUCCESS;
       
        else retVal = CONNECT_ERROR;
     
      }

     /*
      if (buffer.startsWith("LI")) {
        switch(profiles) {
          case SPP:
            if (buffer.substring(17,19) == "SP") retVal = SUCCESS;
            break;
          case BLE:
            if (buffer.substring(17,19) == "BL") retVal = SUCCESS;
            break;
          case A2DP:
            if (buffer.substring(17,19) == "A2") retVal = SUCCESS;
            break;
          case HFP:
            if (buffer.substring(17,19) == "HF") retVal = SUCCESS;
            break;
          case AVRCP:
            if (buffer.substring(17,19) == "AV") retVal = SUCCESS;
            break;
          case PBAP:
            if (buffer.substring(17,19) == "PB") retVal = SUCCESS;
            break;
          case MAP:
            if (buffer.substring(17,19) == "MA") retVal = SUCCESS;
            break;
          case ANY:
          default:
            break;
        }
      }
    */
    
      if (buffer.startsWith("OK")) return retVal;
      
      buffer = "";
    
    }
 
  }

  while (bt.available() > 0) bt.read();
  
  return retVal;
  
}
