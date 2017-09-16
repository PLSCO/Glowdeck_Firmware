//
//  GlowdeckLights.ino
//  Glowdeck
//
//  Created by Justin Kaufman on 12/14/2016
//  Copyright © 2016 Justin Kaufman. All rights reserved.
//

void init_leds() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

void adjustBrightness(uint8_t newVal) {
  if (newVal > VIB) {
    do {
      VIB++; 
      FastLED.setBrightness(VIB);
      FastLED.delay(1);
    } while (VIB < newVal);
  }
  else {
    do {
      VIB--; 
      FastLED.setBrightness(VIB); 
      FastLED.delay(1);
    } while (VIB > newVal);
  }
}

void light_pad() {
  int rate = 45;
  int i, j, x, y, z;
  int c0 = COL[0]; 
  int c1 = COL[1]; 
  int c2 = COL[2];
  leds_icon(1);
  
  for (j = 0; j < rate; j++) {
    for (i = 0; i < NUM_LEDS; i++) {    
      x = ( (leds[i].r) + (c0 - leds[i].r) * (j/rate) );
      y = ( (leds[i].g) + (c1 - leds[i].g) * (j/rate) );
      z = ( (leds[i].b) + (c2 - leds[i].b) * (j/rate) );
      leds[i] = CRGB(x, y, z);
    }
    FastLED.show();
    if ((x == c0) && (y == c1) && (z == c2)) {
      break;
    }
  }
  if ((c0 == 0) && (c1 == 0) && (c2 == 0)) {
    leds_icon(0);
  }
}

void colorPick() {
  
  int i, j, x, y, z;
  uint8_t rate = 44;
  leds_icon(1);
  
  for (j = 0; j < rate; j++) {
    for (i = 0; i < NUM_LEDS; i++) {    
      x = ( (leds[i].r) + (COL[0] - leds[i].r) * (j/rate) );
      y = ( (leds[i].g) + (COL[1] - leds[i].g) * (j/rate) );
      z = ( (leds[i].b) + (COL[2] - leds[i].b) * (j/rate) );
      leds[i] = CRGB(x, y, z);
    }
    FastLED.show();
    if ((x == COL[0]) && (y == COL[1]) && (z == COL[2])) {
      break;
    }
  }
  if ((COL[0] == 0) && (COL[1] == 0) && (COL[2] == 0)) {
    leds_icon(0);
  }
}

void cross_fade(int c0, int c1, int c2, int speed) {  
  int i, j, x, y, z;
 
  leds_icon(1);
  
  for (j = 0; j < speed; j++) {
    for (i = 0; i < NUM_LEDS; i++) {    
      x = ( (leds[i].r) + (c0 - leds[i].r) * (j/speed) );
      y = ( (leds[i].g) + (c1 - leds[i].g) * (j/speed) );
      z = ( (leds[i].b) + (c2 - leds[i].b) * (j/speed) );
      leds[i] = CRGB(x, y, z);
    }
    FastLED.show();
    if ((x == c0) && (y == c1) && (z == c2)) {
      break;
    }
  }
  if ((c0 == 0) && (c1 == 0) && (c2 == 0)) {
    leds_icon(0);
  } 
}

void msg_pulse() {
  if ((ANA != 0) || (ANM != -1)) {
    animationStop();
    //if (BLE == 1) BC127.print(F("SEND ANM:-1^\n\r"));
  }
  else if ((MSL == 1) || (MSA != 0)) {
    music_sync_stop();
    // if (BLE == 1) BC127.print(F("SEND MSL:0^\n\r"));
  }
  
  if ((COL[0] + COL[1] + COL[2] > 0) || (HUE != 999)) {
    leds_off(); 
    FastLED.clear();
  }
  
  uint8_t a = 32; uint8_t b = 162; uint8_t c = 169;
  COL[0] = a; COL[1] = b; COL[2] = c; fadecross_leds(); delay(50);
  COL[0] = 0; COL[1] = 0; COL[2] = 0; fadeout_leds(); delay(225);
  COL[0] = a; COL[1] = b; COL[2] = c; switch_leds();
}

