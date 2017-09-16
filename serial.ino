void stream() {
  
  // SERIAL: UART
  if (DEBUG == 1) {
    if (MCU.available()) {
      detach_interrupts();
      int st = millis();
      while (MCU.available()) {
        char inChar = MCU.read();
        buffer += inChar;
        if (buffer.indexOf(F("\r")) != -1) {
          mcu_handler(buffer);
          buffer = F("");
          break;
        }
        if ((millis() - st) > 5000) {
          buffer = F("");
          break;
        }
      }
      reattach_interrupts();
    }
  }
  /*
  // SERIAL1: WIFI
  if (USR.available()) {
    detach_interrupts();
    usr_handler(usr_response(0));
    reattach_interrupts();
  }
  */
  
  // SERIAL2: BLUETOOTH
  if (BC127.getCommandCount()) {
    detach_interrupts(); 
    memset(buffer2, 0, sizeof(buffer2));
    while (BC127.getCommandCount()) {
      BC127.pullCommand(buffer2);
      process = (char*)buffer2;
      memset(buffer2, 0, sizeof(buffer2));
      if (process.indexOf(F("RECV ")) != -1) {
        if (process.indexOf(F("^")) == -1) {
          if (process.length() >= (BT_PREFIX+19)) {
            
            String appended;
            
            if (nextGen) {
              String tempProcess = process.substring(BT_PREFIX, process.length());
              String tempAppended;
              if (SPP == 1) {
                appended = F("RECV SPP 14 ") + tempProcess.substring(tempProcess.indexOf(" ") + 1, tempProcess.length());
              }
              else {
                appended = F("RECV BLE ") + tempProcess.substring(tempProcess.indexOf(" ") + 1, tempProcess.length());
              }              
            }
            else {
              
              appended = process;
              
            }
            
            process = F("");
            
            
            appended.trim();
            int st = millis();
            while (millis() - st < 15000) {
              if (BC127.getCommandCount()) {
                BC127.pullCommand(buffer2);
                process = (char*)buffer2;
                memset(buffer2, 0, sizeof(buffer2));
                
                if (process.indexOf(F("RECV ")) != -1) {
                  
                  int len = process.length();

                  if (nextGen) {

                    String temp = process.substring(BT_PREFIX, len);

                    appended = appended + temp.substring(temp.indexOf(" ") + 1, temp.length());
                    
                  }
                  else {
                  
                    appended = appended + process.substring(BT_PREFIX, len);
                  
                  } 
                  
                  process = F(""); 
                  appended.trim();
                  if (appended.indexOf(F("^")) != -1) {
                    process = appended;
                    appended = F(""); 
                    break;
                  }
                }    
              }
            }
          }
          else {
            //FOR STANDARD BC127 COMMANDS SENT FROM STREAMS DEV CONSOLE
            process = process.substring(BT_PREFIX, process.length()); 
            process.trim();
            btc_handler(process); 
            process = F("");
            // if ((BC127.checkListOverflow())) BC127.resetCommandList();
            if ((BC127.checkListOverflow()) || (BC127.checkCharOverflow())) BC127.resetCommandList();
            reattach_interrupts();
            return;
          }
        }
      }
      bc127_handler(process); 
      process = F("");
      if ((BC127.checkListOverflow()) || (BC127.checkCharOverflow())) BC127.resetCommandList();
      reattach_interrupts();
      return;
    }
    reattach_interrupts();
  }
  /*
  else if (MCU.available()) { // (DEBUG == 1) {
    detach_interrupts();
    char inChar = MCU.read();
    if (inChar == '\r') {
      buffer.trim();
      if (buffer.startsWith(F("SEND"))) {
         buffer += F("\n\r");
      }
      else {
        buffer += F("\r");
      }      
      BC127.print(buffer);
      buffer = F("");
      BC127.flush();
      if ((BC127.checkListOverflow()) || (BC127.checkCharOverflow())) BC127.resetCommandList();
      reattach_interrupts();
      return;
    }
    buffer += inChar;
    reattach_interrupts();
  }
  */
  else if (BTQ != F("")) {
    if (BTQ.indexOf(F("OPEN ")) != -1) {
      if ((BTQ.indexOf(F(" MAP")) != -1) && (MAP == 1)) {
        BTQ = F("");
        return;
      }
    }   
    detach_interrupts(); 
    btq_handler();
    if ((BC127.checkListOverflow()) || (BC127.checkCharOverflow())) BC127.resetCommandList();
    reattach_interrupts();
  }
  else if ((BC127.checkListOverflow()) || (BC127.checkCharOverflow())) {
    BC127.resetCommandList();
  }

}

