//
//  BluetoothManager.h
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//

#ifndef _BLUETOOTHMANAGER_H
#define _BLUETOOTHMANAGER_H

#include "GlobalClass.h"

#define ON                  1
#define OFF                 0

#define SINK                0
#define SOURCE              1

#define SINGLE              0
#define MULTIPOINT          1
#define TWS                 2

#define MELODY_5_0          0
#define MELODY_5_7          1
#define MELODY_5_8          2
#define MELODY_6_0          3
#define MELODY_6_1          3

#define FULL_ADDRESS        0
#define PAIR_INDEX          1
#define AVAILIBLE_INDEX     2

#define PHONE_IDLE          0
#define PHONE_DIALING       1
#define PHONE_IN_CALL       2
#define PHONE_RINGING       3

class BluetoothManager: public Runnable {
  public:
    BluetoothManager(HardwareSerial2 *ser);

    void setup();
    void loop();

  public:

    // Connection types
    enum profiles { SPP, BLE, A2DP, HFP, AVRCP, PBAP, MAP, ANY };

    // Function results
    enum btResult { REMOTE_ERROR = -5,
                    CONNECT_ERROR, 
                    INVALID_PARAM,
                    TIMEOUT_ERROR,
                    MODULE_ERROR, 
                    DEFAULT_ERR, 
                    SUCCESS };

    // Audio commands
    enum audioCmds { PLAY, PAUSE, FORWARD, BACK, UP, DOWN, STOP };
    
    // Valid baud rates
    enum baudRates { s9600bps, s19200bps, s38400bps, s57600bps, s115200bps };

  public:
    bool              nextGen = false;
    String            melodyVersion;
    bool              bleConnected;
    bool              sppConnected;
    bool              mapConnected;
    bool              hfpConnected;
 
  private:
    BluetoothManager();

  private:
    HardwareSerial2   bt;

    uint8_t           recvPrefixLength;
    uint8_t           recvBuffer[256];
    String            recvProcess;
    String            endOfLine;
    
    int               _baudRate;
    String            _addresses[5];
    char              _numAddresses;
    
    btResult knownStart();

  public:
    friend class SerialManager;

    void    handleMessage(String msg);
    void    clear();
    String  sendMessage(String msg, uint8_t timeout);
    String  send(String raw);

    
  public:
    void unitTest();

    btResult reset();
    btResult restore();
    btResult writeConfig();
    btResult inquiry(int timeout);
    btResult connect(char index, profiles connection);
    btResult connect(String address, profiles connection);
    btResult getAddress(char index, String &address);
    btResult exitDataMode(int guardDelay = 420);
    btResult enterDataMode();
    btResult BLEDisable();
    btResult BLECentral();
    btResult BLEPeripheral();
    btResult BLEAdvertise();
    btResult BLENoAdvertise();
    btResult BLEScan(int timeout);
    btResult setBaudRate(baudRates newSpeed);
    btResult musicCommands(audioCmds command);
    btResult addressQuery(String &address);
    btResult setClassicSink();
    btResult setClassicSource();
    btResult stdGetParam(String command, String *param);
    btResult stdSetParam(String command, String param);
    btResult stdCmd(String command);
    btResult connectionState();
};

#endif