void fade_pulse() {
  COL[0] = 0; COL[1] = 0; COL[2] = 0; fadeout_leds();
}

void leds_off() {
  //if ((COL[0] + COL[1] + COL[2] > 0) && (BLE == 1)) {
  //  BC127.print(F("SEND COL:0:0:0^\n\r"));
  //}
  
  COL[0] = 0; COL[1] = 0; COL[2] = 0; HUE = 999;
  
  //if ((ANA != 0) || (ANM != -1)) animationStop();  
  
  //if ((MSL == 1) || (MSA != 0)) music_sync_stop();

  fill_solid(leds, NUM_LEDS, CRGB(COL[0], COL[1], COL[2])); 
  
  FastLED.show();
  
  //if (BLE == 1) {
  //  BC127.print(F("SEND COL:0:0:0^\n\r"));
  //}
  
  leds_icon(0);
}

void switch_leds() {
  fill_solid(leds, NUM_LEDS, CRGB(COL[0], COL[1], COL[2]));
  if (COL[0] + COL[1] + COL[2] == 0) {
    if (NGT != 1) leds_icon(0); 
  }
  else {
    leds_icon(1);
  }
  FastLED.show();
}

void fadepad_leds() {
  int c1; 
  int c2; 
  int c3;
  
  //FADE PAD (CROSS "LIGHT")
  for (int i = 0; i < 256; i++) {
    c1 = leds[28].r; 
    c2 = leds[28].g; 
    c3 = leds[28].b;
    
    if (COL[0] > c1) c1++;
    else if (COL[0] < c1) c1--;
    
    if (COL[1] > c2) c2++;
    else if (COL[1] < c2) c2--;
    
    if (COL[2] > c3) c3++;
    else if (COL[2] < c3) c3--;
  
    fill_solid(leds, NUM_LEDS, CRGB(c1, c2, c3));
    FastLED.show();

    if (((c1 == COL[0]) && (c2 == COL[1]) && (c3 == COL[2])) || (i == 255)) {
      leds_icon(1);
      return;
    }
  }
  return; 
}

void fadepad_led(int id, int red, int green, int blue, int alpha, int sound) {
  if ((id < 0) || (id > (NUM_LEDS-1))) return;

  if (alpha == 0) {
    red = 0;
    green = 0;
    blue = 0;
  }
  
  int r = leds[id].r;
  int g = leds[id].g;
  int b = leds[id].b;
  
  if (r == red) {
    if (g == green) {
      if (b == blue) {
        return;
      }
    }
  }
  
  for (int i = 0; i < 256; i++) {
    r = leds[id].r;
    g = leds[id].g;
    b = leds[id].b;
    
    if (red > r) r++;
    else if (red < r) r--;
    
    if (green > g) g++;
    else if (green < g) g--;
    
    if (blue > b) b++;
    else if (blue < b) b--;
 
    leds[id] = CRGB(r, g, b);
    FastLED.show();

    if ((r == red) && (g == green) && (b == blue)) {
      if (sound == 1) colorTone(red, green, blue);
      return;
    }
    else if (i == 255) {
      leds[id] = CRGB(red, green, blue);
      FastLED.show();
      if (sound == 1) colorTone(red, green, blue);
      return;
    } 
  }
}