void mcu_handler(String cmd) {  
  // SEND CUSTOM BLE/SPP COMMAND
  if (cmd.indexOf(F("^")) != -1) {
      cmd.replace(F("\n\r"), F(""));
      cmd.replace(F("\r"), F(""));
      cmd.replace(F("\n"), F(""));
      cmd.replace(F("SEND "), F(""));
      cmd = F("SEND ") + cmd + F("\n\r");
  }
  else {
    if (cmd.indexOf("\r") == -1) {
      cmd = cmd + "\r";
    } 
  }
  
  if (cmd.startsWith(F("```"))) {
    dfu_mode(); 
    return;
  }
  
  else if (cmd.startsWith(F("^!^"))) {
    force_factory();
    return;
  }
  
  else if (cmd.startsWith(F("^*^"))) {
    EEPROM.write(50, '2');
    // BC127.print(F("SET NAME=BlueCreation-0"));
    // BC127.print(SID); 
    // BC127.print(F("\r"));
    // BC127.print(F("WRITE\r"));
    dfu_mode();
    return;
  }
  
  BC127.print(cmd); 
  BC127.flush();
  return;
  /*
  else if (cmd.startsWith(F("GET_SOC")))
  {
    if ((SPP == 1) || (BLE == 1))
    {
      BC127.print(F("SEND SOC^\n\r"));
    }
    return;
  }
  
  else if (cmd.startsWith(F("GET_PER")))
  {
    if ((SPP == 1) || (BLE == 1))
    {
      BC127.print(F("SEND PER^\n\r"));
    }
    return;
  }
  
  else if (cmd.startsWith(F("GET_WTR")))
  {
    if ((SPP == 1) || (BLE == 1))
    {
      BC127.print(F("SEND WTR^\n\r"));
    }
    return;
  }
  */
  
  /*
  else if (cmd.startsWith(F("GET_EEPROM")))
  {
    MCU.println(F("Printing EEPROM Values:"));
    for (int e = 1; e < 13; e++)
    {
      char epChar = char(EEPROM.read(e));
      MCU.print(epChar);
      if ((e == 1) || (e == 6) || (e == 11) || (e == 12)) 
      {
        MCU.println();
      }
      delay(75);
    }
    MCU.println();
    return;
  }
  */
  /*
  else if (cmd.startsWith(F("TI:")))   //SET GLOWDECK TIME
  {
    //Ex: RECV BLE 2015-01-31 03:04:55
    //cmd = F("RECV BLE ")
    int len = cmd.length(); int yr_begin = cmd.indexOf(F(":"));
    String andr_time = cmd.substring(yr_begin, len);
    len = andr_time.length();
    int t_yr, t_mon, t_day, t_hr, t_min, t_sec; int h1, h2, s1, c1, c2;
    int p1 = andr_time.indexOf(F(":")); h1 = andr_time.indexOf(F("-")); h2 = andr_time.indexOf(F("-"), h1+1); s1 = andr_time.indexOf(F(" ")); c1 = andr_time.indexOf(F(":"), s1+1); c2 = andr_time.indexOf(F(":"), c1+1); 
    t_yr = (andr_time.substring(p1+1, h1)).toInt(); t_mon = (andr_time.substring(h1+1, h2)).toInt(); t_day = (andr_time.substring(h2+1, s1)).toInt(); 
    t_hr = (andr_time.substring(s1+1, c1)).toInt(); t_min = (andr_time.substring(c1+1, c2)).toInt(); t_sec = (andr_time.substring(c2+1, len)).toInt();
    setTime(t_hr,t_min,t_sec,t_day,t_mon,t_yr); Teensy3Clock.set(now()); monthsLast = -1; update_clock(); 
    return;
  }
  */
  /*
  
  else if (cmd.startsWith(F("GET_WEATHER")))
  {
    get_stream(F("w"));
    return;
  }

  else if (cmd.startsWith(F("GET_AMBIENT")))
  {
    int tmpamb = analogRead(AMB_LEVEL);
    MCU.print(F("AMB: ")); MCU.print(AMB); MCU.print(F(" | NOW: ")); MCU.println(tmpamb);
    return;
  }
  */

  /*
  else if (cmd.startsWith(F("GET_ANALOG")))
  {
    int v_left = 0; 
    int v_right = 0;
    //v_left = analogRead(AUDIO_L); v_right = analogRead(AUDIO_R);
    v_left = map(analogRead(A3),400, 800, 0, 1024); 
    v_right = map(analogRead(A0),400, 800, 0, 1024);
    MCU.print(F("L: ")); 
    MCU.print(v_left); 
    MCU.print(F(" | R:")); 
    MCU.println(v_right);
    return;
  }

  else if (cmd.startsWith(F("ON_MSYNC")))
  {
    int elapse; 
    int current = millis();
    do
    {
      //msync();
      int v_left = 0; 
      int v_right = 0; //v_left = analogRead(AUDIO_L); v_right = analogRead(AUDIO_R);
      v_left = map(analogRead(A3),400, 800, 0, 1024); 
      v_right = map(analogRead(A0),400, 800, 0, 1024);
      MCU.print(F("L: ")); 
      MCU.print(v_left); 
      MCU.print(F(" | R:")); 
      MCU.println(v_right);
      delay(1000);
      elapse = millis();
    }
    while (elapse - current < 15000);
    return;
  }
  
  else if (cmd.startsWith(F("XMIT_INIT")))
  {
    send_init();
    return;
  }

  else if (cmd.startsWith(F("ON_AMP")))
  {
    digitalWrite(AMP, HIGH); 
    SND = 1;
    volume = 50; 
    vol_icon(volume);
    return;
  }

  else if (cmd.startsWith(F("OFF_AMP")))
  {
    digitalWrite(AMP, LOW); 
    SND = 0;
    volume = 0; 
    vol_icon(volume);
    return;
  }
  */
  
  /* MAJOR REMOVAL********************
  else if (cmd.startsWith(F("AT+")))
  {
    cmd.trim(); 
    cmd += F("\r"); 
    USR.print(cmd); 
    usr_handler(usr_response(0)); 
    return;
  }
  */
  /*
  else
  {
    if (cmd.startsWith(F("SEND ")))
    {
      if ((BLE == 1) || (SPP == 1))
      {
        cmd.trim(); cmd += F("\n\r"); 
        BC127.print(cmd); return;
      }
    }
    else
    {
      cmd.trim(); cmd += F("\r");
      BC127.print(cmd); return;
    }
  }
  */
}

