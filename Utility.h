//
//  Utility.h
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//

#ifndef _UTILITY_H
#define _UTILITY_H

static int scaleBrightness(uint8_t inp) {
  if (inp == 10) return 255;
  else if (inp == 0) return 0;
  else if (inp == 9) return 228;
  else if (inp == 8) return 205;
  else if (inp == 7) return 180;
  else if (inp == 6) return 150;
  else if (inp == 5) return 121;
  else if (inp == 4) return 84;
  else if (inp == 3) return 43;
  else if (inp == 2) return 20;
  else if (inp == 1) return 1;
  else return 255;
}

#endif
