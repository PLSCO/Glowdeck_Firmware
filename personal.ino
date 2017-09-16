void personal_pane() {
  STR_ICON = 0;
  //FADE-OUT LCD
  int i; int c = analog_convert(DBR);
  for (i = c; i > -1; i--)
  {
    analogWrite(LCD_BL, i); delay(1);
  }
  minutesLast = -1;
  //WE ARE DIMMED, PRINT personal STREAM CONTENT, THEN RE-LIGHT LCD
  pxs.setColor(0,0,0); pxs.fillRectangle(0, 55, 320, 185); //CLEAR DISPLAY BELOW HEADER ICONS
  //DRAW COMPLETE PANE UI
  //pxs.setColor(125,125,125); pxs.drawLine(12,197,308,197); //DRAW LOWER DIV
  pxs.setColor(125,125,125); pxs.drawLine(12, 96, 308, 96); //DRAW UPPER DIV
  pxs.setColor(115,115,115); pxs.drawLine(12,191,308,191);
 
  pxs.drawCompressedBitmap(11, 200, streams_icon);
  
  if ((personal_counter < 0) || (personal_counter > 14)) personal_counter = 0;
  personal_print();
  
  //FADE-IN LCD
  for (i = 0; i < c; i++) {
    analogWrite(LCD_BL, i); delay(1);
  }
  PANE = 5; ROTATE.reset();
  return;
}
  
