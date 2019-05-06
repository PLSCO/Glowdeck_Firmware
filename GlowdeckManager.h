//
//  GlowdeckManager.h
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//

#ifndef _GLOWDECKMANAGER_H
#define _GLOWDECKMANAGER_H

#include "GlobalClass.h"

class GlowdeckManager {
  public:
    GlowdeckManager(bool on);

  private:
    bool _on;
    
  public:
    String longName;
    String udid;

  protected:
    friend class WifiManager;
    friend class BluetoothManager;
    friend class DisplayManager;
};

#endif