void btc_handler(String cmd) {
  if (cmd.startsWith(F("```"))) {
    dfu_mode(); 
    return;
  }
  else if (cmd.startsWith(F("^!^"))) {
    force_factory();
    return;
  }
  else if (cmd.startsWith("^")) {
    cmd = cmd.substring(6, cmd.length());
    cmd.trim();
    BC127.print(cmd); BC127.print(F("\r")); BC127.flush();
    return;
  }
  /* MAJOR REMOVAL****************************
  else if (cmd.startsWith(F("AT+")))
  {
    //cmd.trim(); BC127.print(F("SEND ") + cmd + F("\n\r")); BC127.flush();
    cmd.trim(); cmd += '\r'; USR.print(cmd); USR.flush();
    String usr_rsp = usr_response(0); usr_rsp.trim();
    //BC127.print(F("SEND ") + usr_rsp + F("\n\r")); BC127.flush();
    usr_handler(usr_rsp); return;
  }
  */
  else {
    //cmd.trim(); BC127.print(F("SEND ") + cmd + F("\n\r")); BC127.flush();
    cmd.trim(); BC127.print(cmd); BC127.print(F("\r"));
    return;
  }
}

void btq_handler() {  
  if (BTQ.startsWith(F("SEND "))) {
    if (BTQ.indexOf(F("\n\r")) == -1) {
      BTQ.replace(F("\r"), F(""));
      BTQ += F("\n\r"); 
    }
  }
  else {
    if (BTQ.indexOf(F("\r")) == -1) {
      BTQ += F("\r");
    } 
  }
  
  if ((BTQ.startsWith(F("OPEN "))) && (BTQ.indexOf(F(" MAP")) != -1) && (MAP == 1)) {
    BTQ = F("");
    return;
  }
  
  if (remap_flag == 0) {
    if (BTQ.indexOf(F(" MAP")) != -1) {
      map_flag = 1;
      remap_flag++;
      attempts_flag++;
    }
  }
  
  BC127.print(BTQ); 
  BC127.flush(); 
  BTQ = F("");
  return;
}

