void home_pane() {

  msgPulsed = false;
  
  if (PANE != 1) {
    int i; int c = analog_convert(DBR);
    for (i = c; i > -1; i--) {
      analogWrite(LCD_BL, i); 
      delay(1);
    }
    leds_off();
  }

  if (messageOn == true) {
    messageOn = false;
    if (BLE == 1) {
      BC127.print(F("SEND SMS:0^\n\r"));
      BC127.flush();
    }
  }
  
  if (PANE == 6) {
    pxs.setColor(0,0,0); 
    pxs.fillRectangle(0, 0, 320, 240); //CLEAR ENTIRE DISPLAY  
    pxs.setColor(130,130,130); 
    pxs.drawLine(64,12,64,28); 
    pxs.drawLine(128,12,128,28);
    pxs.drawLine(192,12,192,28); 
    pxs.drawLine(256,12,256,28);
    bleIconLast = F("x");
    NOT = 0;
    
    if ((BLE == 1) || (SPP == 1)) {
      ble_icon(1);
    }
    else {
      ble_icon(0);
    }
    if (BTC == 1) {
      vol_icon(volume); 
    }
    else {
      vol_icon(-1);
    }
  }
  
  pxs.setColor(0, 0, 0); 
  pxs.fillRectangle(0, 48, 320, 187); //CLEAR DISPLAY BELOW HEADER ICONS
  pxs.setColor(115,115,115); 
  pxs.drawLine(12,190,308,190);
  minutesLast = -1;
  monthsLast = -1;
  //dateStringLast = F("x");
  wifi_icon(0);
  foreLast = F("0");
  startup = true;
  update_clock();
  
  if ((STR == 1) && ((personal_count+social_count) > 0) && ((BLE == 1) || (SPP == 1))) {
    streams_banner();
  }
  
  if (PANE != 1) {
    int i; int c = analog_convert(DBR);
    for (i = 0; i < c; i++) {
      analogWrite(LCD_BL, i); 
      delay(1);
    }
  }

  if (NOT == 1) {
    if (BLE == 1) {
      BC127.print(F("SEND NOT:0^\n\r"));
      BC127.flush();
    }
  }
  NOT = 0;
  
  if ((PANE == 6) && (MSL == 1) && (music == 1) && (BTC == 1) && (MSA == 0)) {
    music_sync_start(); 
    MSA = 1;
  }
  
  PANE = 1; 
  ROTATE.reset();
  
  if (STR == 1) {
    if (((personal_count+social_count) > 0) && ((BLE == 1) || (SPP == 1))) {
      streams_banner();
    }
    else {
      if (music == 1) {
        print_track();
      }
    }
  }
  else {
    if (STR_ICON == 1) {
      pxs.setColor(0,0,0); 
      pxs.fillRectangle(0,201,320,39); 
      pxs.setColor(255,255,255); 
      STR_ICON = 0;
    }
    else {
      if (music == 1) {
        print_track();
      }
    }
  }
}



















