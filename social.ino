//
//  Glowdeck_Stream.ino
//  Glowdeck
//
//  Created by Justin Kaufman on 12/14/2016
//  Copyright © 2016 Justin Kaufman. All rights reserved.
//

void notificationHandler(String cmd) {
  
  int a1 = cmd.indexOf(F(":"));
  int a2 = cmd.indexOf(F("|"));
  int a3 = cmd.indexOf(F("|"), a2+1);
  int a4 = cmd.indexOf(F("|"), a3+1);
  int a5 = cmd.indexOf(F("^"));
  
  String srcTmp = cmd.substring(a1+1, a2);
  String headTmp = cmd.substring(a2+1, a3);
  String detTmp = cmd.substring(a3+1, a4);
  String timeTmp = cmd.substring(a4+1, a5);

  // STILL NEED MORE DATA, RETURN UNTIL ALL DATA RETRIEVED
  if ((srcTmp.indexOf(F("RECV")) != -1) || (headTmp.indexOf(F("RECV")) != -1) || (detTmp.indexOf(F("RECV")) != -1) || (timeTmp.indexOf(F("RECV")) != -1)) return;

  if ((ANM != -1) || (ANA != 0)) {
    animationStop();
  }

  if ((MSL == 1) || (MSA != 0)) {
    music_sync_stop();
    MSL = 0;
    MSA = 0;
  }

  displayNotification(srcTmp, headTmp, detTmp, timeTmp);
  
  BC127.print(F("SEND NOT:1^\n\r")); 
  BC127.flush();

  notificationTimer.begin(notificationExpired, 3500000);
  
}

void notificationExpired() {

  notificationTimer.end();
  
  if (NOT == 1) {
    
    if (BLE == 1) {
      
      BC127.print(F("SEND NOT:0^\n\r"));
      
      BC127.flush();
      
    }

    NOT = 0;
    
  }
    
}

