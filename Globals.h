//
//  Globals.h
//  Glowdeck
//
//  Created by Justin Kaufman on 8/17/17.
//  Copyright © 2017 Justin Kaufman. All rights reserved.
//

#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <Arduino.h>

// Debugger
#define DEBUG                     1


// Bootloader
#define CPU_RESTART_ADDR          (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL           0x5FA0004
#define CPU_RESTART               (*CPU_RESTART_ADDR = CPU_RESTART_VAL);


// Display
#define LCD_CS                    20                              // CHIP SELECT
#define LCD_SCK                   13                              // SERIAL CLOCK
#define LCD_MOSI                  11                              // MASTER OUT SLAVE IN
#define LCD_DC                    2                               // REGISTER SELECT
#define LCD_RST                   5                               // RESET
#define LCD_BL                    6                               // BACKLIGHT PWM


// Ambient Sensor
#define AMB_LEVEL                 13                              // AMBIENT LIGHT SENSOR


// Lights
#define LED_PIN                   23                              // DATA IN
#define NUM_LEDS                  56                              // LED COUNT
#define BRIGHTNESS                255                             // LED BRIGHTNESS
#define FRAMES_PER_SECOND         120
#define LED_TYPE                  WS2812B                         // LED DRIVER (TECHNICALLY WS2812B)
#define COLOR_ORDER               GRB                             // RGB HARDWARE SPEC
#define DENSITY                   255
#define BITS_PER_DIRECTION_FLAG   1
#define STARTING_BRIGHTNESS       128
#define FADE_IN_SPEED             22
#define FADE_OUT_SPEED            20


// Coils
#define COILS                     12                              // CHARGING SENSOR
#define COIL_PW                   15                              // COIL SWITCH


// Buttons
#define LEFT_BUTTON               24                              // LEFT BUTTON
#define RIGHT_BUTTON              25                              // RIGHT BUTTON
#define FRONT_BUTTON              26                              // FRONT BUTTON


// Amp
#define AMP                       22                              // AMPLIFIER SWITCH
#define AUDIO_L                   3                               // AUDIO ANALOG-IN LEFT
#define AUDIO_R                   0                               // AUDIO ANALOG-IN RIGHT


// Wifi
#define WIFI_RESTORE              7                               // FACTORY RESTORE
#define WIFI_RESET                8                               // RESET SWITCH


// Bluetooth
#define PIO0                      27                              // VOLUME UP
#define PIO1                      28                              // VOLUME DOWN
#define PIO2                      29                              // MUSIC PLAY/PAUSE | CALL ANSWER/END
#define PIO4                      30                              // MUSIC BACKWARD
#define PIO5                      31                              // MUSIC FORWARD


// USB Detect
#define USB_IN                    16                              // DETECT MICRO-USB PLUG


// Ports
#define main                      Serial                          // FREESCALE MK20DX256 MCU
#define wifi                      Serial1                         // USR-WIFI232-T WIFI MODULE
#define btle                      Serial2                         // BC127 BLUETOOTH MODULE


// Miscellaneous
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


// Fonts
extern prog_uchar                 glowdeck28a[2417];
extern prog_uchar                 Avenir17a[6059];
extern prog_uchar                 Avenir9i[983];
extern prog_uchar                 Avenir18a[9409];
extern prog_uchar                 ClockFont[4892];
extern prog_uchar                 streams_icon[759];
extern prog_uchar                 glowdeck_icon[1742];


  

#endif







