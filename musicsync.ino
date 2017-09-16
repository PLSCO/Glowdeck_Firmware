//
//  Glowdeck_MusicSync.ino
//  Glowdeck
//
//  Created by Justin Kaufman on 12/14/2016
//  Copyright © 2016 Justin Kaufman. All rights reserved.
//

void musicSync() {
  if (samplingIsDone()) {
    arm_cfft_radix4_instance_f32 fft_inst;
    arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 0, 1);
    arm_cfft_radix4_f32(&fft_inst, samples);
    arm_cmplx_mag_f32(samples, magnitudes, FFT_SIZE);
    if (LEDS_ENABLED == 1) {
      spectrumLoop();
    }
    samplingBegin();
  }
  
  // parserLoop();
}

// Compute the average magnitude of a target frequency window vs. all other frequencies.
void windowMean(float* magnitudes, int lowBin, int highBin, float* windowMean, float* otherMean) {
    *windowMean = 0;
    *otherMean = 0;
    // Notice the first magnitude bin is skipped because it represents the
    // average power of the signal.
    for (int i = 1; i < FFT_SIZE/2; ++i) {
      if (i >= lowBin && i <= highBin) {
        *windowMean += magnitudes[i];
      }
      else {
        *otherMean += magnitudes[i];
      }
    }
    *windowMean /= (highBin - lowBin) + 1;
    *otherMean /= (FFT_SIZE / 2 - (highBin - lowBin));
}

// Convert a frequency to the appropriate FFT bin it will fall within.
int frequencyToBin(float frequency) {
  float binFrequency = float(SAMPLE_RATE_HZ) / float(FFT_SIZE);
  return int(frequency / binFrequency);
}

uint32_t pixelSetColor(float hue, float saturation, float value) {
  float chroma = value * saturation;
  float h1 = float(hue)/60.0;
  float x = chroma*(1.0-fabs(fmod(h1, 2.0)-1.0));
  float r = 0;
  float g = 0;
  float b = 0;
  if (h1 < 1.0) {
    r = chroma;
    g = x;
  }
  else if (h1 < 2.0) {
    r = x;
    g = chroma;
  }
  else if (h1 < 3.0) {
    g = chroma;
    b = x;
  }
  else if (h1 < 4.0) {
    g = x;
    b = chroma;
  }
  else if (h1 < 5.0) {
    r = x;
    b = chroma;
  }
  else // h1 <= 6.0
  {
    r = chroma;
    b = x;
  }
  float m = value - chroma;
  r += m;
  g += m;
  b += m;
  
  uint32_t rand = 0;
  return rand;
  //return pixels.Color(int(255*r), int(255*g), int(255*b));
  //leds[id] = CRGB(int(255*r), int(255*g), int(255*b));
}

void spectrumSetup() {
  // Set the frequency window values by evenly dividing the possible frequency
  // spectrum across the number of neo pixels.
  float windowSize = (SAMPLE_RATE_HZ / 2.0) / float(NUM_LEDS);
  for (int i = 0; i < NUM_LEDS+1; ++i) {
    frequencyWindow[i] = i*windowSize;
  }
  // Evenly spread hues across all pixels.
  for (int i = 0; i < NUM_LEDS; ++i) {
    hues[i] = 360.0*(float(i)/float(NUM_LEDS-1));
  }
}

void spectrumLoop() {
  // Update each LED based on the intensity of the audio in the associated frequency window.
  float intensity, otherMean;
  for (int i = 0; i < NUM_LEDS; ++i) {
    windowMean(magnitudes, frequencyToBin(frequencyWindow[i]), frequencyToBin(frequencyWindow[i+1]), &intensity, &otherMean);
    // Convert intensity to decibels
    intensity = 30.0*log10(intensity);  
    // Scale the intensity and clamp between 0.3 and 1.0
    intensity -= SPECTRUM_MIN_DB;
    intensity = intensity < 0.1 ? 0.1 : intensity;
    intensity /= (SPECTRUM_MAX_DB-SPECTRUM_MIN_DB);
    intensity = intensity > 1.0 ? 1.0 : intensity;
    // pixels.setPixelColor(i, pixelSetColor(hues[i], 1.0, intensity));
    leds[i] = CHSV(int(255*hues[i]), 255, int(255*intensity));
    //pixelSetColor(i, hues[i], 1.0, intensity);
  }
  FastLED.show();
}