String bc127_response() {
  // READ DATA FROM BC127
  String result = F(""); 
  BC127.flush(); 
  delay(5);

  if (!BC127.getCommandCount()) {
    delay(50);
  }
  
  if (!BC127.getCommandCount()) {
    delay(165);
  }
  
  if (BC127.getCommandCount()) {
    memset(buffer2, 0, sizeof(buffer2));
    BC127.pullCommand(buffer2);
    result = (char*)buffer2;
    memset(buffer2, 0, sizeof(buffer2));
   
    /*
    delay(1);
    if (BC127.getCommandCount()) {
      memset(buffer2, 0, sizeof(buffer2));
      BC127.pullCommand(buffer2);
      result = result + (char*)buffer2;
      memset(buffer2, 0, sizeof(buffer2));      
      delay(1);
      
      if (BC127.getCommandCount()) {
        memset(buffer2, 0, sizeof(buffer2));
        BC127.pullCommand(buffer2);
        result = result + (char*)buffer2;
        memset(buffer2, 0, sizeof(buffer2));      
      }
    }
    */

    if ((BC127.checkListOverflow()) || (BC127.checkCharOverflow())) BC127.resetCommandList();
    
    return result;
  }
  
  if ((BC127.checkListOverflow()) || (BC127.checkCharOverflow())) BC127.resetCommandList();
  
  return result;
} 

String usr_response(int contentLength) { 
  // 0 = cmd, 1 = header, 2=body
  
  /*MAJOR REMOVAL**********************
  String buffer1 = F(""); char inByte; int rCount = 0; int nCount = 0; int curLength = 0; bool end = false;
  USR.flush(); int st = millis();

  while (!end)
  {
    
    if (USR.available())
    {
      inByte = USR.read();
      curLength++;

      if (contentLength == 0)
      {
        if ((inByte == '\n') && (rCount == 2) && (nCount == 1))
        {
          end = true;
          int strLength = buffer1.length()-3;
          buffer1 = buffer1.substring(0, strLength);
        }
        else if (inByte == '\r')
        {
          rCount++;
        }
        else if (inByte == '\n')
        {    
          nCount++;
        }
        else
        {
          rCount = 0;
          nCount = 0;
        }
      }
      else if (curLength >= contentLength)
      {
        end = true;
      }
      buffer1 += inByte;
    }
    else
    {
      if ((millis() - st) > (5500)) 
      {
        break;
      }
    }
  }
  if (contentLength != 0)
  {
    if (USR.available())
    {
      int st = millis(); 
      bool end = false;
      while (!end)
      {
        while (USR.available())
        {
          USR.read();
        }
        delay(50);
        if ((!USR.available()) || ((millis() - st) > 1000))
        {
          end = true;
        }
      }
    }
  }
  return buffer1;
  */
  return F("");
}

















