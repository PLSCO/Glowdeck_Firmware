//
//  GlowdeckDisplay.ino
//  Glowdeck
//
//  Created by Justin Kaufman on 12/14/2016
//  Copyright © 2016 Justin Kaufman. All rights reserved.
//

void init_lcd() {
  uint8_t left_reset = 0;
  
  pxs.setSpiPins(LCD_SCLK, LCD_MOSI, LCD_CS, LCD_RSET, LCD_DC);
  pxs.init(); pxs.setOrientation(LANDSCAPE_FLIP); 
  pinMode(LCD_BL, OUTPUT); analogWrite(LCD_BL, 0);
  pxs.setOriginAbsolute(); pxs.setBackground(0,0,0); pxs.setColor(255,255,255); pxs.clear();
  //pxs.setLineWidth(4.0);
  CLR[0] = 0; CLR[1] = 255; CLR[2] = 255;
  
  pxs.drawCompressedBitmap(121, 86, glowdeck_icon);
  int i; int p; int j; int w; 
  COL[0] = 0; 
  COL[1] = 0; 
  COL[2] = 0;

  if (digitalRead(PRESS_L) == LOW) left_reset++;
  
  int COLL[3]; int COLF[3]; int COLR[3];
  COLL[0] = 0; COLL[1] = 0; COLL[2] = 0;
  COLF[0] = 0; COLF[1] = 0; COLF[2] = 0;
  COLR[0] = 0; COLR[1] = 0; COLR[2] = 0;
  int c1 = 0; int c2 = 0; int c3 = 0;
  
  for (i = 0; i < 512; i++) {
    if (i < 86)
    {
      COLL[0] = 0; COLL[1]++; COLL[2]++;
      c1 = COLL[0]; c2 = COLL[1]; c3 = COLL[2];
    }
    else if (i < 256)
    {
      COLL[0] = 0; COLL[1]++; COLL[2]++;
      COLR[0]++; COLR[1] = 0; COLR[2]++;
      if (i < 107)
      {
        c1 = COLL[0]; c2 = COLL[1]; c3 = COLL[2];
      }
      else
      {
        c1 = COLR[0]; c2 = COLR[1]; c3 = COLR[2];
      }
    }

    else if (i < 312)
    {
      COLR[0]++; COLR[1] = 0; COLR[2]++;
      COLF[0]++; COLF[1]++; COLF[2] = 0;
      if (i < 193)
      {
        c1 = COLR[0]; c2 = COLR[1]; c3 = COLR[2];
      }
      else
      {
        c1 = COLF[0]; c2 = COLF[1]; c3 = COLF[2];
      }
    }

    else if (i < 384)
    {
      COLR[0]++; COLR[1] = 0; COLR[2]++;
      COLF[0]++; COLF[1]++; COLF[2] = 0;
      if (i < 345)
      {
        c1 = COLR[0]; c2 = COLR[1]; c3 = COLR[2];
      }
      else
      {
        c1 = COLF[0]; c2 = COLF[1]; c3 = COLF[2];
      }     
    }
    else
    {
      COLF[0]++; COLF[1]++; COLF[2] = 0;
      c1 = COLF[0]; c2 = COLF[1]; c3 = COLF[2];
    }
    
    for (int o = 0; o < 3; o++)
    {
      if (COLL[o] > 255) COLL[o] = 255;
      if (COLR[o] > 255) COLR[o] = 255;
      if (COLF[o] > 255) COLF[o] = 255;
    }
    
    for (p = 0; p < 56; p++)
    {
      if (p < 18)
      {
        leds[p] = CRGB(COLL[0], COLL[1], COLL[2]);
      }
      else if (p < 36)
      {
        leds[p] = CRGB(COLF[0], COLF[1], COLF[2]);
      }
      else
      {
        leds[p] = CRGB(COLR[0], COLR[1], COLR[2]);
      }
    }
    ledfade_icon(c1, c2, c3);
    if (i == 192) print_stream(F("glowdeck beta | v0.") + String(VER), F(""));
    if (i < 256) analogWrite(LCD_BL, i);
    FastLED.show();
    delay(1);
  }
  
  DBR = 10;
  delay(200); 
  fadeout_leds();
  COL[0] = 0; COL[1] = 0; COL[2] = 0;
  currentPalette = RainbowColors_p; 
  currentBlending = BLEND;
}