void samplingCallback() {
  // Read from the ADC L and R and Average the Sample Data
  samples[sampleCounter] = (((float32_t)analogRead(AUDIO_INPUT_PIN) + (float32_t)analogRead(AUDIO_INPUT_PIN2))/2);
  
  // FFT function requires coefficient for imaginary portion of input
  // Because all data is real, set coefficient to zero
  samples[sampleCounter+1] = 0.0;
  
  // Update sample buffer position and stop after the buffer is filled
  sampleCounter += 2;
  if (sampleCounter >= FFT_SIZE*2) {
    samplingTimer.end();
  }
}

void samplingBegin() {
  // Reset sample buffer position and start callback at necessary rate
  sampleCounter = 0;
  samplingTimer.begin(samplingCallback, 1000000/SAMPLE_RATE_HZ);
}

boolean samplingIsDone() {
  return sampleCounter >= FFT_SIZE*2;
}









////////////////////////////////////////////////////////////////////////////////
// COMMAND PARSING FUNCTIONS
// These functions allow parsing simple commands input on the serial port.
// Commands allow reading and writing variables that control the device.
//
// All commands must end with a semicolon character.
// 
// Example commands are:
// GET SAMPLE_RATE_HZ;
// - Get the sample rate of the device.
// SET SAMPLE_RATE_HZ 400;
// - Set the sample rate of the device to 400 hertz.
// 
////////////////////////////////////////////////////////////////////////////////
/*
void parserLoop() {
  // Process any incoming characters from the serial port
  while (Serial.available() > 0) {
    char c = Serial.read();
    // Add any characters that aren't the end of a command (semicolon) to the input buffer.
    if (c != ';') {
      c = toupper(c);
      strncat(commandBuffer, &c, 1);
    }
    else
    {
      // Parse the command because an end of command token was encountered.
      parseCommand(commandBuffer);
      // Clear the input buffer
      memset(commandBuffer, 0, sizeof(commandBuffer));
    }
  }
}
// Macro used in parseCommand function to simplify parsing get and set commands for a variable
#define GET_AND_SET(variableName) \
  else if (strcmp(command, "GET " #variableName) == 0) { \
    Serial.println(variableName); \
  } \
  else if (strstr(command, "SET " #variableName " ") != NULL) { \
    variableName = (typeof(variableName)) atof(command+(sizeof("SET " #variableName " ")-1)); \
  }

void parseCommand(char* command) {
  if (strcmp(command, "GET MAGNITUDES") == 0) {
    for (int i = 0; i < FFT_SIZE; ++i) {
      Serial.println(magnitudes[i]);
    }
  }
  else if (strcmp(command, "GET SAMPLES") == 0) {
    for (int i = 0; i < FFT_SIZE*2; i+=2) {
      Serial.println(samples[i]);
    }
  }
  else if (strcmp(command, "GET FFT_SIZE") == 0) {
    Serial.println(FFT_SIZE);
  }

  GET_AND_SET(SAMPLE_RATE_HZ)
  GET_AND_SET(LEDS_ENABLED)
  GET_AND_SET(SPECTRUM_MIN_DB)
  GET_AND_SET(SPECTRUM_MAX_DB)
  
  // Update spectrum display values if sample rate was changed.
  if (strstr(command, "SET SAMPLE_RATE_HZ ") != NULL) {
    spectrumSetup();
  }
  
  // Turn off the LEDs if the state changed.
  if (LEDS_ENABLED == 0) {
    FastLED.clear();
    FastLED.show();
  }
}
*/

















