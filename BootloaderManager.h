//
//  BootloaderManager.h
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//

#ifndef _BOOTLOADERMANAGER_H
#define _BOOTLOADERMANAGER_H

#include "GlobalClass.h"

class BootloaderManager {

  private:
    BootloaderManager();
    BootloaderManager(BootloaderManager const& copy);
    BootloaderManager& operator=(BootloaderManager const& copy);
    
  public:
    static BootloaderManager& getInstance() {
      static BootloaderManager instance;
      return instance;
    }

  public:
    static void enterBootloaderMode();

  public:
    friend class SerialManager;
  
};

#endif