void fadecross_leds() {
  int c1; 
  int c2; 
  int c3;
  
  int l1; 
  int l2;
  int l3;
  
  int q = NUM_LEDS/2;

  // FADE CROSS
  for (int i = 0; i < 256; i++) {
    for (int p = 0; p < NUM_LEDS; p++) {
      c1 = leds[p].r;
      c2 = leds[p].g;
      c3 = leds[p].b;
      
      if (COL[0] > c1) c1++;
      else if (COL[0] < c1) c1--;
      
      if (COL[1] > c2) c2++;
      else if (COL[1] < c2)c2--;
      
      if (COL[2] > c3) c3++;
      else if (COL[2] < c3) c3--;
    
      if (p == q) {
        l1 = c1; 
        l2 = c2; 
        l3 = c3;
      }
      
      leds[p] = CRGB(c1, c2, c3);
      
      if (((c1 == COL[0]) && (c2 == COL[1]) && (c3 == COL[2])) || (i == 255)) {
        if (COL[0] + COL[1] + COL[2] <= 0) leds_icon(0);
        else leds_icon(1);

        leds[p] = CRGB(COL[0], COL[1], COL[2]);
        FastLED.show();
        
        return;
      }
    }
    ledfade_icon(l1, l2, l3);
    
    FastLED.show();
  }
}

void fadeout_leds() {
  int c1; int c2; int c3;

  //FADE OUT
  for (int i = 255; i > -1; i--) {
    for (int p = 0; p < NUM_LEDS; p++) {
      c1 = leds[p].r; c2 = leds[p].g; c3 = leds[p].b; 
      c1--; c2--; c3--;
      if (c1 < 0) c1 = 0;
      if (c2 < 0) c2 = 0;
      if (c3 < 0) c3 = 0;
      leds[p] = CRGB(c1, c2, c3);
      if (i == 0) {
        COL[0] = 0; COL[1] = 0; COL[2] = 0;
        leds[p] = CRGB(0, 0, 0);
        leds_icon(0);
        FastLED.show();
        return;
      }
      else {
        COL[0] = c1; COL[1] = c2; COL[2] = c3;
        leds[p] = CRGB(c1, c2, c3);
      }
    }
    
    FastLED.show();
  }
  COL[0] = 0; COL[1] = 0; COL[2] = 0; leds_icon(0);
  return;
}

void fadein_leds() {
  int c1; int c2; int c3;

  //FADE IN
  for (int i = 0; i < 256; i++) {
    for (int p = 0; p < NUM_LEDS; p++)
    {
      c1 = leds[p].r; c2 = leds[p].g; c3 = leds[p].b; 
      c1++; c2++; c3++;
      if (c1 > COL[0]) c1 = COL[0];
      if (c2 > COL[1]) c2 = COL[1];
      if (c3 > COL[2]) c3 = COL[2];
      leds[p] = CRGB(c1, c2, c3);
      
      if ((c1 == COL[0]) && (c2 == COL[1]) && (c3 == COL[2]))
      {
        leds[p] = CRGB(COL[0], COL[1], COL[2]); leds_icon(1);
        FastLED.show();
        return;
      }
      
    }
    ledfade_icon(c1, c2, c3);
    FastLED.show();
    delay(1);
  }
  return;
}

void crossfade_hue() {
  uint8_t countUp = 1;
  int q = (NUM_LEDS/2)-1;
  if (HUE < HUE_FROM) countUp = -1;

  for (int i = 0; i < 256; i++)
  {
    for (int p = q; p < (q+6); p++)
    {
      HUE_FROM = HUE_FROM + (1*countUp);
      leds[p].setHue(HUE_FROM);
    }
    FastLED.show();
    
    if (HUE_FROM == HUE) 
    {
      COL[0] = leds[q].r;
      COL[1] = leds[q].g;
      COL[2] = leds[q].b;
      leds_icon(1);
      return;
    }
    
    delay(1);
  }
  return;
}