void displayNotification(String src, String head, String det, String elapsed) {
  
  // 1. Fade-out LCD for pane transition
  STR_ICON = 0;
  
  int i; 
  int c = analog_convert(DBR);
  for (i = c; i > -1; i--) {
    analogWrite(LCD_BL, i); 
    delay(1);
  }
  
  minutesLast = -1; 
  foreLast = F("x"); 
  // dateStringLast = F("x");
  
  // 2. With display dark, clear display below the header row
  pxs.setColor(0, 0, 0); 
  pxs.fillRectangle(0, 55, 320, 185);

  // 3. Draw the streams pane interface
  pxs.setColor(125,125,125); 
  pxs.drawLine(12, 96, 308, 96);      // DRAW LOWER DIV
  pxs.setColor(115,115,115); 
  pxs.drawLine(12,191,308,191);       // DRAW UPPER DIV
  pxs.drawCompressedBitmap(11, 200, streams_icon);

  // 4. Print the notification content
  pxs.setColor(0,0,0); 
  pxs.fillRectangle(8, 59, 38, 33);  // ERASE ICON
  pxs.setColor(41,170,224); 
  pxs.drawIcon(11, 59, news_icon);   // DRAW NEWS ICON
  pxs.setColor(0,0,0); 
  pxs.fillRectangle(53,51,267,45);   // ERASE SOURCE/LAPSED
  pxs.setColor(0,0,0); 
  pxs.fillRectangle(0,96,320,144);   // ERASE CONTENT
  
  // MEASURE SOURCE
  pxs.setColor(255,255,255); 
  pxs.setFont(Avenir18a);

  uint16_t source_width = pxs.getTextWidth(src);
  
  if (source_width > 215) {
    for (int i = (src.length()-1); i > 0; i--) {
      String tmpSrc = src.substring(0, i);
      if (pxs.getTextWidth(tmpSrc) <= 215) {
        src = tmpSrc;
        break;
      }
    }
    /*
    int sp1 = src.indexOf(F(" "));
    int sp2 = src.indexOf(F(" "), sp1+1);
    int sp3 = src.indexOf(F(" "), sp2+1);
    int sp4 = src.indexOf(F(" "), sp3+1);
    
    if (sp4 != -1) {
      src = src.substring(0, sp4);
      if (pxs.getTextWidth(src) > 215) {
        src = src.substring(0, sp3);
        if (pxs.getTextWidth(src) > 215) {
          src = src.substring(0, sp2);   
          if (pxs.getTextWidth(src) > 215) {
            src = src.substring(0, sp1);
            if (pxs.getTextWidth(src) > 215) {
              int src_len = (src.length() - 1); 
              int e;
              for (e = src_len; e > 0; e--) {
                src = src.substring(0, e);
                if (pxs.getTextWidth(src) <= 215) {
                  break;
                }
              }
            }
          }
        }
      }
    }
    else if (sp3 != -1) {
      src = src.substring(0, sp3);
      if (pxs.getTextWidth(src) > 215) {
        src = src.substring(0, sp2);   
        if (pxs.getTextWidth(src) > 215) {
          src = src.substring(0, sp1);
          if (pxs.getTextWidth(src) > 215) {
            int src_len = (src.length() - 1); 
            int e;
            for (e = src_len; e > 0; e--) {
              src = src.substring(0, e);
              if (pxs.getTextWidth(src) > 215) {
                break;
              }
            }
          }
        }
      }     
    }
    else if (sp2 != -1) {
      src = src.substring(0, sp2);   
      if (pxs.getTextWidth(src) > 215) {
        src = src.substring(0, sp1);
        if (pxs.getTextWidth(src) > 215) {
          int src_len = (src.length() - 1); 
          int e;
          for (e = src_len; e > 0; e--) {
            src = src.substring(0, e);
            if (pxs.getTextWidth(src) > 215) {
              break;
            }
          }
        }
      }   
    }
    else if (sp1 != -1) {
      src = src.substring(0, sp1);
      if (pxs.getTextWidth(src) > 215) {
        int src_len = (src.length() - 1); 
        int e;
        for (e = src_len; e > 0; e--) {
          src = src.substring(0, e);
          if (pxs.getTextWidth(src) > 215) {
            break;
          }
        }
      } 
    }
    else {
      int src_len = (src.length() - 1); 
      int e;
      for (e = src_len; e > 0; e--) {
        src = src.substring(0, e);
        if (pxs.getTextWidth(src) > 215) {
          break;
        }
      }
    }
    */
  }
  
  pxs.print(53, 71, src);         // PRINT SOURCE
  pxs.setColor(200, 200, 200); 
  pxs.setFont(Avenir18a);
  pxs.print(273, 71, elapsed);    // PRINT ELAPSED
  pxs.setColor(125,125,125); 
  pxs.drawLine(12, 96, 308, 96);  // REPRINT UPPER DIV
  
  pxs.setColor(255,255,255); 
  pxs.setFont(Avenir18a); 
  int width_total = pxs.getTextWidth(det); // FORMAT CONTENT
  int ln = 0;
  String line[3];
  if (width_total > 245) {
    int p; 
    int len = det.length();
    for (p = 0; p < len; p++) {
      line[ln] += det.substring(p, p+1);
      if (pxs.getTextWidth(line[ln]) >= 245) {
        ln++;
        if (ln > 2) {
          ln = 2; 
          break;
        }
      }
    }
  }
  else {
    ln = 0; 
    line[0] = det;
  }
 
  if (ln == 0) {
    pxs.print(24, 103, line[0]);
  }
  else if (ln == 1) {
    if ((!line[0].endsWith(F(" "))) && (!line[1].startsWith(F(" ")))) {
      line[0] += F("-");
    }
    pxs.print(24, 103, line[0]);
    if (line[1].startsWith(F(" "))) line[1] = line[1].substring(1, line[1].length());
    pxs.print(24, 131, line[1]);
  }
  else if (ln == 2) {
    if ((!line[0].endsWith(F(" "))) && (!line[1].startsWith(F(" ")))) {
      line[0] += F("-");
    }
    if ((!line[1].endsWith(F(" "))) && (!line[2].startsWith(F(" ")))) {
      line[1] += F("-");
    }
   
    pxs.print(24, 103, line[0]);
    if (line[1].startsWith(F(" "))) line[1] = line[1].substring(1, line[1].length());
    pxs.print(24, 131, line[1]);
    if (line[2].startsWith(F(" "))) line[2] = line[2].substring(1, line[2].length());
    pxs.print(24, 159, line[2]);
  }
  
  if (music == 1) {
    print_track();
  }
  else {
    hours = hour(); 
    minutes = minute(); 
    String addZ = F(""); 
    String ampm;
    
    if ((hours >= 0) && (hours <= 11)) {
      ampm = F("AM");
    }
    else {
      ampm = F("PM");
    }
    
    if (minutes < 10) addZ = F("0");
    if (forecast.length() >= 2) {
      social_banner(hrConvert(hours) + F(":") + addZ + minutes + F(" ") + ampm + F(" | ") + forecast + F(" | ") + temperature);
    }
    else {
      social_banner(hrConvert(hours) + F(":") + addZ + minutes + F(" ") + ampm);
    }
  }
  
  // FADE-IN LCD
  for (i = 0; i < c; i++) {
    analogWrite(LCD_BL, i); 
    delay(1);
  }

  PANE = 4;

  if (msgPulsed == false) {

    msgPulsed = true;
    
    msg_pulse();
    
    fade_pulse();
  
  }
  
  ROTATE.reset();
 
}