void personal_print() {
  if ((personal_counter < 0) || (personal_counter > 14))
  {
    personal_counter = 0;
  }

  if (personal[0] == F("N"))
  {
    /*
    if (personal_last != F("N"))
    {
      pxs.setColor(0,0,0);
      pxs.fillRectangle(8,59,38,33); //ERASE ICON
      personal_last = F("N");
    }
    */
    pxs.setColor(0,0,0); pxs.fillRectangle(8, 59, 38, 33); //ERASE ICON
    pxs.setColor(41,170,224); pxs.drawIcon(11, 59, news_icon); //DRAW NEWS ICON
    //pxs.setColor(0,0,0); pxs.fillRectangle(47,200,273,40);  //CLEAR BANNER
    //pxs.setColor(255,255,255); pxs.setFont(Avenir18a);
    //pxs.print(47, 202, F(""));
  }
  else if (personal[0] == F("T"))
  {
    /*
    if (personal_last != F("T"))
    {
      pxs.setColor(0,0,0); pxs.fillRectangle(8, 59, 38, 33); //ERASE ICON
      personal_last = F("T");
    }
    */
    pxs.setColor(0,0,0); pxs.fillRectangle(8, 59, 38, 33); //ERASE ICON
    pxs.setColor(41,170,224); pxs.drawIcon(8, 62, news_icon); //DRAW TWITTER ICON
  }
  else if (personal[0] == F("E"))
  {
    pxs.setColor(0,0,0); pxs.fillRectangle(8, 59, 38, 33); //ERASE ICON
    pxs.setColor(41,170,224); pxs.drawIcon(8, 62, news_icon); //DRAW EMAIL ICON    
  }
  else if (personal[0] == F("M"))
  {
    pxs.setColor(0,0,0); pxs.fillRectangle(8, 59, 38, 33); //ERASE ICON
    pxs.setColor(41,170,224); pxs.drawIcon(8, 62, messages_icon); //DRAW EMAIL ICON    
  }    
  else
  {
    personal_counter++;
    if ((personal_counter > personal_count) || (personal_counter > 14))
    {
      personal_counter = 0;
    }
    pxs.setColor(0,0,0); pxs.fillRectangle(8, 59, 38, 33); //ERASE ICON
    pxs.setColor(41,170,224); pxs.drawIcon(11, 59, news_icon); //DRAW NEWS ICON
  }
  
  pxs.setColor(0,0,0); pxs.fillRectangle(53,51,267,45); //ERASE SOURCE/LAPSED
  pxs.setColor(0,0,0); pxs.fillRectangle(0,96,320,144); //ERASE CONTENT
  
  pxs.setColor(255,255,255); pxs.setFont(Avenir18a);
  
  //MEASURE SOURCE
  pxs.setColor(255,255,255); pxs.setFont(Avenir18a);
  uint16_t source_width = pxs.getTextWidth(personal[1]);
  if (source_width > 215)
  {
    int sp1 = personal[1].indexOf(F(" "));
    int sp2 = personal[1].indexOf(F(" "), sp1+1);
    int sp3 = personal[1].indexOf(F(" "), sp2+1);
    int sp4 = personal[1].indexOf(F(" "), sp3+1);
    
    if (sp4 != -1)
    {
      personal[1] = personal[1].substring(0, sp4);
      if (pxs.getTextWidth(personal[1]) > 215)
      {
        personal[1] = personal[1].substring(0, sp3);
        if (pxs.getTextWidth(personal[1]) > 215)
        {
          personal[1] = personal[1].substring(0, sp2);   
          if (pxs.getTextWidth(personal[1]) > 215)
          {
            personal[1] = personal[1].substring(0, sp1);
            if (pxs.getTextWidth(personal[1]) > 215)
            {
              int src_len = (personal[1].length() - 1); int e;
              for (e = src_len; e > 0; e--)
              {
                personal[1] = personal[1].substring(0, e);
                if (pxs.getTextWidth(personal[1]) <= 215)
                {
                  break;
                }
              }
            }
          }
        }
      }         
    }
    else if (sp3 != -1)
    {
      personal[1] = personal[1].substring(0, sp3);
      if (pxs.getTextWidth(personal[1]) > 215)
      {
        personal[1] = personal[1].substring(0, sp2);   
        if (pxs.getTextWidth(personal[1]) > 215)
        {
          personal[1] = personal[1].substring(0, sp1);
          if (pxs.getTextWidth(personal[1]) > 215)
          {
            int src_len = (personal[1].length() - 1); int e;
            for (e = src_len; e > 0; e--)
            {
              personal[1] = personal[1].substring(0, e);
              if (pxs.getTextWidth(personal[1]) > 215)
              {
                break;
              }
            }
          }
        }
      }     
    }
    else if (sp2 != -1)
    {
      personal[1] = personal[1].substring(0, sp2);   
      if (pxs.getTextWidth(personal[1]) > 215)
      {
        personal[1] = personal[1].substring(0, sp1);
        if (pxs.getTextWidth(personal[1]) > 215)
        {
          int src_len = (personal[1].length() - 1); int e;
          for (e = src_len; e > 0; e--)
          {
            personal[1] = personal[1].substring(0, e);
            if (pxs.getTextWidth(personal[1]) > 215)
            {
              break;
            }
          }
        }
      }   
    }
    else if (sp1 != -1)
    {
      personal[1] = personal[1].substring(0, sp1);
      if (pxs.getTextWidth(personal[1]) > 215)
      {
        int src_len = (personal[1].length() - 1); int e;
        for (e = src_len; e > 0; e--)
        {
          personal[1] = personal[1].substring(0, e);
          if (pxs.getTextWidth(personal[1]) > 215)
          {
            break;
          }
        }
      } 
    }
    else
    {
      int src_len = (personal[1].length() - 1); int e;
      for (e = src_len; e > 0; e--)
      {
        personal[1] = personal[1].substring(0, e);
        if (pxs.getTextWidth(personal[1]) > 215)
        {
          break;
        }
      }
    }
  }
  
  pxs.print(53, 71, personal[1]); //PRINT SOURCE
  
  pxs.setColor(200,200,200); pxs.setFont(Avenir18a); 
  pxs.print(273, 71, personal[4]); //PRINT ELAPSED
  pxs.setColor(125,125,125); pxs.drawLine(12, 96, 308, 96); //REPRINT UPPER DIV
  
  pxs.setColor(255,255,255); pxs.setFont(Avenir18a); int width_total = pxs.getTextWidth(personal[3]); //FORMAT CONTENT
  int ln = 0;  String line[3];
  if (width_total > 245)
  {
    int p; int len = personal[3].length();
    for (p = 0; p < len; p++)
    {
      line[ln] += personal[3].substring(p, p+1);
      if (pxs.getTextWidth(line[ln]) >= 245)
      {
        ln++; 
        if (ln > 2) 
        {
          ln = 2; break;
        }
      }
    }
  }
  else
  {
    ln = 0; line[0] = personal[3];
  }
 
  if (ln == 0)
  {
    pxs.print(24, 103, line[0]);
  }
  else if (ln == 1)
  {
   if ( (!line[0].endsWith(F(" "))) && (!line[1].startsWith(F(" "))) ) 
   {
     line[0] += F("-");
   }
   pxs.print(24, 103, line[0]);
   if (line[1].startsWith(F(" "))) line[1] = line[1].substring(1, line[1].length());
   pxs.print(24, 131, line[1]);
  }
  else if (ln == 2)
  {
   if ( (!line[0].endsWith(F(" "))) && (!line[1].startsWith(F(" "))) )
   {
     line[0] += F("-");
   }
   if ( (!line[1].endsWith(F(" "))) && (!line[2].startsWith(F(" "))) )
   {
     line[1] += F("-");
   }
    pxs.print(24, 103, line[0]);
    if (line[1].startsWith(F(" "))) line[1] = line[1].substring(1, line[1].length());
    pxs.print(24, 131, line[1]);  
    if (line[2].startsWith(F(" "))) line[2] = line[2].substring(1, line[2].length());
    pxs.print(24, 159, line[2]);   
  }
  
  /*
  if (personal[0] == F("N"))
  {
    pxs.drawCompressedBitmap(240, 180, gnews_icon); //DRAW GOOGLE NEWS LOGO FOR ATTRIBUTION     
  }
  */
  
  if ((music == 1) && (artist.length() >= 1))
  {
    print_track();
  }
  else
  {
    hours = hour(); minutes = minute(); String addZ = F(""); String ampm;
    if ((hours >= 0) && (hours <= 11))
    {
      ampm = F("AM");
    }
    else
    {
      ampm = F("PM");
    }
    if (minutes < 10) addZ = F("0");
    
    if (forecast.length() >= 2)
    {
      personal_banner(hrConvert(hours) + F(":") + addZ + minutes + " " + ampm + " | " + forecast + " | " + temperature);
    }
    else
    {
      personal_banner(hrConvert(hours) + F(":") + addZ + minutes + " " + ampm);
    }
  }
}

void personal_next() {  
  personal_counter++;
  if ( (personal_counter > personal_count) || (personal_counter > 14) || (personal[0] == F("X")))
  {
    personal_counter = 0;
  }
  personal_print();
}

void personal_back() {
  personal_counter--; personal_counter--; personal_next();
}

void personal_banner(String txt) {
  clear_stream();
  pxs.setColor(0,0,0); pxs.fillRectangle(8, 205, 28, 35);
  pxs.setColor(115,115,115); pxs.drawLine(12,190,308,190);
  pxs.drawCompressedBitmap(11, 200, streams_icon);
  STR_ICON = 1;
  pxs.setColor(0,0,0); pxs.fillRectangle(47,205,273,35);  //CLEAR BANNER
  pxs.setColor(215,215,215); pxs.setFont(Avenir18a); pxs.print(47, 202, txt);
}



















