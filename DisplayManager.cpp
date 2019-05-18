//
//  DisplayManager.cpp
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//

#include "DisplayManager.h"
#include "Pixels_SPIhw.h"
#include "Pixels_Antialiasing.h"
#include "Pixels_ILI9341.h"
#include "Fonts.h"

extern GlowdeckManager glowdeckManager;

Pixels pxs(320, 240);

String streamTypes[8] = {"weather", "instagram", "calls", "facebook", "messages", "twitter", "email", "news"};
String firmwareBuild = "95";

DisplayManager::DisplayManager(uint8_t clk, uint8_t mosi, uint8_t cs, uint8_t rst, uint8_t dc, uint8_t bl) : _clk(clk), _mosi(mosi), _cs(cs), _rst(rst), _dc(dc), _bl(bl) {
  pinMode(_clk, OUTPUT);
  pinMode(_mosi, OUTPUT);
  pinMode(_cs, OUTPUT);
  pinMode(_rst, OUTPUT);
  pinMode(_dc, OUTPUT);
  pinMode(_bl, OUTPUT);
  pinMode(21, INPUT);

  pxs.setSpiPins(_clk, _mosi, _cs, _rst, _dc);
  pxs.init();
  pxs.setOrientation(LANDSCAPE_FLIP);
  pxs.setOriginAbsolute();

  analogWrite(_bl, 255);
}

void DisplayManager::setup() {
  pxs.setBackground(0, 0, 0);
  pxs.setColor(255, 255, 255);
  pxs.clear();
  pxs.drawCompressedBitmap(121, 65, glowdeck_icon);
  printStream("general", "Glowdeck v2.0", "", "");
}

void DisplayManager::loop() {
  // streamsDemo();
}

void DisplayManager::streamsDemo() {
  pxs.setColor(255, 255, 255);
  pxs.clear();
  
  // Weather
  printStream("weather", "Partly Cloudy", "46 | 3:44pm", "");
  delay(2500);
  pxs.setColor(0, 0, 0);
  pxs.fillRectangle(0, 198, 320, 42);
  
  // Instagram
  printStream("instagram", "what are you doing?", "@quickstagramm", "");
  delay(2500);
  pxs.setColor(0, 0, 0);
  pxs.fillRectangle(0, 198, 320, 42);
  
  // Calls
  printStream("calls", "Incoming Call", "Jennifer Smith", "");
  delay(2500);
  
  // Facebook
  printStream("facebook", "happy birthday pal", "Joe McMann", "");
  delay(2500);
  
  // Messages
  printStream("messages", "where are you", "Melissa Quick", "");
  delay(2500);
  
  // Twitter
  printStream("twitter", "streams is getting a makeover", "glowdeck", "streams");
  delay(2500);
  
  // Email
  printStream("email", "Re: meeting tomorrow", "John Smith", "");
  delay(2500);
  
  // News
  printStream("news", "NJ lottery jackpot grows", "NYTimes", "lottery");
}

void DisplayManager::debugPrint(String text) {
  pxs.clear();
  pxs.setColor(255, 255, 255);
  pxs.print(0, 0, text);
}

void DisplayManager::printStream(String type, String text, String source, String keyword) {
  printStreamSpectrum(type, 7);
  printStreamBorder();
  printTypeIcon(type);
  printHeadline(text, keyword);
  printSource(source);
}

void DisplayManager::printBluetooth(uint8_t on) {
  if (on == 1) {
     // pxs.drawCompressedBitmap(6, 6, btdIcon);
  }
}

void DisplayManager::printWifi(uint8_t on) {
  if (on == 1) {
    pxs.setFont(glowdeck28a);
    pxs.setColor(255, 255, 255);
    pxs.print(147, 4, "I");  
  }
}

void DisplayManager::printTypeIcon(String type) {
  int x = 3;
  int y = 201;
  
  String icon = "?";
  pxs.setColor(0, 0, 0);
  pxs.fillRectangle(0, 197, 43, 43);

  if (type == "general") {
    pxs.drawCompressedBitmap(x + 1, y + 2, streams_icon);
    return;
  }
  else if (type == "calls") {icon = "@"; pxs.setColor(235, 32, 38);}
  else if (type == "messages") {icon = "\\"; pxs.setColor(8, 114, 185);}
  else if (type == "email") {icon = "^"; pxs.setColor(105, 188, 69);}
  else if (type == "weather") {icon = "f"; pxs.setColor(252, 238, 33);}
  else if (type == "news") {icon = "f"; pxs.setColor(250, 175, 58);}
  else if (type == "twitter") {icon = "<"; pxs.setColor(0, 173, 238);}
  else if (type == "facebook") {icon = ";"; pxs.setColor(103, 47, 143);}
  else if (type == "instagram") {icon = "-"; pxs.setColor(250, 175, 58);}
  else if (type == "music") {icon = "d"; pxs.setColor(235, 32, 38);}
  
  pxs.setFont(glowdeck28a);
  pxs.print(x, y, icon);
}