void leds_pulse() {
  if ((LED == 0) || (NGT == 1)) {
    if ((COL[0] != 0) || (COL[1] != 0) || (COL[2] != 0)) {
      fadeout_leds();
      COL[0] = 0; COL[1] = 0; COL[2] = 0;
    }
    return;
  }

  int led_left = 0; 
  int led_front = 0; 
  int led_right = 0;
  int c1; 
  int c2; 
  int c3;
  
  //FADE IN LEDS
  for (int i = 0; i < 256; i++) {
    for (int p = 0; p < NUM_LEDS; p++) {
      if (p < 16) {
        c1 = leds[p].r; c2 = leds[p].g; c3 = leds[p].b; 
        c1++; c2++; c3++;
        if (c1+c2+c3 >= 510) {
          if (led_left != 1) {
            leds[p] = CRGB(0, 255, 255);
            led_left = 1;
          }
        }
        else {
          if (c1 > 255) c1 = 255;
          if (c2 > 255) c2 = 255;
          if (c3 > 255) c3 = 255;
          leds[p] = CRGB(c1, c2, c3);
        }
      }
      else if (p < 40) {
        c1 = leds[p].r; c2 = leds[p].g; c3 = leds[p].b; 
        c1++; c2++; c3++;
        if (c1+c2+c3 >= 510) {
          if (led_front != 1) {
            COL[0] = 255; COL[1] = 255; COL[2] = 0;
            leds[p] = CRGB(255, 255, 0);
            led_front = 1;
          }
        }
        else {
          if (c1 > 255) c1 = 255;
          if (c2 > 255) c2 = 255;
          if (c3 > 255) c3 = 255;
          COL[0] = c1; COL[1] = c2; COL[2] = c3;
          leds[p] = CRGB(c1, c2, c3);
        }
      }
      else {
        c1 = leds[p].r; c2 = leds[p].g; c3 = leds[p].b; 
        c1++; c2++; c3++;
        if (c1+c2+c3 >= 510) {
          if (led_right != 1) {
            leds[p] = CRGB(255, 0, 255);
            led_right = 1;
          }
        }
        else {
          if (c1 > 255) c1 = 255;
          if (c2 > 255) c2 = 255;
          if (c3 > 255) c3 = 255;
          leds[p] = CRGB(c1, c2, c3);
        }
      }
    }
    leds_icon(1);
    FastLED.show();
    if ((led_left+led_right+led_front) >= 3) {
      led_left = 0; led_right = 0; led_front = 0;
      break;
    }
    delay(1);
  }
  
  delay(100);
  
  //FADE OUT
  for (int i = 255; i > -1; i--) {
    for (int p = 0; p < NUM_LEDS; p++) {
      if (p < 16) {
        c1 = leds[p].r; c2 = leds[p].g; c3 = leds[p].b; 
        c1--; c2--; c3--;
        if ((c1 <= 0) && (c2 <= 0) && (c3 <= 0)) {
          if (led_left != 1) {
            leds[p] = CRGB(0, 0, 0);
            led_left = 1;
          }
        }
        else {
          if (c1 < 0) c1 = 0;
          if (c2 < 0) c2 = 0;
          if (c3 < 0) c3 = 0;
          leds[p] = CRGB(c1, c2, c3);
        }
      }
      else if (p < 40) {
        c1 = leds[p].r; c2 = leds[p].g; c3 = leds[p].b; 
        c1--; c2--; c3--;
        if ((c1 <= 0) && (c2 <= 0) && (c3 <= 0)) {
          if (led_front != 1)
          {
            COL[0] = 0; COL[1] = 0; COL[2] = 0;
            leds[p] = CRGB(0, 0, 0);
            led_front = 1;
          }
        }
        else {
          if (c1 < 0) c1 = 0;
          if (c2 < 0) c2 = 0;
          if (c3 < 0) c3 = 0;
          COL[0] = c1; COL[1] = c2; COL[2] = c3;
          leds[p] = CRGB(c1, c2, c3);
        }
      }
      else {
        c1 = leds[p].r; c2 = leds[p].g; c3 = leds[p].b; 
        c1--; c2--; c3--;
        if ((c1 <= 0) && (c2 <= 0) && (c3 <= 0)) {
          if (led_right != 1)
          {
            leds[p] = CRGB(0, 0, 0);
            led_right = 1;
          }
        }
        else {
          if (c1 < 0) c1 = 0;
          if (c2 < 0) c2 = 0;
          if (c3 < 0) c3 = 0;
          leds[p] = CRGB(c1, c2, c3);
        }
      }
    }
    leds_icon(1);
    FastLED.show();
    if ((led_left+led_right+led_front) >= 3) {
      led_left = 0; led_right = 0; led_front = 0;
      COL[0] = 0; COL[1] = 0; COL[2] = 0; leds_icon(0);
      break;
    }
    delay(1);
  }
  return;
}

