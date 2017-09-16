void init_coils()
{
  pinMode(COIL_PW, OUTPUT); pinMode(COILS, INPUT); digitalWrite(COIL_PW, HIGH);
}

void update_charge()
{
  int tmp_coils; int tmp_coils2; int tmp_coils3; 
  tmp_coils = analogRead(COILS);
  
  if (tmp_coils <= 2)
  {
    tmp_coils = analogRead(COILS); tmp_coils2 = analogRead(COILS); tmp_coils3 = analogRead(COILS);
    if ((tmp_coils <= 2) && (tmp_coils2 <= 2) && (tmp_coils3 <= 2))
    {
      if (CHQ == 1)
      {
        CHQ = 0; charge_icon(0); CHG_ICON = 0; delay(325);
        if (BLE == 1)
        {
          BC127.print(F("SEND CHQ:0^\n\r")); BC127.flush();
        }
      }
      //fuelLast = -1;
      CHQ = 0; return;
    }
  } 
  else if (tmp_coils > 8)
  {
    tmp_coils = analogRead(COILS);
    if (tmp_coils > 8)
    {
      if (CHQ == 0)
      {
        tmp_coils = analogRead(COILS); tmp_coils2 = analogRead(COILS); tmp_coils3 = analogRead(COILS);
        if ((tmp_coils >= 9) && (tmp_coils2 >= 9) && (tmp_coils3 >= 9))
        {
          if ((BLE == 1) || (SPP == 1))
          {
            if (BLE == 1)
            {
              BC127.print(F("SEND CHQ:1^\n\r")); BC127.flush(); delay(100);
              BC127.print(F("SEND BAT^\n\r"));  BC127.flush();
            }
            else if (SPP == 1)
            {
              BC127.print(F("SEND BA^\n\r")); 
            }
            
            //String res = bc127_response();
            /*
            if (res.indexOf(F("BAT:")) != -1) //PHONE CHARGE LEVEL
            {
              int chg_val = (res.substring((res.indexOf(F(":"))+1), res.indexOf(F("^")))).toInt();
              BAT = chg_val;
              //MCU.print(F("FUEL: ")); MCU.println(BAT);
            }
            */
            //BTQ = F("SEND BAT^");
          }

          charge_icon(1);
       
          /*
          if ((NGT != 1) && (LED != 0) && (LSW != 0))
          { 
            int t1 = COL[0]; int t2 = COL[1]; int t3 = COL[2];
            COL[0] = random(0, 255); COL[1] = random(0, 255); COL[2] = random(0, 255);
            fadecross_leds(); delay(65); 
            COL[0] = t1; COL[1] = t2; COL[2] = t3;
            fadecross_leds();
            if ((HUE == 0) && (COL[0]+COL[1]+COL[2] == 0))
            {
              FastLED.clear();
            }
          }
          */
          CHQ = 1; CHG_ICON = 1;
        }
        return;
      }
      else
      {
        tmp_coils = analogRead(COILS); tmp_coils2 = analogRead(COILS); tmp_coils3 = analogRead(COILS);
        if ((tmp_coils >= 9) && (tmp_coils2 >= 9) && (tmp_coils3 >= 9))
        {
          if (CHG_ICON == 0)
          {
            charge_icon(1); CHQ = 1;
          }
  
          if (CHG_ICON != 1)
          {
            charge_icon(1);
         
            /*
            if ((NGT != 1) && (LED != 0) && (LSW != 0))
            { 
              int t1 = COL[0]; int t2 = COL[1]; int t3 = COL[2];
              COL[0] = random(0, 255); COL[1] = random(0, 255); COL[2] = random(0, 255);
              fadecross_leds(); delay(65); 
              COL[0] = t1; COL[1] = t2; COL[2] = t3;
              fadecross_leds();
              if ((HUE == 0) && (COL[0]+COL[1]+COL[2] == 0))
              {
                FastLED.clear();
              }
            }
            */
          }
          CHQ = 1; CHG_ICON = 1;
        }
      }
    }
  }
}

void smart_charge()
{
  if ((CHQ == 1) && (CHG == 1))
  { 
    if (BAT >= 97)
    {
      //MCU.println(F("Smart Charge: OFF"));
      print_info(F("* smart charge: off *")); info_flag = true;
      CHG = 0; digitalWrite(COIL_PW, LOW); 
      if (BLE == 1)
      {
        BC127.print(F("SEND CHG:0^\n\r")); BC127.flush(); 
      }
      return;
    }
  }
  else if (CHG == 0)
  {
    if ((BLE == 1) || (SPP == 1))
    {
      if (BAT < 91)
      {
        //MCU.println(F("Smart Charge: ON"));
        print_info(F("* smart charge: on *")); info_flag = true;
        CHG = 1; digitalWrite(COIL_PW, HIGH);
      }
    }
    else
    {
      CGS = 0; CHG = 1; digitalWrite(COIL_PW, HIGH);
      //MCU.println(F("Smart Charge N/A"));
    }
  }
}



















