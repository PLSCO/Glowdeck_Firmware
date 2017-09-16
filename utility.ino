void validate_vars() {
  if ((PRV != 0) && (PRV != 1))
  {
    char pChar = EEPROM.read(1);
    PRV = (String(pChar)).toInt();
    if ((PRV != 0) && (PRV != 1))
    {
      PRV = 1;
      EEPROM.write(1, '1');
    }
  }
  if (SID.length() < 4)
  {
    SID = get_sid();
    if (SID.length() < 4)
    {
      SID = F("00000");
    }
  }
}

/*
String utf8ascii(String s) {     
  String r = "";
  char c;
  
  for (int i=0; i<s.length(); i++) {
    c = utf8ascii(s.charAt(i));
    if (c != 0) r += c;
  }
  
  return r;
}

void utf8ascii(char* s) {       
  int k = 0;
  char c;
  
  for (int i = 0; i < strlen(s); i++) {
    c = utf8ascii(s[i]);
    if (c != 0) s[k++] = c;
  }
  
  s[k] = 0;
}
*/

int analog_convert(int inp) {
  if (inp == 10) return 255;
  else if (inp == 0) return 0;
  else if (inp == 9) return 228;
  else if (inp == 8) return 205;
  else if (inp == 7) return 180;
  else if (inp == 6) return 150;
  else if (inp == 5) return 121;
  else if (inp == 4) return 84;
  else if (inp == 3) return 43;
  else if (inp == 2) return 20;
  else if (inp == 1) return 1;
  else return 255;
}

int bright_revert(int inp) {
  if (inp == 255) return 10;
  else if (inp == 0) return 0;
  else if (inp == 228) return 9;
  else if (inp == 205) return 8;
  else if (inp == 180) return 7;
  else if (inp == 150) return 6;
  else if (inp == 121) return 5;
  else if (inp == 84) return 4;
  else if (inp == 43) return 3;
  else if (inp == 20) return 2;
  else if (inp == 1) return 1;
  else return 10;
}

int hsv_convert(int inp) {
  int result; result = (inp * 5.1);
  return result;
}

String string_split(String parse, String div) {
  String result = ""; int p_len;
  
  p_len = parse.length();
  
  if (p_len < 1)
  {
    return result;
  }
  if (parse.indexOf(div) == -1)
  {
    return result;
  }
  return result;
}

int char_count(const char *str, char div) {
  const char *p = str; int count = 0;
  do
  {
    if (*p == div)
    {
      count++;
    }
  } while (*(p++));

  return count;
}

void eeprom_write_bytes(int startAddr, const byte* array, int numBytes) {
  // counter
  int i;
  for (i = 0; i < numBytes; i++) 
  {
    EEPROM.write(startAddr + i, array[i]);
  }
}

void eeprom_write_string(int addr, const char* string) {
  // actual number of bytes to be written
  int numBytes;

  // we'll need to write the string contents
  // plus the string terminator byte (0x00)
  numBytes = strlen(string) + 1;

  eeprom_write_bytes(addr, (const byte*)string, numBytes);
}

