void leds_right(int r, int g, int b, int speed) {
  if ((LED == 0) || (NGT == 1)) {
    if ((COL[0] != 0) || (COL[1] != 0) || (COL[2] != 0)) {
      COL[0] = 0; COL[1] = 0; COL[2] = 0;
      FastLED.showColor(CRGB(COL[0], COL[1], COL[2])); leds_icon(0);
    }
    return;
  }
  
  int i, j, x, y, z; 
  //COL[0] = r; COL[1] = g; COL[2] = b;
  leds_icon(1);
  
  for (j = 0; j < speed; j++) {
    for (i = 0; i < 16; i++) {    
      x = leds[i].r + (r - leds[i].r) * j/speed;
      y = leds[i].g + (g - leds[i].g) * j/speed;
      z = leds[i].b + (b - leds[i].b) * j/speed;
      leds[i] = CRGB(x, y, z);
    }
    for (i = 16; i < NUM_LEDS; i++) {
      leds[i] = CRGB(0, 0, 0);
    }
    COL[0] = x; COL[1] = y; COL[2] = z;
    leds_icon(1);
    FastLED.show();
    if ((x == r) && (y == g) && (z == b)) {
      if ((r == 0) && (g == 0) && (b == 0)) {
        COL[0] = 0; COL[1] = 0; COL[2] = 0;
        leds_icon(0);
      } 
      break;
    }
  }
}

void leds_front(int r, int g, int b, int speed) { 
  if ((LED == 0) || (NGT == 1)) {
    if ((COL[0] != 0) || (COL[1] != 0) || (COL[2] != 0)) {
      COL[0] = 0; COL[1] = 0; COL[2] = 0;
      FastLED.showColor(CRGB(COL[0], COL[1], COL[2])); leds_icon(0);
    }
    return;
  }
  
  int i, j, x, y, z; 
  //COL[0] = r; COL[1] = g; COL[2] = b;
  leds_icon(1);
  
  for (j = 0; j < speed; j++) {
    for (i = 16; i < 40; i++) {    
      x = leds[i].r + (r - leds[i].r) * j/speed;
      y = leds[i].g + (g - leds[i].g) * j/speed;
      z = leds[i].b + (b - leds[i].b) * j/speed;
      leds[i] = CRGB(x, y, z);
    }
    for (i = 0; i < 16; i++) {
      leds[i] = CRGB(0, 0, 0);
    }
    for (i = 40; i < NUM_LEDS; i++) {
      leds[i] = CRGB(0, 0, 0);
    }
    
    COL[0] = x;
    COL[1] = y;
    COL[2] = z;
    
    leds_icon(1);
    FastLED.show();
    
    if ((x == r) && (y == g) && (z == b)) {
      if ((r == 0) && (g == 0) && (b == 0)) {
        leds_icon(0);
      } 
      break;
    }
  }
}

void leds_left(int r, int g, int b, int speed) { 
  if ((LED == 0) || (NGT == 1)) {
    if ((COL[0] != 0) || (COL[1] != 0) || (COL[2] != 0)) {
      COL[0] = 0; COL[1] = 0; COL[2] = 0;
      FastLED.showColor(CRGB(COL[0], COL[1], COL[2])); leds_icon(0);
    }
    return;
  }
  
  int i, j, x, y, z; //COL[0] = r; COL[1] = g; COL[2] = b;
  leds_icon(1);
  
  for (j = 0; j < speed; j++) {
    for (i = 40; i < NUM_LEDS; i++) {    
      x = leds[i].r + (r - leds[i].r) * j/speed;
      y = leds[i].g + (g - leds[i].g) * j/speed;
      z = leds[i].b + (b - leds[i].b) * j/speed;
      leds[i] = CRGB(x, y, z);
    }
    for (i = 0; i < 40; i++) {
      leds[i] = CRGB(0, 0, 0);
    }

    COL[0] = x; COL[1] = y; COL[2] = z;
    leds_icon(1);
    
    FastLED.show();
    if ((x == r) && (y == g) && (z == b)) {
      if ((r == 0) && (g == 0) && (b == 0)) {
        leds_icon(0);
      } 
      break;
    }
  }
}

