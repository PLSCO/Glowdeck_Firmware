//
//  WifiManager.h
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//


#ifndef _WIFIMANAGER_H
#define _WIFIMANAGER_H

#include "GlobalClass.h"


class WifiManager: public Runnable {
  
  public:
    WifiManager(HardwareSerial1 *ser);

    void setup();
    void loop();

  public:
    enum baudRates { s9600bps, s19200bps, s38400bps, s57600bps, s115200bps };

  private:
    WifiManager();

  private:
    HardwareSerial1   wi;

    String            wifiBuffer;
    uint8_t           recvPrefixLength;

    
    String            accessPointName;
    String            password;
    String            wiscan[20][5];    // STORAGE ARRAY FOR FOUND WIFI NETWORKS (0:SSID,1:AUTHENTICATION TYPE,2:ENCRYPTION METHOD,3:RSSI,4:PASSWORD)
    uint8_t           ssid_id = -1;
    uint8_t           ssid_count = -1;
    uint8_t           str_errs = 0;
    String            sta_ip = "";
    uint8_t           wifiOn = 1;
    uint8_t           signalStrength = 3;

    
  public:
    friend class SerialManager;
    friend class BluetoothManager;
    friend class GlowdeckManager;

    String  send(String raw);
    void    clear();

    String  getResponse(int contentLength);
    void    handleMessage(String cmd);
    String  sendMessage(String msg, uint8_t timeout);
    void    enterCommandMode();
    boolean startATSequence();
    void    config();
    int     wifiSetup();
    boolean commandMode();
    String  shorten(String cmd);
    void    logSsid(String cmd);
    void    parseSsid(String cmd);
    void    connect(int router_id);
    void    status();
    void    ssid();
    void    ip();
    void    reset();
    boolean link();
    boolean txrx(String command);

};

#endif