void init_ui() {
  /*---------------------
   INITIALIZE LCD
   ---------------------*/
  // scroll_timer.begin(lcd_scroll, 20*1000); 
  // delay(2500); 
  // scroll_timer.end(); 
  // pxs.setBackground(0,0,0);
  // pxs.scroll(320, SCROLL_SMOOTH | SCROLL_CLEAN);
  
  int i; 
  int c = analog_convert(DBR);
  for (i = c; i > -1; i--) {
    analogWrite(LCD_BL, i); 
    delay(1);
  }  
  pxs.clear();
  
  /*---------------------
   CREATE GLOWDECK GUI
   ---------------------*/
  //DRAW HEADER_ICON_DIVS
  pxs.setColor(130,130,130); 
  pxs.drawLine(64,12,64,28); 
  pxs.drawLine(128,12,128,28); 
  pxs.drawLine(192,12,192,28);
  pxs.drawLine(256,12,256,28);
  //DRAW STREAMS_DIV
  pxs.setColor(115,115,115); 
  pxs.drawLine(12,191,308,191);
  
  //DRAW HEADER_ICONS
  //pxs.setFont(glowdeck_icons24a); pxs.setColor(255,255,255);
  //DRAW PRIVACY_MODE_ICON
  /*
  pxs.setFont(glowdeck_icons24a); pxs.setColor(255,255,255);
  pxs.print(283, 173, "g");
  */
  update_clock();
  
  /*
  //UNCOMMENT FOR ONBOARDING
  
  if (onboard == 1)
  {
    onboarding();
    pxs.clear();
    //DRAW HEADER_ICON_DIVS
    pxs.setColor(130,130,130); pxs.drawLine(64,12,64,28); pxs.drawLine(128,12,128,28); pxs.drawLine(192,12,192,28); pxs.drawLine(256,12,256,28);
    //DRAW STREAMS_DIV
    pxs.drawLine(12,198,308,198);
    //DRAW HEADER_ICONS
    pxs.setFont(glowdeck_icons24a); pxs.setColor(255,255,255);
    fade_out(1);
    monthsLast = -1; minutesLast = -1; update_clock();
    ble_advertise(); delay(200); btc_discoverable(); delay(200); 
  }

  fade_out(6); ble_advertise();
  
  */ 

  
  for (i = 255; i > -1; i--) {
    COL[0] = 0; COL[1] = i; COL[2] = i;
    c = (i-255)*(-1);
    analogWrite(LCD_BL, c);
    FastLED.showColor(CRGB(COL[0], COL[1], COL[2])); 
    leds_icon(1);
  }
 
  COL[0] = 0; COL[1] = 0; COL[2] = 0; leds_icon(0);
  PANE = 1;
  
  // JUST UPDATED FIRMWARE
  if (EEPROM.read(133) == '1') {
    delay(10);
    char gf_flag = '2';
    if (EEPROM.read(133) != gf_flag) {
      delay(10);
      EEPROM.write(133, gf_flag);
    }
    print_info(F("* updated to v0.") + String(VER) + F(" *"));
    info_flag = true;
  }
  else {
    print_info(F("* launch app to begin *")); 
  }
  wifi_icon(1);
  info_flag = true;
  ROTATE.reset();
  bc127_status();
}

void scrollLeft() {
  pxs.setBackground(0,0,0);
  pxs.scroll(pxs.getWidth(), SCROLL_SMOOTH | SCROLL_CLEAN);
}
	
void scrollRight() {
  pxs.setBackground(0,0,0);
  pxs.scroll(-pxs.getWidth(), SCROLL_SMOOTH | SCROLL_CLEAN);
}

void lcd_scroll() {
  pxs.scroll(1, 0);
}

void streams_banner() {
  int qty = social_count + personal_count;
  
  //NO STREAMS
  if ((qty == 0) || (STR == 0) || ((BLE == 0) && (SPP == 0))) {
    if ((music != 1) || (STR_ICON == 1)) {
      pxs.setColor(0,0,0); 
      pxs.fillRectangle(0,201,320,39); 
      pxs.setColor(255,255,255); 
      STR_ICON = 0;
    }
  }
  //STREAMS
  else {
    if (qty > 0) {
      if (music == 1) clear_stream();
      String tmp = F("You have ") + String(qty) + F(" items.");
      pxs.drawCompressedBitmap(11, 200, streams_icon);
      pxs.setColor(0,0,0); 
      pxs.fillRectangle(47,200,273,40);  //CLEAR BANNER
      pxs.setColor(255,255,255); 
      pxs.setFont(Avenir18a);
      pxs.print(47, 202, tmp); 
      STR_ICON = 1;
    }
    else {
      if (music != 1) clear_stream();
    }
  }
}