void social_pane() {
  // 1. Fade-out LCD for pane transition
  STR_ICON = 0;
  int i; int c = analog_convert(DBR);
  for (i = c; i > -1; i--) {
    analogWrite(LCD_BL, i); 
    delay(1);
  }
  minutesLast = -1;
  
  // 2. With display dark, clear display below the header row
  pxs.setColor(0,0,0); 
  pxs.fillRectangle(0, 55, 320, 185);

  // 3. Draw the streams pane interface
  pxs.setColor(125,125,125); pxs.drawLine(12, 96, 308, 96); // DRAW LOWER DIV
  pxs.setColor(115,115,115); pxs.drawLine(12,191,308,191); // DRAW UPPER DIV
  pxs.drawCompressedBitmap(11, 200, streams_icon);
  
  if ((social_counter < 0) || (social_counter > 14)) social_counter = 0;
  social_print();
  
  //FADE-IN LCD
  for (i = 0; i < c; i++)
  {
    analogWrite(LCD_BL, i); delay(1);
  }
  PANE = 4; ROTATE.reset();
  return;
}
  
void social_print() {
  if ((social_counter < 0) || (social_counter > 14)) {
    social_counter = 0;
  }

  /*
  if (social[social_counter][0] == F("N")) {
    
    if (social_last != F("N")) {
      pxs.setColor(0,0,0);
      pxs.fillRectangle(8,59,38,33); //ERASE ICON
      social_last = F("N");
    }
    
    pxs.setColor(0,0,0); pxs.fillRectangle(8, 59, 38, 33); //ERASE ICON
    pxs.setColor(41,170,224); pxs.drawIcon(11, 59, news_icon); //DRAW NEWS ICON
    
    // pxs.setColor(0,0,0); pxs.fillRectangle(47,200,273,40);  //CLEAR BANNER
    // pxs.setColor(255,255,255); pxs.setFont(Avenir18a);
    // pxs.print(47, 202, F(""));
  }
  else if (social[social_counter][0] == F("T")) {
    
    if (social_last != F("T")) {
      pxs.setColor(0,0,0); pxs.fillRectangle(8, 59, 38, 33); //ERASE ICON
      social_last = F("T");
    }
    
    
    pxs.setColor(0,0,0); pxs.fillRectangle(8, 59, 38, 33); //ERASE ICON
    pxs.setColor(41,170,224); pxs.drawIcon(8, 62, twitter_icon); //DRAW TWITTER ICON
  }
  else if (social[social_counter][0] == F("E")) {
    pxs.setColor(0,0,0); pxs.fillRectangle(8, 59, 38, 33); //ERASE ICON
    pxs.setColor(41,170,224); pxs.drawIcon(8, 62, email_icon); //DRAW EMAIL ICON    
  }
  else {
    social_counter++;
    if ((social_counter > social_count) || (social_counter > 14)) {
      social_counter = 0;
    }
    pxs.setColor(0,0,0); pxs.fillRectangle(8, 59, 38, 33); //ERASE ICON
    pxs.setColor(41,170,224); pxs.drawIcon(11, 59, news_icon); //DRAW NEWS ICON
  }
  
  pxs.setColor(0,0,0); pxs.fillRectangle(53,51,267,45); //ERASE SOURCE/LAPSED
  pxs.setColor(0,0,0); pxs.fillRectangle(0,96,320,144); //ERASE CONTENT
  
  // MEASURE SOURCE
  pxs.setColor(255,255,255); pxs.setFont(Avenir18a);
  
  // CONDENSE SOURCE
  if (social[social_counter][1].indexOf("New York Times") != -1) {
    social[social_counter][1] = F("NYTimes");
  }
  else if (social[social_counter][1].startsWith("The ")) {
    int len_the = social[social_counter][1].length();
    social[social_counter][1] = social[social_counter][1].substring(4, len_the);
  }
  else if (social[social_counter][1].endsWith(".com")) {
    int len_com = social[social_counter][1].length() - 4;
    social[social_counter][1] = social[social_counter][1].substring(0, len_com);
  }
  
  uint16_t source_width = pxs.getTextWidth(social[social_counter][1]);
  if (source_width > 215) {
    int sp1 = social[social_counter][1].indexOf(F(" "));
    int sp2 = social[social_counter][1].indexOf(F(" "), sp1+1);
    int sp3 = social[social_counter][1].indexOf(F(" "), sp2+1);
    int sp4 = social[social_counter][1].indexOf(F(" "), sp3+1);
    
    if (sp4 != -1) {
      social[social_counter][1] = social[social_counter][1].substring(0, sp4);
      if (pxs.getTextWidth(social[social_counter][1]) > 215) {
        social[social_counter][1] = social[social_counter][1].substring(0, sp3);
        if (pxs.getTextWidth(social[social_counter][1]) > 215) {
          social[social_counter][1] = social[social_counter][1].substring(0, sp2);   
          if (pxs.getTextWidth(social[social_counter][1]) > 215) {
            social[social_counter][1] = social[social_counter][1].substring(0, sp1);
            if (pxs.getTextWidth(social[social_counter][1]) > 215) {
              int src_len = (social[social_counter][1].length() - 1); 
              int e;
              for (e = src_len; e > 0; e--) {
                social[social_counter][1] = social[social_counter][1].substring(0, e);
                if (pxs.getTextWidth(social[social_counter][1]) <= 215) {
                  break;
                }
              }
            }
          }
        }
      }         
    }
    else if (sp3 != -1) {
      social[social_counter][1] = social[social_counter][1].substring(0, sp3);
      if (pxs.getTextWidth(social[social_counter][1]) > 215) {
        social[social_counter][1] = social[social_counter][1].substring(0, sp2);   
        if (pxs.getTextWidth(social[social_counter][1]) > 215) {
          social[social_counter][1] = social[social_counter][1].substring(0, sp1);
          if (pxs.getTextWidth(social[social_counter][1]) > 215) {
            int src_len = (social[social_counter][1].length() - 1); int e;
            for (e = src_len; e > 0; e--) {
              social[social_counter][1] = social[social_counter][1].substring(0, e);
              if (pxs.getTextWidth(social[social_counter][1]) > 215) {
                break;
              }
            }
          }
        }
      }     
    }
    else if (sp2 != -1) {
      social[social_counter][1] = social[social_counter][1].substring(0, sp2);   
      if (pxs.getTextWidth(social[social_counter][1]) > 215) {
        social[social_counter][1] = social[social_counter][1].substring(0, sp1);
        if (pxs.getTextWidth(social[social_counter][1]) > 215) {
          int src_len = (social[social_counter][1].length() - 1); 
          int e;
          for (e = src_len; e > 0; e--) {
            social[social_counter][1] = social[social_counter][1].substring(0, e);
            if (pxs.getTextWidth(social[social_counter][1]) > 215) {
              break;
            }
          }
        }
      }   
    }
    else if (sp1 != -1) {
      social[social_counter][1] = social[social_counter][1].substring(0, sp1);
      if (pxs.getTextWidth(social[social_counter][1]) > 215) {
        int src_len = (social[social_counter][1].length() - 1); 
        int e;
        for (e = src_len; e > 0; e--) {
          social[social_counter][1] = social[social_counter][1].substring(0, e);
          if (pxs.getTextWidth(social[social_counter][1]) > 215) {
            break;
          }
        }
      } 
    }
    else {
      int src_len = (social[social_counter][1].length() - 1); 
      int e;
      for (e = src_len; e > 0; e--) {
        social[social_counter][1] = social[social_counter][1].substring(0, e);
        if (pxs.getTextWidth(social[social_counter][1]) > 215) {
          break;
        }
      }
    }
  }
  
  pxs.print(53, 71, social[social_counter][1]); //PRINT SOURCE
  pxs.setColor(200,200,200); 
  pxs.setFont(Avenir18a);
  pxs.print(273, 71, social[social_counter][4]); //PRINT ELAPSED
  pxs.setColor(125,125,125); 
  pxs.drawLine(12, 96, 308, 96); //REPRINT UPPER DIV
  
  pxs.setColor(255,255,255); pxs.setFont(Avenir18a); int width_total = pxs.getTextWidth(social[social_counter][3]); //FORMAT CONTENT
  int ln = 0;  String line[3];
  if (width_total > 245) {
    int p; int len = social[social_counter][3].length();
    for (p = 0; p < len; p++) {
      line[ln] += social[social_counter][3].substring(p, p+1);
      if (pxs.getTextWidth(line[ln]) >= 245) {
        ln++; 
        if (ln > 2) {
          ln = 2; break;
        }
      }
    }
  }
  else {
    ln = 0; line[0] = social[social_counter][3];
  }
 
  if (ln == 0) {
    pxs.print(24, 103, line[0]);
  }
  else if (ln == 1) {
   if ((!line[0].endsWith(F(" "))) && (!line[1].startsWith(F(" ")))) {
     line[0] += F("-");
   }
   pxs.print(24, 103, line[0]);
   if (line[1].startsWith(F(" "))) line[1] = line[1].substring(1, line[1].length());
   pxs.print(24, 131, line[1]);
  }
  else if (ln == 2) {
   if ((!line[0].endsWith(F(" "))) && (!line[1].startsWith(F(" ")))) {
     line[0] += F("-");
   }
   if ((!line[1].endsWith(F(" "))) && (!line[2].startsWith(F(" ")))) {
     line[1] += F("-");
   }
   
   pxs.print(24, 103, line[0]);
   if (line[1].startsWith(F(" "))) line[1] = line[1].substring(1, line[1].length());
   pxs.print(24, 131, line[1]);
   if (line[2].startsWith(F(" "))) line[2] = line[2].substring(1, line[2].length());
   pxs.print(24, 159, line[2]);
   
  }
  */
  /*
  if (social[social_counter][0] == F("N")) {
    pxs.drawCompressedBitmap(240, 180, gnews_icon); // DRAW GOOGLE NEWS LOGO FOR ATTRIBUTION     
  }
  */
  /*
  if (music == 1) {
    print_track();
  }
  else {
    hours = hour(); 
    minutes = minute(); 
    String addZ = F(""); 
    String ampm;
    
    if ((hours >= 0) && (hours <= 11)) {
      ampm = F("AM");
    }
    else {
      ampm = F("PM");
    }
    
    if (minutes < 10) addZ = F("0");
    if (forecast.length() >= 2) {
      social_banner(hrConvert(hours) + F(":") + addZ + minutes + F(" ") + ampm + F(" | ") + forecast + F(" | ") + temperature);
    }
    else {
      social_banner(hrConvert(hours) + F(":") + addZ + minutes + F(" ") + ampm);
    }
  }
  */
}

void social_next() {
  /*
  social_counter++;
  if ((social_counter > social_count) || (social_counter > 14) || (social[social_counter][0] == F("X"))) {
    social_counter = 0;
  }
  social_print();
  */
}

void social_back() {
  social_counter--; 
  social_counter--; 
  social_next();
}

void social_banner(String txt) {
  clear_stream();
  pxs.setColor(0,0,0); 
  pxs.fillRectangle(8, 205, 28, 35);
  pxs.setColor(115,115,115); 
  pxs.drawLine(12,190,308,190);
  pxs.drawCompressedBitmap(11, 200, streams_icon);
  
  STR_ICON = 1;
  
  pxs.setColor(0,0,0); 
  pxs.fillRectangle(47,205,273,35);  //CLEAR BANNER
  pxs.setColor(225,225,225); 
  pxs.setFont(Avenir18a); 
  pxs.print(47, 202, txt);
}
















