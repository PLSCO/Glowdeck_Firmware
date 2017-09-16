void sketch_pane()
{
  STR_ICON = 0;
  int i; int c = analog_convert(DBR);
  for (i = c; i > -1; i--)
  {
    analogWrite(LCD_BL, i); delay(1);
  }
  minutesLast = -1;
  
  //WE ARE DIMMED, PRINT personal STREAM CONTENT, THEN RE-LIGHT LCD

  //pxs.setColor(0,0,0); pxs.fillRectangle(0, 55, 320, 185); //CLEAR DISPLAY BELOW HEADER ICONS
  pxs.setColor(0,0,0); pxs.fillRectangle(0, 0, 320, 240); //CLEAR DISPLAY BELOW HEADER ICONS
  
  //DRAW COMPLETE PANE UI
  //pxs.setColor(125,125,125); pxs.drawLine(12, 96, 308, 96); //DRAW UPPER DIV
  pxs.setColor(115,115,115); pxs.drawLine(12,191,308,191); //DRAW LOWER DIV
 
  pxs.drawCompressedBitmap(11, 200, streams_icon);
  sketch_banner();
  
  for (i = 255; i > -1; i--)
  {
    FastLED.setBrightness(i);
  }
  if (HUE == 999)
  {
    HUE = 0; 
    //BC127.print(F("SEND HUE:0\n\r"));
  }
  HUE = 0;
  for (int p = 0; p < NUM_LEDS; p++)
  {
    leds[p].setHue(HUE);
  }
  
  //FADE-IN LCD
  for (i = 0; i < c; i++)
  {
    analogWrite(LCD_BL, i); delay(1);
  }
 
  for (i = 0; i < 255; i++)
  {
    FastLED.setBrightness(i);
  }
  VIB = 255;
    
  PANE = 6; ROTATE.reset();
  
  pxs.setColor(CLR[0], CLR[1], CLR[2]);
  int startTime = millis();
  
  //TELL DEVICE TO START SENDING ARRAY
  BC127.print(F("SEND XY^\n\r"));
  
  sketch_flag = 1;
  /*
  while (sketch_flag == 1)
  {
    bc127_draw();
    if (sketch_flag == 2)
    {
      if ((millis() - startTime) > 8500)
      {
        sketch_flag = 0;
        break;
      }
    }
    else if (sketch_flag == 0)
    {
      break; 
    }
    else
    {
      if ((millis() - startTime) > 30000)
      {
        sketch_flag = 0;
        break;
      }
    }
  }
  */
  
  //sketch_flag = 0;
  //reattach_interrupts(); 
  return;
}

void draw_sketch(String cmd)
{
  
  if (cmd.indexOf(F("XY:")) != -1) //DRAW LINE COMMAND
  {
    
    uint8_t a, b, c, d, e; int x1, y1, x2, y2;
    
    a = cmd.indexOf(F(":")); b = cmd.indexOf(F(":"), a+1); c = cmd.indexOf(F(":"), b+1); d = cmd.indexOf(F(":"), c+1); e = cmd.indexOf(F("^"));
    
    x1 = (cmd.substring(a+1, b)).toInt(); y1 = (cmd.substring(b+1, c)).toInt(); x2 = (cmd.substring(c+1, d)).toInt(); y2 = (cmd.substring(d+1, e)).toInt();
    
    pxs.drawLine(x1, y1, x2, y2);
    
  }
  
  else if ((cmd.indexOf(F("RGB:")) != -1) || (cmd.indexOf(F("HSV:")) != -1)) //NEW PAINTBRUSH COLOR
  {
    
    uint8_t ind_a = cmd.indexOf(F(":")); uint8_t ind_b = cmd.indexOf(F(":"), ind_a+1); uint8_t ind_c = cmd.indexOf(F(":"), ind_b+1); uint8_t ind_d = cmd.indexOf(F("^"));
    
    CLR[0] = (cmd.substring(ind_a+1, ind_b)).toInt(); CLR[1] = (cmd.substring(ind_b+1, ind_c)).toInt(); CLR[2] = (cmd.substring(ind_c+1, ind_d)).toInt();
    
    pxs.setColor(CLR[0], CLR[1], CLR[2]);
    
    FastLED.showColor(CRGB(CLR[0], CLR[1], CLR[2]));
    
    return;
    
  }
  
  else if (cmd.indexOf(F("XYEND^")) != -1) //LCD DRAW COMMAND
  {
    sketch_flag = 0;
    FastLED.clear(); 
    int millisStart = millis();
  
    do 
    {
      confetti(); FastLED.delay(30);
    } 
    while ((millis() - millisStart) < 3375);
    fadeout_leds();
    return;
    
  }
  
  else
  {
    
    sketch_flag = 2;
    
    return;
    
  } 
  
}

void sketch_banner()
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
  
  //personal_banner(hrConvert(hours) + F(":") + addZ + minutes + " " + ampm);
  personal_banner(F("Press to Dismiss Card")); //+ hrConvert(hours) + F(":") + addZ + minutes + " " + ampm
}



