void print_info(String content) {
  //MEASURE AND PRINT CONTENT STRING
  if (PANE == 1) {
    pxs.setBackground(0, 0, 0); 
    pxs.setFont(Avenir18a);  
    int width = pxs.getTextWidth(content); 
    int x1 = ((320 - width)/2);
    pxs.setColor(0, 0, 0); 
    pxs.fillRectangle(0, 144, 320, 40); 
    pxs.setColor(255,255,255); //CLEAR OLD INFO STRING
    pxs.print(x1, 144, content); //PRINT CONTENT (CENTERED)
  }
}

void clear_info() {
  if (PANE == 1) {
    pxs.setColor(0, 0, 0); 
    pxs.fillRectangle(0, 144, 320, 40); 
    pxs.setColor(255,255,255); //CLEAR OLD DATESTRING
 
    //pxs.setColor(0, 0, 0); 
    //pxs.fillRectangle(0, 145, 320, 29); 
    //pxs.setColor(255,255,255); //CLEAR OLD INFO STRING
  }
}

void print_stream(String content_a, String content_b) {
  clear_stream();
  if (content_b == F("")) {
    pxs.setFont(Avenir18a); 
    //pxs.setFont(OpenSansLight14ba); 
    pxs.setColor(255,255,255); 
    int alert_len = pxs.getTextWidth(content_a);
    int cen = ((320 - alert_len) / 2); 
    pxs.print(cen, 210, content_a);  //EVENT ALERT
    return;
  }
  else {
    pxs.setFont(Avenir18a); 
    //pxs.setFont(OpenSansLight14ba); 
    pxs.setColor(255,255,255); 
    pxs.print(12, 210, content_a); 
    int div = 12+pxs.getTextWidth(content_a)+9; //HEADER
    pxs.setColor(125,125,125); 
    pxs.drawLine(div, 209, div, 228); 
    pxs.setColor(255,255,255); //DIV
    pxs.setFont(Avenir18a); 
    //pxs.setFont(OpenSansLight14ba); 
    pxs.print(div+9, 210, content_b);  //CONTENT
    return;
  }
}

void print_track() {
  if (music == 1) {
    if (!(artist.length() >= 2)) return;
   
    int song_len = pxs.getTextWidth(song); 
    int artist_len = pxs.getTextWidth(artist); 
    int div_len = 9;

    // SONG AND ARTIST FIT IN ONE LINE TOGETHER
    if (song_len + artist_len + div_len <= 260) {
      String tmpText = song + F(" | ") + artist;
      if (trackTextLast == tmpText) return;

      clear_stream();
      if (STR_ICON == 1) {
        pxs.setColor(0,0,0);
        pxs.fillRectangle(8, 200, 28, 28);
        STR_ICON = 0;
      }
      pxs.setFont(glowdeck_icons24a);
      pxs.setColor(41,170,224);
      pxs.print(7, 200, F("N"));
      pxs.setFont(Avenir18a);
      pxs.setColor(255,255,255);
      
      music_rotate = -1;
      pxs.setColor(255,255,255); 
      pxs.print(45, 204, song); 
      int div = 45 + pxs.getTextWidth(song) + 9; //SONG
      pxs.setColor(135,135,135); 
      pxs.drawLine(div, 208, div, 228); 
      pxs.setColor(255,255,255); //DIV
      pxs.print(div+9, 204, artist);  //ARTIST
      trackTextLast = song + F(" | ") + artist;
      return;
    }
    //OUR SONG IS TOO BIG TO FIT ON ITS OWN/ROTATE BETWEEN
    else {
      music_rotate++;
      if (music_rotate > 1) music_rotate = 0;
      
      if (music_rotate == 0) {
        if (trackTextLast == song) return;

        clear_stream();
        if (STR_ICON == 1) {
          pxs.setColor(0,0,0);
          pxs.fillRectangle(8, 200, 28, 28);
          STR_ICON = 0;
        }
        pxs.setFont(glowdeck_icons24a);
        pxs.setColor(41,170,224);
        pxs.print(7, 200, F("N"));
        pxs.setFont(Avenir18a);
        pxs.setColor(255,255,255);
        
        pxs.setColor(255,255,255); 
        pxs.print(45, 204, song);
        trackTextLast = song;
      }
      else {
        if (trackTextLast == artist) return;

        clear_stream();
        if (STR_ICON == 1) {
          pxs.setColor(0,0,0);
          pxs.fillRectangle(8, 200, 28, 28);
          STR_ICON = 0;
        }
        pxs.setFont(glowdeck_icons24a);
        pxs.setColor(41,170,224);
        pxs.print(7, 200, F("N"));
        pxs.setFont(Avenir18a);
        pxs.setColor(255,255,255);
        
        pxs.setColor(255,255,255); 
        pxs.print(45, 204, artist);
        trackTextLast = artist;
      }
    }
  }
}

