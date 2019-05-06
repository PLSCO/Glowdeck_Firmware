//
//  LEDManager.cpp
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//

#include "LEDManager.h"
#include <FastLED.h>

FASTLED_USING_NAMESPACE

/*
enum LEDState {
  FADE = 0,
  EMPTY = 1,
  SOLID = 2,
  ANIMATE = 3,
};
*/

// FastLED base object
CRGB leds[NUM_LEDS];

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { LEDManager::rainbow, LEDManager::confetti, LEDManager::sinelon, LEDManager::juggle, LEDManager::bpm };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0;               // rotating "base color" used by many of the patterns


LEDManager::LEDManager(int state) {
  ledState = state;
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
}

void LEDManager::setup() {
  FastLED.setBrightness(BRIGHTNESS);
}

void LEDManager::loop() {

  switch (this->ledState) {
    case -1:
      fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0)); 
      FastLED.show();

      ledState = 0; // EMPTY;
      
      break;
    case 0:

      break;
    case 1:

      break;
    case 2:
    
      // Call the current pattern function once, updating the 'leds' array
      gPatterns[gCurrentPatternNumber]();
    
      // Send the 'leds' array out to the actual LED strip
      FastLED.show();  
      
      // Insert a delay to keep the framerate modest
      FastLED.delay(1000/FRAMES_PER_SECOND); 
    
      // Do some periodic updates
      EVERY_N_MILLISECONDS(20)  { gHue++; } // slowly cycle the "base color" through the rainbow
      EVERY_N_SECONDS(10)       { nextPattern(); } // change patterns periodically
      
      break;      
  }

}

void LEDManager::startAnimating() {
  ledState = 2;
}
  
void LEDManager::off() {
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0)); 
  FastLED.show();

  ledState = 0;
}

void LEDManager::nextPattern() {
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
}

void LEDManager::rainbow() {
  // FastLED's built-in rainbow generator
  fill_rainbow(leds, NUM_LEDS, gHue, 7);
}

void LEDManager::confetti() {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(gHue + random8(64), 200, 255);
}

void LEDManager::sinelon() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS-1);
  leds[pos] += CHSV(gHue, 255, 192);
}

void LEDManager::bpm() {
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void LEDManager::juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds, NUM_LEDS, 20);
  byte dothue = 0;
  for (int i = 0; i < 8; i++) {
    leds[beatsin16(i+7, 0, NUM_LEDS-1)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