void DisplayManager::printHeadline(String text, String focusWord) {
  int x = 44;
  int y = 201;
  int s = 4;
  
  pxs.setFont(Avenir18a);
  
  String word = "";   
  String a = "";
  String b = ""; 
  String c = "";
  
  int focusFlag = 0; 
  int abc = 0;
  
  pxs.setColor(0, 0, 0);
  pxs.fillRectangle(x - 1, y - 1, 275, 42);
  pxs.setColor(255, 255, 255);
  
  if (text.indexOf(" ") != -1) {
    if (focusWord.length() >= 2) {
      if (text.indexOf(focusWord) != -1) {
        while (text.length() >= 1) {
          if (text.indexOf(" ") != -1) {
            word = text.substring(0, text.indexOf(" "));
            text = text.substring(text.indexOf(" ") + 1, text.length());
          } else {
            word = text;
            text = "";
          }
        
          if (word.indexOf(focusWord) != -1) {
            if (focusFlag == 0) { 
              if (a == "") {a = word; abc = 1;}
              else if (b == "") {b = word; abc = 2;}
              else if (c == "") {c = word; abc = 3;}
              
              focusFlag = 1;
            }
          } else {
            if (focusFlag == 0) {
              if (a != "") a += " ";
              a += word;
            } else {
              if (b == "") {
                if (b != "") b += " ";
                b += word;
              } else {
                if (c != "") c += " ";
                c += word;
              }
            }
          }
        }
        
        if (focusFlag == 0) {
          pxs.print(x, y, a);
        } else if (abc == 1) {
          pxs.setColor(0, 173, 238);
          pxs.print(x, y, a);
          int x2 = pxs.getTextWidth(a) + s;
          pxs.setColor(255, 255, 255);
          pxs.print(x + x2, y, b);
          int x3 = pxs.getTextWidth(b) + s;
          pxs.setColor(255, 255, 255);
          pxs.print(x + x2 + x3, y, c);
        } else if (abc == 2) {
          pxs.setColor(255, 255, 255);
          pxs.print(x, y, a);
          int x2 = pxs.getTextWidth(a) + s;
          pxs.setColor(0, 173, 238);
          pxs.print(x + x2, y, b);
          int x3 = pxs.getTextWidth(b) + s;
          pxs.setColor(255, 255, 255);
          pxs.print(x + x2 + x3, y, c);
        }
      } else {
        pxs.print(x, y, text);
        return;
      }
    } else {
      pxs.print(x, y, text);
      return;
    }
  } else {
    pxs.print(x, y, text);
  }
}

void DisplayManager::printSource(String text) {
  pxs.setColor(0, 0, 0);
  pxs.fillRectangle(160, 220, 160, 20);
  pxs.setColor(220, 220, 220);
  pxs.setFont(Avenir17a);
  
  int textWidth = pxs.getTextWidth(text);
  int x = 310 - textWidth;
  int y = 224;
  
  pxs.print(x, y, text);
}

void DisplayManager::printStreamSpectrum(String active, int q) {
  int key = 1;
  
  for (int i = 0; i < 7; i++) {
    if (streamTypes[i] == active) {
      key = i;
      break;
    }
  }
  
  for (int i = 0; i < q; i++) {
    printStreamSquare(streamTypes[key], i, q);
    key = (key + 1) % 7;
  }
}

void DisplayManager::printStreamSquare(String type, int id, int q) {
    if (type == "general") pxs.setColor(0, 173, 238);           // GENERAL (LIGHT BLUE)
    if (type == "weather") pxs.setColor(252, 238, 33);          // WEATHER (YELLOW)
    else if (type == "instagram") pxs.setColor(250, 175, 58);   // INSTAGRAM (ORANGE)
    else if (type == "calls") pxs.setColor(235, 32, 38);        // MUSIC/CALLS (RED)
    else if (type == "facebook") pxs.setColor(103, 47, 143);    // FACEBOOK (PURPLE)
    else if (type == "messages") pxs.setColor(8, 114, 185);     // MESSAGES (DARK BLUE)
    else if (type == "twitter") pxs.setColor(0, 173, 238);      // TWITTER (LIGHT BLUE)
    else if (type == "email") pxs.setColor(105, 188, 69);       // EMAIL (GREEN)
    else if (type == "news") pxs.setColor(250, 175, 58);        // NEWS (ORANGE)
    
    if (q > 7) q = 7;
    else if (q < 1) q = 1;
    
    if (id > q) id = q;
    else if (id < 0) id = 0;
  
    int totalWidth = 308;
    int w = totalWidth / q;
    int x = 6 + (w * id);
    int y = 178 + 8; 
    int h = 8;
    
    pxs.fillRectangle(x, y, w, h);
}

void DisplayManager::printStreamBorder() {
  /*
  int totalWidth = 308; int h = 17;
  int x = 6; int y = 178;
  
  pxs.setColor(70, 70, 70);
  pxs.drawLine(x, y-1, x+totalWidth-1, y-1);
  pxs.drawLine(x, y+h, x+totalWidth-1, y+h);
  */
}