void clear_stream() {
  pxs.setColor(0,0,0); 
  pxs.fillRectangle(0,185,320,55); 
  pxs.setColor(255,255,255); //CLEAR STREAM
  pxs.setColor(115,115,115); 
  pxs.drawLine(12,191,308,191);
  STR_ICON = 0; 
  sta_flag = 0;
}

void ble_icon(int on) {
  // if ((BTC == 1) && (bleIconLast == F("i"))) return;

  uint8_t btcOffset = 4;
  String icon_id = F("h");
  uint8_t x = 85; uint8_t y = 4;
  pxs.setFont(glowdeck_icons24a);
  pxs.setColor(255, 255, 255);

  if ((BLE == 1) || (SPP == 1)) {
    if (BTC == 1) {
      icon_id = F("i");
      
      if (bleIconLast != F("i")) {
        pxs.setColor(0, 0, 0); 
        pxs.fillRectangle(78, 3, 50, 34); 
        pxs.setColor(255, 255, 255);
        
        pxs.print(x-btcOffset, y, icon_id);
        bleIconLast = icon_id;
        return;
      }
    }
    else {
      icon_id = F("h");
      if (bleIconLast != F("h")) {
        pxs.setColor(0, 0, 0); 
        pxs.fillRectangle(78, 3, 50, 34); 
        pxs.setColor(255, 255, 255);
        
        pxs.print(x, y, icon_id);
        bleIconLast = icon_id;
        return;
      }
    }
  }
  else {
    if (BTC == 1) {
      icon_id = F("i");
      
      if (bleIconLast != F("i")) {
        pxs.setColor(0, 0, 0); 
        pxs.fillRectangle(78, 3, 50, 34); 
        pxs.setColor(255, 255, 255);
        
        pxs.print(x-btcOffset, y, icon_id);
        bleIconLast = icon_id;
        return;
      }
    }
    else {
      if (bleIconLast != F("")) {
        pxs.setColor(0, 0, 0); 
        pxs.fillRectangle(78, 3, 50, 34); 
        pxs.setColor(255, 255, 255);
        bleIconLast = F("");
        return;
      }
    }
  }
}

void btc_icon(int on) {
  if (BTC == 1) {
    ble_icon(1);
    vol_icon(50);
  }
  else {
    vol_icon(-1);
    ble_icon(0);
  }
}

void wifi_icon(int on) {

  /*
  String icon_id = F("v");
  pxs.setFont(glowdeck_icons24a); 
  if (on == 4)
  {
    pxs.setColor(18,200,24);
  }
  else if ((on > 0) && (on < 4))
  {
    pxs.setColor(255,255,255);
  }
  else
  {
    pxs.setColor(0,0,0);
  }
  
  if (on == 1) icon_id = F("D");
  else if (on == 2) icon_id = F("E");
  else if ((on == 3) || (on == 0)) icon_id = F("F");
  else if (on == 4) icon_id = F("F");
  else icon_id = F("F");
  */
  pxs.setFont(glowdeck_icons24a);
  pxs.setColor(255, 255, 255);
  pxs.print(147, 4, F("I"));
}

void ledfade_icon(int c1, int c2, int c3) {
  pxs.setColor(c1,c2,c3);
  pxs.setFont(glowdeck_icons24a); 
  pxs.print(214, 4, F("m"));
}

void leds_icon(int on) {
  pxs.setFont(glowdeck_icons24a); 
  if (on == -1) {
    pxs.setColor(255, 255, 255);
    pxs.print(214, 4, F("l"));
  }
  else if (on == -2) {
    pxs.setColor(0, 0, 0);
    pxs.print(214, 4, F("l"));     
  }
  else {
    if (on == 1) {
      pxs.setColor(COL[0],COL[1],COL[2]);
    }
    else if (on == 0) {
      pxs.setColor(0,0,0);
    } 
    pxs.print(214, 4, F("m"));
  }
}

