void init_clock() {
  setSyncProvider(getTeensy3Time);
  Teensy3Clock.compensate(rtcCompensate); //-240 seconds/day
}

time_t getTeensy3Time() {
  return Teensy3Clock.get();
}

unsigned long processSyncMessage() {
  unsigned long pctime = 0L;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013 

  if (MCU.find(TIME_HEADER)) {
     pctime = MCU.parseInt();
     return pctime;
     if (pctime < DEFAULT_TIME) { //Check the value is a valid time (i.e. greater than 1/1/13)
       pctime = 0L; //Return 0 to indicate that the time is not valid
     }
  }
  return pctime;
}

String hrConvert(int inp) {
  String result = F("12");

  if (inp == 0) 
  {
    result = F("12");
    return result;
  }
  else if (inp == 1) 
  {
    result = F("1");
    return result;
  }
  else if (inp == 2) 
  {
    result = F("2");
    return result;
  }
  else if (inp == 3) 
  {
    result = F("3");
    return result;
  }
  else if (inp == 4) 
  {
    result = F("4");
    return result;
  }
  else if (inp == 5) 
  {
    result = F("5");
    return result;
  }
  else if (inp == 6) 
  {
    result = F("6");
    return result;
  }
  else if (inp == 7) 
  {
    result = F("7");
    return result;
  }
  else if (inp == 8) 
  {
    result = F("8");
    return result;
  }
  else if (inp == 9) 
  {
    result = F("9");
    return result;
  }
  else if (inp == 10) 
  {
    result = F("10");
    return result;
  }
  else if (inp == 11) 
  {
    result = F("11");
    return result;
  }
  else if (inp == 12) 
  {
    result = F("12");
    return result;
  }
  else if (inp == 13) 
  {
    result = F("1");
    return result;
  }
  else if (inp == 14) 
  {
    result = F("2");
    return result;
  }
  else if (inp == 15) 
  {
    result = F("3");
    return result;
  }
  else if (inp == 16) 
  {
    result = F("4");
    return result;
  }
  else if (inp == 17) 
  {
    result = F("5");
    return result;
  }
  else if (inp == 18) 
  {
    result = F("6");
    return result;
  }
  else if (inp == 19) 
  {
    result = F("7");
    return result;
  }
  else if (inp == 20) 
  {
    result = F("8");
    return result;
  }
  else if (inp == 21) 
  {
    result = F("9");
    return result;
  }
  else if (inp == 22) 
  {
    result = F("10");
    return result;
  }
  else if (inp == 23) 
  {
    result = F("11");
    return result;
  }
  return result;
}

String monthConvert(int inp, int abbrv) {
  String result;

  if (inp == 1) 
  {
    result = F("january");
  }
  else if (inp == 2) 
  {
    result = F("february");
  }
  else if (inp == 3) 
  {
    result = F("march");
  }
  else if (inp == 4) 
  {
    result = F("april");
  }
  else if (inp == 5) 
  {
    result = F("may");
  }
  else if (inp == 6) 
  {
    result = F("june");
  }
  else if (inp == 7) 
  {
    result = F("july");
  }
  else if (inp == 8) 
  {
    result = F("august");
  }
  else if (inp == 9) 
  {
    result = F("september");
  }
  else if (inp == 10) 
  {
    result = F("october");
  }
  else if (inp == 11) 
  {
    result = F("november");
  }
  else
  {
    result = F("december");
  }
  
  if (abbrv == 1) {
    if (result != F("May")) {
      result = result.substring(0, 3);
    }
  }
  
  return result;
}

void clockSet(unsigned long timeCode) {
  time_t t = timeCode;
  if (t != 0) {
    Teensy3Clock.set(t); // Set the RTC
    setTime(t);
    Teensy3Clock.compensate(rtcCompensate);
  }
  update_clock();
}