void light_swirl() {
  int q = NUM_LEDS/2;
  if (digitalRead(PRESS_F) == LOW) {
    do {
      for (int j = 0; j < 255; j++) {
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1;
        FillLEDsFromPaletteColors(startIndex);
        FastLED.show();
        COL[0] = leds[q].r; COL[1] = leds[q].g; COL[2] = leds[q].b; leds_icon(1);
        FastLED.delay(1000/100);
      } 
    } while (digitalRead(PRESS_F) == LOW);
  }
  else {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1;
    FillLEDsFromPaletteColors(startIndex);
    FastLED.show();
    COL[0] = leds[q].r; COL[1] = leds[q].g; COL[2] = leds[q].b; leds_icon(1);
    FastLED.delay(1000/100);    
  }
  for (int t = 0; t < NUM_LEDS; t++) {
    leds[t] = CRGB(0, 0, 0);
  }
  FastLED.show();
  COL[0] = 0; COL[1] = 0; COL[2] = 0;
  leds_icon(0); 
}

void chooseColorPalette() {
  uint8_t numberOfPalettes = 4;
  uint8_t secondsPerPalette = 5;
  uint8_t whichPalette = (millis()/(1000*secondsPerPalette)) % numberOfPalettes;
  CRGB r(CRGB::Red), b(CRGB::Blue), w(85,85,85), g(CRGB::Green), W(CRGB::White), l(0xE1A024);

  switch(whichPalette) {  
    case 0: // Incandescent "fairy lights"
      gPalette = CRGBPalette16( l,l,l,l, l,l,l,l, l,l,l,l, l,l,l,l );
      break;
    case 1: // Red, Green, and White
      gPalette = CRGBPalette16( r,r,r,r, r,r,r,r, g,g,g,g, w,w,w,w ); 
      break;
    case 2: // Blue and White
      //gPalette = CRGBPalette16( b,b,b,b, b,b,b,b, w,w,w,w, w,w,w,w ); 
      gPalette = CloudColors_p; // Blues and whites!
      break;
    case 3: // Rainbow of colors
      gPalette = RainbowColors_p;
      break;
    case 4: // Snow
      gPalette = CRGBPalette16( W,W,W,W, w,w,w,w, w,w,w,w, w,w,w,w );
      break;
  }
}

bool getPixelDirection(uint16_t i) {
  return directionFlags[i];
}

void setPixelDirection(uint16_t i, bool dir) {
  directionFlags[i] = dir;
}

CRGB makeBrighter(const CRGB& color, fract8 howMuchBrighter) {
  CRGB incrementalColor = color;
  incrementalColor.nscale8( howMuchBrighter);
  return color + incrementalColor;
}

CRGB makeDarker(const CRGB& color, fract8 howMuchDarker) {
  CRGB newcolor = color;
  newcolor.nscale8( 255 - howMuchDarker);
  return newcolor;
}

void brightenOrDarkenEachPixel( fract8 fadeUpAmount, fract8 fadeDownAmount) {
 for (uint16_t i = 0; i < NUM_LEDS; i++) {
    if (getPixelDirection(i) == GETTING_DARKER) {
      // This pixel is getting darker
      leds[i] = makeDarker(leds[i], fadeDownAmount);
    } 
    else {
      // This pixel is getting brighter
      leds[i] = makeBrighter(leds[i], fadeUpAmount);
      // Check to see if maxd the brightness
      if (leds[i].r == 255 || leds[i].g == 255 || leds[i].b == 255) {
        // If so, turn around and start getting darker
        setPixelDirection(i, GETTING_DARKER);
      }
    }
  }
}

