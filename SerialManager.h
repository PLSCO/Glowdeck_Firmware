//
//  SerialManager.h
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//

#ifndef _SERIALMANAGER_H
#define _SERIALMANAGER_H

#include "GlobalClass.h"


class SerialManager: public Runnable {

  public:
    // SerialManager(uint8_t baudRate);
    SerialManager(usb_serial_class *ser);
    
    void setup();
    void loop();

    // Transmit directors
    enum sendChannel { WIFI, BLUETOOTH };
    
    void setSendChannel(sendChannel newChannel);

    static void clearAll(void);

    void btleSend(String tx);
    void wifiSend(String tx);
    
    void mainHandler(String raw);

    static void mainClear(void);
    static void logEvent(String event);

    String mainBuffer;


  private:
    usb_serial_class sp;
    sendChannel channel;
    
    
  public:
    friend class BootloaderManager;
    friend class LEDManager;
    friend class BluetoothManager;
    
};

#endif
