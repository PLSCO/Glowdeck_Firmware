void get_stream(String strid) { 
  String UID = F("00000");
  
  //STREAMS AUTHENTICATION
  //p:      Provision device with Streams account associated with UID
  //v:      Verifies device is provisioned in the Streams database
  
  //STREAMS ITEM COUNTS
  //ce:     Retrieves # of Items in Email Stream
  //cn:     Retrieves # of Items in News Stream
  
  //STREAMS ITEM RETRIEVAL
  //w:      Retrieves the Weather Stream
  //n[X]:   Retrieves the News Stream [X = Item #]
  
  //String get_pre = F("AT+HTTPPH="); 
  String get_req;
  
  //SET HTTP COMMAND CONSTANTS HERE
  while (USR.available()) { USR.read(); }
  //USR.print(F("AT+TCPLK\r")); String tmp_rsp = usr_response(0);
  //MCU.print(F("Server Link: ")); MCU.println(tmp_rsp);
  //tmp_rsp = F("");
  
  if (strid == F("v"))  //VERIFY DEVICE IS PROVISIONED FOR UID
  {
    get_req = "/api/devicestream/vd/?U=" + UID + "&D=" + SID;
    MCU.print(F("GET: ")); MCU.println(get_req);
    //e.g. success:  {"status":"ok","verified":1,"token":"c357f2f1fae61ffa1afb2316932458b3"}
    //e.g. not-reg:  {"status":"ok","verified":0}
  }
  else if (strid == F("a"))  //PROVISION DEVICE VIA WIFI WEB CLIENT
  {
    //e.g. path:     /api/devicestream/ad/?U=jmkauf@gmail.com&P=CorrectPW&D=15F16
    //e.g. success:  {"status":"ok","verified":"1","uid":957,"tkn":"c357f2f1fae61ffa1afb2316932458b3"}
    
    //e.g. path:     /api/devicestream/ad/?U=jmkauf@gmail.com&P=WrongPW&D=15F31
    //e.g. fail:     {"status":"FAIL","error":"Invalid email and\/or password."}
    
    /*
    get_req = "/api/devicestream/ad/?U=" + UNM + "&P=" + PSW + "&D=" + SID;
    */
    return;
    MCU.print(F("GET: ")); MCU.println(get_req);
  }
  else if (strid == F("p"))  //AUTO-PROVISION DEVICE FOR UID
  {
    //e.g. path:      /api/devicestream/pd/?U=957&D=15F16
    //e.g. success:   {"status":"ok","0":["438947432432",0,"Test Glowdeck2","walnut",1424218026,0,"IP Address"],"":[null,0,"Glowdeck","walnut",1427933231,0,"IP Address"],"(null)":["(null)",0,"(null)","walnut",1426831344,0,"IP Address"],"15F16":["15F16",0,"Glowdeck15F16","walnut",1427933289,0,"IP Address"]} 
    get_req = "/api/devicestream/pd/?U=" + UID + "&D=" + SID;
    MCU.print(F("GET: ")); MCU.println(get_req);
  }
  else if (strid == F("w"))  //GET WEATHER STREAM
  { 
    get_req = "/api/devicestream/ss/?U=" + UID + "&D=" + SID + "&S=w"; //+ strid;
    MCU.print(F("GET: ")); MCU.println(get_req);
  }
  else if (strid == F("nc")) //GET NEWS COUNT
  {
    get_req = "/api/devicestream/sc/?U=" + UID + "&D=" + SID + "&S=n";
    MCU.print(F("GET: ")); MCU.println(get_req);
  }
  else if (strid == F("ec")) //GET EMAIL COUNT
  {
    get_req = "/api/devicestream/sc/?U=" + UID + "&D=" + SID + "&S=e";
    MCU.print(F("GET: ")); MCU.println(get_req);
  }
  else if (strid == F("n")) //GET NEWS ITEM
  {
    /*
    if (social_count >= 1)
    {
      social_counter--;
      if (social_counter < 1) social_counter = (social_count - 1);
      if (social_counter >= social_count) social_counter = (social_count - 1);
      if (social_counter == 0) social_counter = 1;
    }
    else
    {
      return;
    }
    */
    //get_req = F("/api/devicestream/si/?U=") + UID + String(F("&D=")) + SID + String(F("&S=n&N=")) + social_counter;
    get_req = "/api/devicestream/ss/?U=" + UID + "&D=" + SID + "&S=n";
    MCU.print(F("GET: ")); MCU.println(get_req);
  }
  else if (strid == F("e")) //GET EMAIL ITEM
  {
    if (email_count >= 1)
    {
      email_counter--;
      if (email_counter < 1) email_counter = (email_count - 1);
      if (email_counter == 0) email_counter = 1;
    }
    else
    {
      return;
    }
    get_req = F("/api/devicestream/si/?U=") + UID + String(F("&D=")) + SID + String(F("&S=e&N=")) + email_counter;
    //MCU.print(F("GET: ")); MCU.println(get_req);
  }
  
  String buffer1 = F(""); wifi_icon(4);
  while (USR.available()) { USR.read(); }
  
  USR.print("AT+HTTPPH="); USR.print(get_req); USR.print('\r'); //USR.flush(); 
  usr_response(0); get_req = F(""); 
  //while (USR.available()) { USR.read(); }
  USR.print("AT+HTTPDT"); USR.print('\r'); //USR.flush();
  
  //AWAIT REPLY FROM SERVER 
  bool success = true;
  unsigned long st_time = millis();
  if (!USR.available())
  {
    while (!USR.available())
    {
      if (USR.available())
      {
        success = true; break;
      }
      if (millis() - st_time > 10000) 
      {
        success = false; break;
      }
    }
  }
  if (success == false)
  {
    int st = millis(); bool end = false;
    while (!end)
    {
      while (USR.available())
      {
        USR.read();
      }
      delay(50);
      if ((!USR.available()) || ((millis() - st) > 1250))
      {
        end = true;
      }
    }
    MCU.println(F("NO RESPONSE FROM SERVER!"));
    USR.print(F("AT+Z\r")); delay(500); //init_usr();
    buffer1 = F(""); wifi_icon(3); return;
  }
  
  String header = usr_response(0);
  String contentLength = header.substring(header.lastIndexOf(F("Content-Length: ")));
  contentLength = contentLength.substring(16, contentLength.indexOf(F("\r")));
  int dataLength = contentLength.toInt();
  //header = F("");
  
  if (dataLength != 0)
  {
    buffer1 = usr_response(dataLength);
    int content_end = buffer1.indexOf(F("HTTP/1.1"));
    if (content_end != -1)
    {
      buffer1 = buffer1.substring(0, content_end);
    }
  }
  else
  {
    MCU.println(F("NO CONTENT FROM SERVER!"));
    /*MCU.println(header); MCU.println(buffer1); buffer1 = F(""); header = F(""); */
    USR.print(F("AT+Z\r")); delay(500); //init_usr();
    buffer1 = F(""); wifi_icon(3); return;
  }
  
  //***GET RESPONSE HANDLERS***
  if (strid == F("v"))
  {
    if (buffer1.indexOf(F(":1")) != -1)
    {
      buffer1.trim();
      int tlen = (buffer1.length()-2); int tbeg = (buffer1.lastIndexOf(F(":"))+2);
      // TKN = buffer1.substring(tbeg, tlen);
      // MCU.println(F("Device Provisioned UID ") + UID); MCU.println();
      REG = 1; str_errs = 0;
    }
    else
    {
      str_errs++; REG = -1;  //SET AUTO-PROVISIONING FLAG
      // String UID = F("00000");
      MCU.print(F("Queue Provisioning for UID: ")); MCU.println(UID); MCU.println();
    }
  }
  
  else if (strid == F("a"))
  {
    //e.g. success:  {"status":"ok","verified":"1","uid":957,"tkn":"c357f2f1fae61ffa1afb2316932458b3"}
    //e.g. fail:     {"status":"FAIL","error":"Invalid email and\/or password."}
    
    //FAIL
    if (buffer1.indexOf(F("Invalid email")) != -1)
    {
      REG = -1;
      MCU.println(F("Could Not Authenticate Email and Password!"));
      //MCU.print(F("Email: ")); MCU.print(UNM); MCU.print(F(" | Pass: ")); MCU.println(PSW); MCU.println();
    }
    //SUCCESS
    else if ( (buffer1.indexOf(F("uid")) != -1) && (buffer1.indexOf(F("tkn")) != -1) )
    {
      int a1 = buffer1.indexOf(F(":")); int a2 = buffer1.indexOf(F(":"), a1+1); 
      int a3 = buffer1.indexOf(F(":"), a2+1); int a4 = buffer1.indexOf(F(":"), a3+1);
      if ((a3 != -1) && (a4 != -1))
      {
        int c1 = buffer1.indexOf(F(","), a3+1); int c2 = buffer1.indexOf(F("}"));
        String UID = buffer1.substring(a3+1, c1); // TKN = buffer1.substring(a4+2, c2-1);
        String UID_S = F(""); int UID_LEN = UID.length();
        if (UID_LEN == 1)
        {
          UID_S = F("0000");
        }
        else if (UID_LEN == 2)
        {
          UID_S = F("000");
        }
        else if (UID_LEN == 3)
        {
          UID_S = F("00");
        }
        else if (UID_LEN == 4)
        {
          UID_S = F("0");
        }
        else
        {
          UID_S = F("");
        }
        UID_S += UID; char UID_CHR[10];
        UID_S.toCharArray(UID_CHR, 6);
        eeprom_write_string(7, UID_CHR);
        MCU.print(F("UID RECEIVED: ")); MCU.println(UID);
        // MCU.print(F("TKN RECEIVED: ")); MCU.println(TKN);
        print_info(F("Streams Account Verified!"));
        if (PRV == 0)
        {
          PRV = 1; EEPROM.write(1, '1');
        }
      }
    }
  }
  
  else if (strid == F("p"))
  {
    String UID = F("00000");
    if (buffer1.indexOf(SID) != -1)
    {
      MCU.print(F("Provision Success for UID ")); MCU.print(UID); MCU.print(F(" | SID ")); MCU.println(SID); MCU.println();
      REG = 1;
      if (PRV == 0)
      {
        EEPROM.write(1, '1'); PRV = 1;
      }
    }
    else
    {
      MCU.println(F("Provision Unsuccessful! Output:")); MCU.println(buffer1); MCU.println();
      str_errs++; REG = -1;
    }
  }
  
  else if (strid == F("w"))
  {
    if ((buffer1.indexOf(F("status")) != -1) && (buffer1.indexOf(F("ok")) != -1))
    {
      int i1; int i2; int a1; int a2; int a3;
      i1 = buffer1.indexOf(F("[")); i2 = buffer1.indexOf(F("]"));
      buffer1 = buffer1.substring(i1+1, i2);
      int a_len = buffer1.length();
      a1 = buffer1.indexOf(F(",")); a2 = buffer1.indexOf(F(","), a1+1); a3 = buffer1.indexOf(F(","), a2+1);
      location = buffer1.substring(1, a1-1);
      temperature = buffer1.substring(a1+2, a2-1) + F("Ãƒâ€šÃ‚Â¬ÃƒÂ¢Ã‹â€ Ã‚Â«") + buffer1.substring(a2+2, a3-1);
      forecast = buffer1.substring(a3+2, a_len-1);
      
      if (location.indexOf(F("FAIL")) != -1)
      {
        MCU.println(F("Queue Auto-Provisioning"));
        temperature = tempLast; forecast = foreLast;
        REG = -1;
      }
      else
      {
        MCU.print(F("City: ")); MCU.println(location);
        MCU.print(F("Temp: ")); MCU.println(temperature);
        MCU.print(F("Fore: ")); MCU.println(forecast);
        MCU.println();
        str_errs = 0;
      }
    }
  }
  
  else if (strid == F("nc"))
  {
    //Ex: {"status":"ok","0":15}
    if ((buffer1.indexOf(F("status")) != -1) && (buffer1.indexOf(F("ok")) != -1))
    {
      int tmp_count = (buffer1.substring((buffer1.lastIndexOf(F(":"))+1), buffer1.indexOf(F("}")))).toInt();
      if (tmp_count > 0)
      {
        if (tmp_count > social_count)
        {
          //NEW NEWS ITEM
          social_counter = tmp_count;
        }
        social_count = tmp_count;
        MCU.print(F("News Items: ")); MCU.println(social_count);
        str_errs = 0; social_errors = 0;
      }
      else
      {
        MCU.println(F("Error Retrieving News Count!"));
        social_errors++;
      }
    }
  }
      
  else if (strid == F("ec"))
  {
    //Ex: {"status":"ok","0":15}
    if ((buffer1.indexOf(F("status")) != -1) && (buffer1.indexOf(F("ok")) != -1))
    {
      int tmp_count = (buffer1.substring((buffer1.lastIndexOf(F(":"))+1), buffer1.indexOf(F("}")))).toInt();
      if (tmp_count > 0)
      {
        if (tmp_count > email_count)
        {
          //NEW NEWS ITEM
          email_counter = tmp_count;
        }
        email_count = tmp_count;
        MCU.print(F("Email Items: ")); MCU.println(email_count);
        str_errs = 0;
      }
      else
      {
        MCU.println(F("Error Retrieving Email Count!"));
      }
    }
  }
  
  else if (strid == F("nx"))
  {
    //Ex: {"status":"ok","0":["NDTV","Buying a Wi-Fi Router? Here Are the Features and Specifications That Matter",0,"Wifi",1427703948,null]}
    //"NDTV","Buying a Wi-Fi Router? Here Are the Features and Specifications That Matter",0,"Wifi",1427703948,null
    //"NDTV","Buying a Wi-Fi Router? Here Are the Features and Specifications That Matter",0,"Wifi"
    //"NDTV","Buying a Wi-Fi Router? Here Are the Features and Specifications That Matter",0
    //"NDTV","Buying a Wi-Fi Router? Here Are the Features and Specifications That Matter"
    //Buying a Wi-Fi Router? Here Are the Features and Specifications That Matter
    if ((buffer1.indexOf(F("status")) != -1) && (buffer1.indexOf(F("ok")) != -1))
    {
      int i1; int i2;
      i1 = buffer1.indexOf(F("[")); i2 = buffer1.indexOf(F("]"));
      buffer1 = buffer1.substring(i1+1, i2); buffer1 = buffer1.substring(0, buffer1.lastIndexOf(F(","))); buffer1 = buffer1.substring(0, buffer1.lastIndexOf(F(",")));
      String type_tmp = buffer1.substring((buffer1.lastIndexOf(F(","))+2), (buffer1.length()-1)); buffer1 = buffer1.substring(0, buffer1.lastIndexOf(F(",")));  buffer1 = buffer1.substring(0, buffer1.lastIndexOf(F(",")));
      if (type_tmp.indexOf(F("FAIL")) != -1)
      {
        str_errs++;
        MCU.println(F("Could not retrieve content for this UID/SID!")); MCU.println();
      }
      else
      {
        //STR_A = type_tmp; type_tmp = F("");
        //STR_B = buffer1.substring(1, (buffer1.indexOf(F(","))-1));
        //STR_C = buffer1.substring((buffer1.indexOf(F(","))+2), (buffer1.length()-1));
        //STR_D = F("1hr");
        
        /*
        int head_div = STR_B.indexOf(F(" "));
        if (head_div != -1)
        {
          STR_B = STR_B.substring(0, head_div);
        }
        STR_B.toUpperCase();
        */
        
        //MCU.print(F("Topic: ")); MCU.println(STR_A);
        //MCU.print(F("Source: ")); MCU.println(STR_B);
        //MCU.print(F("Headline: ")); MCU.println(STR_C);
        //MCU.print(F("Elapsed: ")); MCU.println(STR_D);
        MCU.println();
        //print_stream(STR_B, STR_C);
        str_errs = 0; social_errors = 0;
      }
    }
    else
    {
      social_errors++;
    }
  }
  
  else if (strid == F("n"))
  {
    //Ex: 
    //{"status":"ok",
    //"0":["The Verge","William Shatner apparently thinks a Kickstarter-funded water pipeline will ...",0,"Kickstarter",1429378522,null],
    //"1":["Forbes","Amanda Palmer Uncut: The Kickstarter Queen On Spotify, Patreon And Taylor Swift",0,"Kickstarter",1429351568,null],
    //"2":["USA TODAY","David Beckham totally embarrasses son Brooklyn on Instagram",0,"Brooklyn",1429405908,null],
    //"3":["New York Times","One Teenager Is Killed and Another Is Injured in Shooting Outside Brooklyn ...",0,"Brooklyn",1429333221,null],
    //"4":["Gizmag","If at first you don't succeed: Glowdeck makes a second bid for release",0,"Glowdeck",1396977127,null],
    //"5":["Android Police","Glowdeck Combines Wireless Charging, Bluetooth Audio, Notifications, And ...",0,"Glowdeck",1397646254,null],
    //"6":["New York Times","Technology That Prods You to Take Action, Not Just Collect Data",0,"Technology",1429396154,null],
    //"7":["Gazette","Golf notebook: Jason Day jumping feet first into shoe technology - Pittsburgh Post",0,"Technology",1429416254,null],
    //"8":["9 to 5 Mac (blog)","9to5Toys Last Call: Ooma VoIP Phone w\/ Bluetooth $90, iPhone 6\/Plus cases ...",0,"Bluetooth",1429308074,null],
    //"9":["CNET","Get Ausdom M06 Bluetooth headphones for $39.99",0,"Bluetooth",1429189333,null],
    //"10":["CNN","This flat antenna could finally give us good plane Wi-Fi",0,"Wifi",1429261726,null],
    //"11":["Military Times","Wi-Fi rolls out at stateside commissaries",0,"Wifi",1429351654,null],
    //"12":["Motley Fool","How Disney World Does the Internet of Things",0,"Internet of Things",1429392156,null],
    //"13":["The Register","The Internet of things is great until it blows up your house",0,"Internet of Things",1429280361,null]}
    int buf_len = buffer1.length();
    if ((buffer1.indexOf(F(":[")) != -1) && (buffer1.indexOf(F("]}")) != -1))
    {
      int p; buffer1 = buffer1.substring(15, buf_len);
      for (p = 0; p < 15; p++)
      {
        buf_len = buffer1.length(); int a1 = buffer1.indexOf(F("["));
        if (a1 == -1) 
        {
          social_count = p+1; break;
        }
        else //PARSE ONE NEWS ITEM AT A TIME
        {
          /*
          social[p][0] = F("w");  //ICON
          int comma = buffer1.indexOf(F(","), a1+1);
          social[p][2] = buffer1.substring(a1+2, comma-1); //SOURCE
          int comma2 = buffer1.indexOf(F(","), comma+1);
          social[p][3] = buffer1.substring(comma + 2, comma2 - 1);  //HEADLINE
          int comma3 = buffer1.indexOf(F(","), comma2+1);
          int comma4 = buffer1.indexOf(F(","), comma3+1);
          social[p][1] = buffer1.substring(comma3 + 2, comma4 - 1);  //TOPIC
          int comma5 = buffer1.indexOf(F(","), comma4 + 1);
          social[p][4] = buffer1.substring(comma4 + 1, comma5); //TIMESTAMP
          social[p][5] = F("2"); //STATE
          int comma6 = buffer1.indexOf(F(","), comma5 + 1);
          if (comma6 == -1)
          {
            social_count = p+1; break;
          }
          else
          {
            buffer1 = buffer1.substring(comma6 + 1, buf_len);
          }
          */
        }
      }
      /*
      int r;
      for (r = 0; r < social_count; r++)
      {
        MCU.print(F("Topic (")); MCU.print(r); MCU.print(F("): ")); MCU.println(social[r][1]);
        MCU.print(F("Source (")); MCU.print(r); MCU.print(F("): ")); MCU.println(social[r][2]);
        MCU.print(F("Headline (")); MCU.print(r); MCU.print(F("): ")); MCU.println(social[r][3]);
        MCU.print(F("Elapsed (")); MCU.print(r); MCU.print(F("): ")); MCU.println(social[r][4]);
        MCU.println();
        str_errs = 0; social_errors = 0;
      }
      */
    }
    else
    {
      social_errors++;
    }
  }
  
  else if (strid == F("e"))
  {
    //Ex: {"status":"ok","0":{"0":"Christophe <vr6r32@hotmail.com>","1":"RE: Dev Board Shipment","2":"Hi, This is the link I received when I did the order : https:\/\/legacy.trycelery.com\/myorders\/","3":1,"4":1427830458,"ID":"14c7153de38f852b"}}
    if ((buffer1.indexOf(F("status")) != -1) && (buffer1.indexOf(F("ok")) != -1))
    {
      int i1; int i2; //int a1; int a2; int a3;
      i1 = buffer1.indexOf(F("{"), (buffer1.indexOf(F("{"))+1)); i2 = buffer1.indexOf(F("}"));
      buffer1 = buffer1.substring(i1+1, i2); buffer1 = buffer1.substring(0, buffer1.lastIndexOf(F(","))); buffer1 = buffer1.substring(0, buffer1.lastIndexOf(F(",")));
      //STR_A = buffer1.substring((buffer1.lastIndexOf(F(","))+2), (buffer1.length()-1)); buffer1 = buffer1.substring(0, buffer1.lastIndexOf(F(",")));  buffer1 = buffer1.substring(0, buffer1.lastIndexOf(F(",")));
      //STR_B = buffer1.substring(1, (buffer1.indexOf(F(","))-1));
      //STR_C = buffer1.substring((buffer1.indexOf(F(","))+2), (buffer1.length()-1));
      
      //MCU.print(F("Sender: ")); MCU.println(STR_A);
      //MCU.print(F("Subject: ")); MCU.println(STR_B);
      //MCU.print(F("Message: ")); MCU.println(STR_C);
      MCU.println();
      //print_stream(STR_B, STR_C);
      str_errs = 0;
    }    
  
  }
  
  else
  {
    MCU.println(F("ERROR RETRIEVING CONTENT!")); str_errs++;
  }
  buffer1 = F("");
  wifi_icon(3); //interrupts();
}

int next_slot(int type) {
  //int j; 
  int result = 0;

  return result;
}

void reset_social() {

}

void reset_personal() {
    personal[0] = F("X");
    personal[1] = F("");
    personal[2] = F("");
    personal[3] = F("");
    personal[4] = F("");
    personal[5] = F("");
}
    