void FillLEDsFromPaletteColors(uint8_t colorIndex) {
  uint8_t brightness = 255;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}

void animationStart(uint8_t type) {
  if (MSL == 1) {
    music_sync_stop();
  }
  
  if (type == -1) {
    animationStop();
    return;
  }
  else if (ANM == -1) {
    if (type < 0) ANM = 0;
    else if (type > 6) ANM = 6;
    else {
      ANM = type;
    }
  }
  else if (ANM != -1) {
    if (ANM == type) {
      return;
    }
    else {
      animationTimer.end();
      //fadeout_leds();
      FastLED.clear();
      FastLED.show();
      if (type < 0) {
        ANM = 0;
      }
      else if (type > 6) {
        ANM = 6;
      }
      else {
        ANM = type;
      }
    }
  }

  ANA = 1;
  //gCurrentPatternNumber = ANM;
  hueTimer = millis();
  animationTimer.begin(ledsAnimate, anmDelays[ANM]);
}

void animationStop() {
  animationTimer.end();

  if (BLE == 1) {
    BC127.print(F("SEND ANM:-1^\n\r"));
    BC127.flush();
  }
  ANM = -1;
  ANA = 0;

  leds_off();
  ana_mode = 0;
}

void ledsAnimate() {
  if (ANM == -1) return;

  // EVERY 20 MILLIS
  if (millis() - hueTimer > 33) {
    HUE++;
    hueTimer = millis();
  }
  
  gPatterns[ANM]();
  FastLED.show();
}

void ledShow() {
  gPatterns[ANM]();
  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND); 
  //EVERY 20 MILLIS
  if (millis() - hueTimer >= 20) {
    HUE++;
    hueTimer = millis(); 
  }
  //EVERY 7.5 SECONDS
  if (millis() - patternTimer >= 10000) {
    nextPattern(); 
    patternTimer = millis();
  }
}

void nextPattern() {
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void colorWheel() {
  FastLED.showColor(CRGB(CLR[0], CLR[1], CLR[2]));
}

void ledChargeBegin() {
  if (PANE == 1) {
    print_info(F("* wirelessly charging *")); 
    info_flag = true;
  }
  if ((BLE == 1) || (SPP == 1)) {
    BC127.print("SEND BAT^\n\r");
  }
}

void ledConnectionRoutine() {
  FastLED.clear();
  int millisStart = millis();
  int q = NUM_LEDS/2;
  do {
    for (int j = 0; j < 255; j++) {
      static uint8_t startIndex = 0;
      startIndex = startIndex + 1;
      FillLEDsFromPaletteColors(startIndex);
      FastLED.show();
      COL[0] = leds[q].r; 
      COL[1] = leds[q].g; 
      COL[2] = leds[q].b;
      FastLED.delay(3);
    } 
  } while ((millis() - millisStart) < 2375);
  
  // if (BLE == 1) {
    // BC127.print(F("STATUS\r"));
    // BC127.flush();
  // }
  
  fadeout_leds();
  ver_flag = 0;
}

void rainbow() {
  fill_rainbow(leds, NUM_LEDS, HUE, 5);
}

void rainbowWithGlitter() {
  rainbow();
  addGlitter(80);
}

void addGlitter(fract8 chanceOfGlitter) {
  if (random8() < chanceOfGlitter) {
    leds[random16(NUM_LEDS)] += CRGB::White;
  }
}

void confetti() {
  // Random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(HUE + random8(64), 200, 255);
}

void sinelon() {
  //A colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS);
  leds[pos] += CHSV(HUE, 255, 192);
}

void bpm() {
  //Colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(palette, HUE+(i*2), beat-HUE+(i*10));
  }
}