void charge_icon(int on) {
  if (((on < 1) || ((BLE == 0) && (SPP == 0))) || ((CHG_ICON != 0) && (CHQ == 0))) {
    pxs.setColor(0, 0, 0);
    pxs.fillRectangle(3, 3, 47, 28); 
    pxs.setColor(255, 255, 255);
    CHG_ICON = 0;
    fuelLast = BAT;
    batLast = F("");
    return;
  }
 
  String icon_id = F("b");
  uint8_t x = 13; 
  uint8_t y = 4;
  pxs.setFont(glowdeck_icons24a);
  pxs.setColor(255, 255, 255);

  if ((CHG_ICON == 0) && (CHQ != 0)) {
    pxs.print(13, 4, icon_id);
    CHG_ICON = 1;
    batLast = icon_id;
    fuelLast = BAT;
    return;
  }
  else if ((CHG_ICON == 1) && (CHQ == 0)) {
    pxs.setColor(0, 0, 0);
    pxs.fillRectangle(3, 3, 47, 28); 
    pxs.setColor(255, 255, 255);
    CHG_ICON = 0;
    batLast = F("");
    fuelLast = BAT;
    return;
  }
  else {
    if (BAT != fuelLast) {
      if (BAT > 99) icon_id = F("c");
      else if (BAT > 94) icon_id = F("g");
      else if (BAT > 69) icon_id = F("f");
      else if (BAT > 35) icon_id = F("e");
      else if ((BAT > 0) && (BAT <= 35)) icon_id = F("d");
      else if (CHQ == 1) icon_id = F("b");
      else if (BLE == 0) icon_id = F("");
      else if (BLE == 1) icon_id = F("e");
      else icon_id = F("");

      if (batLast != icon_id) {
        if (icon_id == F("")) {
          pxs.setColor(0, 0, 0);
          pxs.fillRectangle(3, 3, 47, 28); 
          pxs.setColor(255, 255, 255);
          CHG_ICON = 0;
          batLast = F("");
          fuelLast = BAT;
        }
        else {
          pxs.print(x, y, icon_id);
          batLast = icon_id;
          fuelLast = BAT;
          CHG_ICON == 2;
        }
      }
    }
  }
}

void busy_icon(int on) {
  pxs.setFont(glowdeck_icons24a); 
  if (on >= 1)
  {
    pxs.setColor(255,5,10);
  }
  else
  {
    pxs.setColor(0,0,0);
  }
  pxs.print(147, 165, F("k")); 
  //pxs.setColor(0,0,0); pxs.fillRectangle(146,190,30,10); pxs.setColor(255,255,255); 
  //pxs.setColor(130,130,130); pxs.drawLine(12,198,308,198); pxs.setColor(255,255,255);
}

void vol_icon(int lev) {
  String loud = F("B");
  if (lev == -1) {
    pxs.setColor(0, 0, 0);
    pxs.fillRectangle(280, 2, 40, 32);
    pxs.setColor(255, 255, 255);
  }
  else if (lev == 0) {
    loud = F("C");
  }
  else if ((lev > 0) && (lev <= 25)) {
    loud = F("z");
  }
  else if ((lev > 25) && (lev <= 70)) {
    loud = F("A");
  }
  else {
    loud = F("B");
  }
  
  pxs.setColor(0, 0, 0);
  pxs.fillRectangle(280, 2, 40, 32);
  pxs.setFont(glowdeck_icons24a);
  pxs.setColor(255, 255, 255);
  pxs.print(280, 4, loud);
}

void update_bright() {
  int AMB1 = analogRead(AMB_LEVEL);
  int AMB2 = analogRead(AMB_LEVEL);
  int AMB3 = analogRead(AMB_LEVEL);
  AMB = ((AMB1+AMB2+AMB3)/3);
  int tmp_dbr = amb_convert(AMB);
  if (tmp_dbr != DBR) {
    AMB = analogRead(AMB_LEVEL);
    tmp_dbr = amb_convert(AMB);
    if (tmp_dbr == DBR) return;
    if (tmp_dbr > DBR) {
      for (int o = analog_convert(DBR); o < (analog_convert(tmp_dbr)+1); o++) {
        analogWrite(LCD_BL, o); 
        delay(2);
      }
    }
    else {
      for (int o = analog_convert(DBR); o > analog_convert(tmp_dbr); o--) {
        analogWrite(LCD_BL, o); 
        delay(2);
      }
    }
    DBR = tmp_dbr;
    if ((BLE == 1) || (SPP == 1)) {
      BC127.print(F("SEND DBR:") + String(DBR) + F("^\n\r"));
    }
  }
}

int amb_convert(int inp) {  
  if (inp > 205) return 10;
  else if (inp > 179) return 9;
  else if (inp > 154) return 8;
  else if (inp > 131) return 7;
  else if (inp > 111) return 6;
  else if (inp > 83) return 5;
  else if (inp > 58) return 4;
  else if (inp > 37) return 3;
  else if (inp > 20) return 2;
  else if (inp <= 20) return 1;

  return 10;
}

