void update_clock() {
  String ampm;
  hours = hour();
  minutes = minute();
  seconds = second();
  date = day();
  months = month();
  years = year();
  
  // UPDATE CLOCK (HH:MM xm)
  if ((minutes != minutesLast) || (hours != hoursLast) || (startup == true)) {    
    if (hours < 12) ampm = F("am");
    else ampm = F("pm");
    		
    // UPDATE TIME
    String addZ = F("");
    if (minutes < 10) addZ = F("0");
    
    // MEASURE WIDTH OF NEW TIME TEXT
    pxs.setFont(ClockFont); 
    pxs.setColor(255,255,255);
    int timeWidth = pxs.getTextWidth(hrConvert(hours) + F(":") + addZ + minutes); // TIME
    pxs.setFont(Avenir18a); 
    pxs.setColor(255,255,255);
    int ampmWidth = pxs.getTextWidth(ampm); 
    int totalWidth = timeWidth + ampmWidth; 
    int tBegin = ((320 - totalWidth)/2);
    
    // PRINT NEW TIME TEXT (e.g. XX:XX XM)
    pxs.setColor(0, 0, 0); 
    pxs.fillRectangle(0, 64, 320, 79); // CLEAR CLOCK
    pxs.setBackground(0, 0, 0); 
    pxs.setColor(255, 255, 255); 
    pxs.setFont(ClockFont); // TEXT SETUP
    pxs.print(tBegin, 65, hrConvert(hours) + F(":") + addZ + minutes); // PRINT TIME
    pxs.setFont(Avenir18a); 
    pxs.print(tBegin+timeWidth+(tBegin/6), 61, ampm); //PRINT XM
    
    // UPDATE STATE VARS
    minutesLast = minutes; 
    hoursLast = hours;
    startup = false;
  }
  
  if ((info_flag == true) && (startup != true)) return;
  
  if (MSA == 1) {
    if (msa_mode == 1) return;
    else msa_mode++;
  }
  else if (ANA == 1) {
    if (ana_mode == 1) return;
    else ana_mode++;
  }
    
  if ((months != monthsLast) || (date != dateLast) || (temperature != tempLast) || (forecast != foreLast)) {
    String dateString;
    int dateWidth;
    pxs.setBackground(0, 0, 0);
    pxs.setFont(Avenir18a);

    if ((forecast != F("")) && (location != F("")) && (temperature != F(""))) {
      dateString = location + F(" | ") + temperature + F(" | ") + forecast;
      dateString.toLowerCase();
      dateWidth = pxs.getTextWidth(dateString);
      if (dateWidth > 316) {
        weather_rotate++; 
        if (weather_rotate > 1) weather_rotate = 0;
        if (weather_rotate == 1) {
          dateString = monthConvert(months, 0) + F(" ") + date + F(" | ") + location;
          dateWidth = pxs.getTextWidth(dateString);
          if (dateWidth > 316) {
            dateString = monthConvert(months, 1) + F(" ") + date + F(" | ") + location;
            dateWidth = pxs.getTextWidth(dateString);
          }
        }
        else {
          dateString = temperature + F(" | ") + forecast;
          dateWidth = pxs.getTextWidth(dateString);
        }
      }
    }
    else {
      dateString = monthConvert(months, 0) + F(" ") + date + F(", ") + years;
      dateWidth = pxs.getTextWidth(dateString);
    }

    //if (dateStringLast != dateString) {
      int dBegin = ((320 - dateWidth) / 2);
      pxs.setColor(0, 0, 0);
      pxs.fillRectangle(0, 146, 320, 38);
      pxs.setColor(255, 255, 255);            // CLEAR OLD DATESTRING
      pxs.print(dBegin, 146, dateString);     // PRINT NEW DATESTRING
      //dateStringLast = dateString;
    //}
    
    //UPDATE STATE VARS
    tempLast = temperature; 
    foreLast = forecast; 
    monthsLast = months;
    dateLast = date;
    startup = false;
  }
}
