void juggle() {
  //Eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds, NUM_LEDS, 20);
  byte dothue = 0;
  for (int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void twinkle() {
  chooseColorPalette();
   
  // Make each pixel brighter or darker, depending on its 'direction' flag
  brightenOrDarkenEachPixel(FADE_IN_SPEED, FADE_OUT_SPEED);
  
  // Now consider adding a new random twinkle
  if (random8() < DENSITY) {
     int pos = random16(NUM_LEDS);
    
     if (!leds[pos]) {
       leds[pos] = ColorFromPalette( gPalette, random8(), STARTING_BRIGHTNESS, NOBLEND);
       setPixelDirection(pos, GETTING_BRIGHTER);
     } 
  }

  FastLED.show();
}

void confetti_extend() {
  //Random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS-56);
  leds[pos+55] += CHSV(HUE + random8(64), 200, 255);
}

void sinelon_extend() {
  //A colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS-56);
  leds[pos+55] += CHSV(HUE, 255, 192);
}

void bpm_extend() {
  //Colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 56; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, HUE+(i*2), beat-HUE+(i*10));
  }
}

void juggle_extend() {
  //Eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds, NUM_LEDS, 20);
  byte dothue = 0;
  for (int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void music_sync() {
  if (samplingIsDone()) {
    arm_cfft_radix4_instance_f32 fft_inst;
    arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 0, 1);
    arm_cfft_radix4_f32(&fft_inst, samples);
    arm_cmplx_mag_f32(samples, magnitudes, FFT_SIZE);
    if (LEDS_ENABLED == 1) {
      spectrumLoop();
    }
    samplingBegin();
  }
}

void music_sync_x() { 
  if (sync_routine == 0) {
    confetti();
  }
  else if (sync_routine == 1) {
    juggle();
  }
  else if (sync_routine == 2) {
    sinelon();
  }
  else if (sync_routine == 3) {
    bpm();
  }
  FastLED.show();
  
  iterations++;
  HUE = (HUE+1) % 256;
  
  if (iterations >= 325) {
    sync_routine = (sync_routine + 1) % 4;
    iterations = 0;
  }
}

void music_sync_extend() { 
  if (sync_routine == 0) {
    confetti_extend();
  }
  else if (sync_routine == 1) {
    juggle_extend();
  }
  else if (sync_routine == 2) {
    sinelon_extend();
  }
  else if (sync_routine == 3) {
    bpm_extend();
  }
  FastLED.show();
  
  iterations++;
  HUE = (HUE+1) % 256;
  
  if (iterations >= 330) {
    sync_routine = (sync_routine + 1) % 4;
    iterations = 0;
  }
}

void music_sync_start() {
  if (ANM != -1) {
    animationStop();
  }
  
  MSA = 1; 
  MSL = 1;
  
  if (BLE == 1) {
    BC127.print(F("SEND MSL:1^\n\r"));
    BC127.flush();
  }
 
  music_sync_timer.begin(music_sync, 13575);
}

void music_sync_stop() {
  if (MSL == 1) {
    music_sync_timer.end();
  
    if (BLE == 1) {
      BC127.print(F("SEND MSL:0^\n\r"));
      BC127.flush();
    }
  }
  
  MSL = 0; 
  MSA = 0;
  
  leds_off();
  msa_mode = 0;
}

void music_sync_start_demo() {
  //MSA = 1; DEM = 1;
  // if (BLE == 1) {
  //  BC127.print(F("SEND MSL:1^\n\r")); BC127.flush();
  // }
  
  //if (NUM_LEDS > 56) { 
    // music_sync_timer_demo.begin(music_sync_extend, 60000);
  //}
  // else {
    // music_sync_timer_demo.begin(music_sync, 60000);
  //}
}

void music_sync_stop_demo() {
  //music_sync_timer_demo.end();
  //fadeout_leds();
  //FastLED.clear();
  //if (BLE == 1) {
  //  BC127.print(F("SEND MSL:0^\n\r")); 
  //  BC127.flush();
  //}
  //iterations = 0; 
  //sync_routine = 0;
  //MSA = 0; DEM = 0;
  //msa_mode = 0;
}















