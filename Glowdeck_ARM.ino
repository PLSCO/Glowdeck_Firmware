#include <Arduino.h>
#include <EEPROM.h>
#include <Metro.h>
#include <TimeLib.h>
#include <Pixels_SPIhw.h>
#include <Pixels_ILI9341.h>
#include <FastLED.h>
#include "arm_math.h"
#define ARM_MATH_CM4

uint8_t DEBUG = 1;      // DEV MODE ON/OFF
volatile uint8_t BT_PREFIX = 9;  // BLE = 9 / SPP = 14

//A. GUI ASSETS
extern prog_uchar glowdeck_icon[1742] PROGMEM;      //SPLASH SCREEN ICON
extern prog_uchar glowdeck_icons24a[10183] PROGMEM; //GLOWDECK ICON SET
extern prog_uchar Avenir18a[9409] PROGMEM;          //STREAMS CONTENT FONT
extern prog_uchar ClockFont[4892] PROGMEM;          //CLOCKFACE FONT
extern prog_uchar streams_icon[759] PROGMEM;        //STREAMS ICON
extern prog_uchar news_icon[343] PROGMEM;           //NEWS ICON
extern prog_uchar messages_icon[341] PROGMEM;       //MESSAGES ICON

//B. HARDWARE SERIAL PORTS
#define MCU         Serial                          //MK20DX256 MICROCONTROLLER
#define USR         Serial1                         //USR-WIFI232-T WIFI MODULE
#define BC127       Serial2                         //BC127 BLUETOOTH MODULE

//C. LCD (2.4" TFT ILI9341)
#define LCD_CS      20                              //CHIP SELECT
#define LCD_SCLK    13                              //SERIAL CLOCK
#define LCD_MOSI    11                              //MASTER OUT SLAVE IN
#define LCD_DC      2                               //REGISTER SELECT
#define LCD_RSET    5                               //RESET
#define LCD_BL      6                               //BACKLIGHT PWM

//C. PHOTORESISTOR (GL5528)
#define AMB_LEVEL   13                              //AMBIENT LIGHT SENSOR

//D. RGB LEDS (WS2812B DRIVER)
#define LED_PIN     23                              //DATA IN
#define NUM_LEDS    56                              //LED COUNT
#define BRIGHTNESS  255                             //LED BRIGHTNESS
#define FRAMES_PER_SECOND  120
#define LED_TYPE    WS2812B                         //LED DRIVER (TECHNICALLY WS2812B)
#define COLOR_ORDER GRB                             //RGB HARDWARE SPEC
#define DENSITY     255
#define BITS_PER_DIRECTION_FLAG 1
#define STARTING_BRIGHTNESS 128
#define FADE_IN_SPEED       22
#define FADE_OUT_SPEED      20

//E. WIRELESS CHARGER (QI-A6 TRANSMITTER)
#define COILS       12                              //CHARGING SENSOR
#define COIL_PW     15                              //COIL SWITCH

//F. BUTTONS (TACTILE MOMENTARY SWITCHES)
#define PRESS_L     24                              //LEFT BUTTON
#define PRESS_R     25                              //RIGHT BUTTON
#define PRESS_F     26                              //FRONT BUTTON

//G. AMPLIFIER (MAX98400BETG+T)
#define AMP         22                              //AMPLIFIER SWITCH
#define AUDIO_L     3                               //AUDIO ANALOG-IN LEFT
#define AUDIO_R     0                               //AUDIO ANALOG-IN RIGHT

//H. WIFI MODULE (USR-WIFI232-T | SERIAL1)
#define WIFI_RESTORE 7                              //FACTORY RESTORE
#define WIFI_RESET   8                              //RESET SWITCH

//I. BLUETOOTH MODULE (BC127 | SERIAL2)
#define PIO0        27                              //VOLUME UP
#define PIO1        28                              //VOLUME DOWN
#define PIO2        29                              //MUSIC PLAY/PAUSE | CALL ANSWER/END
#define PIO4        30                              //MUSIC BACKWARD
#define PIO5        31                              //MUSIC FORWARD

//J. MICRO-USB DETECT (DFU MODE)
#define USB_IN      16                              //DETECT MICRO-USB PLUG

//K. TIME SYNC MESSAGES FROM SERIAL PORT
#define TIME_HEADER "T"                             //HEADER TAG FOR SERIAL TIME SYNC MESSAGE

bool nextGen = false;

int rtcCompensate = 862;

//A. STREAMS CONTAINERS
// String social[15][6];                            //0=TYPE; 1=A; 2=B; 3=C; 4=D; 5=STATE
String personal[6];                                 //0=TYPE; 1=A; 2=B; 3=C; 4=D; 5=STATE

String location = F(""); 
String temperature = F(""); 
String forecast = F("");
volatile long social_timer = 0; 
volatile long personal_timer = 0; 
volatile long weather_timer = 0;
uint8_t personal_count = 0; 
uint8_t personal_counter = -1;
uint8_t social_count = 0;
uint8_t social_counter = -1;
uint8_t soc_id = -1; 
uint8_t per_id = -1;
uint8_t weather_rotate = 1; 
uint8_t news_rotate = 0; 
uint8_t music_rotate = -1; 
uint8_t banner_rotate = 0;
int anmDelays[7];
volatile boolean messageOn = false;

// String EVENT[10][4];
//[0][x] == wakeUpAlarm
//   [0] == 0/1/2           ALARM ON/OFF (0 = OFF, 1 = ON, 2 = ACTIVE)
//   [1] == 10:00 AM        TIME SET (0 = 12:00AM, 2359 = 11:59PM)
//   [2] == 0-255, 999      HUE COLOR (999 = NO LIGHT ALERT)
//   [3] == 0-2             AUDIO TYPE (0 = NONE, 1 = SONG, 2 = TONE)
volatile boolean eventPress = false;
volatile uint8_t alarm = -1;
// String   btscan[15][3];

//M. STREAMS API DEFINED VARIABLES
volatile uint8_t  AMB = 0;                         //AMBIENT LIGHT LEVEL [NOW]
volatile uint8_t  ANA = 0;                         //ANIMATION ACTIVE
volatile uint8_t  ANM = -1;                        //CURRENT LED ANIMATION PATTERN [GET/SET]
volatile uint8_t  BUP = 0;                         //BATTERY LEVEL IS RISING (1) OR FALLING (0)
volatile uint8_t  BAT = 50;                        //BATTERY % LEVEL OF CONNECTED DEVICE [GET]
String  BTR = F("0");                              //REMOTE BLUETOOTH ADDRESS [NOW/SET]
volatile uint16_t COL[3];                          //LED ARRAY COLOR RGB [NOW/SET]
volatile boolean  CGS = 0;                         //SMART CHARGING [SET]
volatile boolean  CHG = 1;                         //WIRELESS CHARGER [NOW/SET]
volatile boolean  CHQ = 0;                         //DEVICE CHARGING [NOW]
boolean  DBA = 0;                                  //AUTO-DIM LCD BRIGHTNESS [SET]
volatile uint8_t  DBR = 10;                        //DISPLAY BRIGHTNESS [NOW/SET]
char     EEP[13];                                  //EEPROM BYTES 1-12 [NOW/SET]
uint8_t  EQZ = 3;                                  //EQUALIZER DEFAULT (3 ROCK) [NOW/SET]
volatile uint8_t LED = 1;                          //LED ARRAY [NOW/SET]
String   LNM = F("0");                             //BLUETOOTH LONG NAME [SET]
volatile uint8_t  MSL = 0;                         //MUSIC SYNC LIGHTS [SET]
volatile uint8_t  MSA = 0;                         //MUSIC SYNC ACTIVE [GET/SET]
volatile boolean  MST = 1;                         //MUSIC STREAM ENABLED [SET]
volatile boolean  NGT = 0;                         //NIGHT MODE ENABLED [SET]
volatile uint8_t  LSW = 1;                         //LED ARRAY MASTER SWITCH [SET]
volatile uint8_t  NOT = 0;                         //STREAMS NOTIFICATION DISPLAYED [GET]
boolean  ONB = 0;                                  //ONBOARDING COMPLETE [SET]
uint8_t  PRV = 1;                                  //DEVICE PROVISIONED FLAG [SET]
volatile uint8_t  POW = 1;                         //GLOWDECK POWER MODE [SET/GET]
uint8_t  REG = 0;                                  //DEVICE REGISTERED IN STREAMS DB [SET/GET]
uint8_t  PRC = 0;                                  //PRIVACY MODE SETTING [SET]
uint8_t  MOB = 1;                                  //MOBILE NOTIFICATIONS ENABLED [SET]
// String   PSW = F("");                           //WIFI PASSWORD
uint8_t  SCR = 0;                                  //STREAMS SCROLL ENABLED [SET]
volatile uint16_t CLR[3];
volatile int HUE = 999;
volatile int HUE_FROM = 999;
volatile boolean  SPIN_CLOCKWISE = true;
volatile int  LID = 25;
String   SID = F("0");                             //UNIQUE HARDWARE ID [SET] 
volatile uint8_t  SND = 0;                         //AUDIO SYSTEM TOGGLE
uint8_t  STR = 0;                                  //STREAMS ENABLED [SET]
// String   TKN = F("0");                          //STREAMS TOKEN [SET]
// String   UID = F("00000");                      //STREAMS USER ID [SET]
// String   UNM = F("");                           //USERNAME
uint8_t  VER = 94;                                 //GLOWDECK FIRMWARE VERSION [READ ONLY]
volatile uint8_t  VIB = 255;                                //LED BRIGHTNESS/VIBRANCE VALUE [GET/SET]
// String   WID = F("");                           //WIFI CURRENT SSID [SET]
uint8_t  WIO = 0;                                  //WIFI CONNECTED TO THE INTERNET
uint8_t  WIF = 0;                                  //WIFI ENABLED [SET]
// String  WIP = F("0");                           //WIFI PASSCODE [SET]
uint8_t  WIS = 3;                                  //WIFI CONNECTION STRENGTH
String   BTQ = F("");                              //COMMAND TO SEND TO BC127 WHEN AVAILABLE
volatile uint8_t  DEM = 0;

String equalizers[7];
String batLast = F("");
String bleIconLast = F("");
String trackTextLast = F("");

//       2. BLUETOOTH
volatile boolean  BTC = 0;                                  //BLUETOOTH CLASSIC CONNECTED FLAG
volatile boolean  BLE = 0;                                  //BLE PROFILE CONNECTED FLAG
volatile boolean  SPP = 0;                                  //SPP PROFILE CONNECTED FLAG
volatile boolean  A2DP = 0;                                 //A2DP PROFILE CONNECTED FLAG
volatile boolean  AVRCP = 0;                                //AVRCP PROFILE CONNECTED FLAG
volatile boolean  HFP = 0;                                  //HFP PROFILE CONNECTED FLAG
volatile boolean  PBAP = 0;                                 //PBAP PROFILE CONNECTED FLAG
volatile boolean  MAP = 0;                                  //MAP PROFILE CONNECTED FLAG

char bleAddress[12];
char btcAddress[12];
String bleAddr;
String btcAddr;

volatile uint8_t PANE = 1; // HOME (DEFAULT PANE)

// MARK: - GLOWDECK STATE VARIABLES
volatile uint8_t  leds_cutoff = 0;           //FLAG FOR CUTTING OFF LEDS LEFT ON FOR TOO LONG (POWER SAVING AND SAFETY MEASURE)
unsigned long     leds_start = 0;            //START COUNTING THE SECONDS SINCE THE LEDS WERE TURNED ON
volatile uint8_t  btr_flag = 0;     //NOTIFY DEVICE OF PROVISION SUCCESS ON REBOOT FLAG
volatile uint8_t  map_flag = 0;     //HAVE WE TRIED OPENING THE MAP CONNNECTION ONCE YET?
volatile uint8_t  hfp_flag = 0;     //HAVE WE TRIED OPENING THE HFP PROFILE YET?
volatile uint8_t  a2dp_flag = 0;    //HAVE WE TRIED OPENING THE A2DP PROFILE YET?
volatile uint8_t  ble_flag = 0;     
volatile uint8_t  btc_flag = 0;
volatile uint8_t  ver_flag = 0;
uint8_t  prv_flag = 0;
uint8_t  remap_flag = 0;   //HAVE WE TRIED REOPENING MAP PROFILE YET?
volatile uint8_t  msg_flag = 0;     //IS A NEW MESSAGE BEING DISPLAYED NOW?
uint8_t  ip_flag = 0;      //DO WE HAVE A LOCAL IP FOR OUR WEBSITE?
uint8_t  sta_flag = 0;     //ARE WE CURRENTLY DISPLAYING THE GLOWDECK URL?
uint8_t  wic_flag = 0;     //IS IOS CURRENTLY WAITING FOR A WIFI STATUS REPORT?
uint8_t  rec_flag = 0;     //RECONNECT TO BTC AFTER PROVISIONING AND RECONNECTING OVER BLE?
volatile uint8_t  attempts_flag = 0;//HOW MANY TIMES HAVE WE TRIED TO RECONNECT OVER BTC SINCE STARTUP?
uint8_t  echo_flag = 0;    //SHOULD WE ECHO A WIFI/BLUETOOTH UART RESPONSE OVER BLE/SPP?
uint8_t  str_flag = 0;      //SHOULD WE SWITCH TO THE STREAMS PANE CUZ SWITCH TURNED ON?
uint8_t  sketch_flag = 0;   //ARE WE IN THE MIDDLE OF A SKETCH?
uint8_t  time_flag = 0;     //HAVE WE SYNC'D TIME SINCE CONNECTING TO A DEVICE?
uint8_t  wtr_flag = 0;      //HAVE WE SYNC'D WEATHER SINCE CONNECTING TO A DEVICE?
uint8_t  bat_flag = 0;      //HAVE WE SYNC'D BATTERY LEVEL?
uint8_t  btcsend_flag = 0;
volatile uint8_t  msa_mode = 0;
volatile uint8_t  ana_mode = 0;
volatile boolean info_flag = false;
uint8_t  fuelLast = -1;
uint8_t  status_flag = 0;
volatile boolean init_flag = false;  //IS THE USER USING STREAMS APP OR GLOWDECK APP?
volatile boolean startup = true;

// MARK: - HOME PANE GUI
volatile uint8_t STR_ICON = 0; 
volatile uint8_t CHG_ICON = 0;

//       3. WIFI
// String   wiscan[20][5];    // STORAGE ARRAY FOR FOUND WIFI NETWORKS (0:SSID,1:AUTHENTICATION TYPE,2:ENCRYPTION METHOD,3:RSSI,4:PASSWORD)
uint8_t  ssid_id = -1;
uint8_t  ssid_count = -1;
uint8_t  str_errs = 0;
// String   sta_ip = F("");

// MARK: - WEATHER
String   tempLast = F("0");
String   foreLast = F("0");
uint8_t  weatherWait = 0;

// MARK: - AUDIO
volatile uint8_t  volume = 50;     //VOLUME STATE (RANGE 0-127);
volatile uint8_t  music = 0;       //MUSIC STATE (0=OFF;1=PLAY;2=PAUSE)
String   song = F("");             //SONG TITLE
String   artist = F("");           //ARTIST NAME
volatile uint8_t  voice = 0;       //VOICE ASSISTANT STATE (0=OFF;1=ON)
volatile uint8_t  call = 0;        //CALL STATE (0=NO CALL;1=INCOMING;2=OUTGOING;3=ACTIVE;4=ENDED)

// MARK: - LEDS
CRGB     leds[NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
TBlendType    BLEND;
enum { GETTING_DARKER = 0, GETTING_BRIGHTER = 1 };
uint8_t directionFlags[NUM_LEDS];

// MARK: - Streams vars
//uint8_t stream_cycler = 0;
uint8_t msgPulsed = false;
uint8_t email_count = 0;
uint8_t email_counter = 0;
uint8_t social_errors = 0;

// MARK: - Press vars
uint8_t  debounce = 148;       //MILLISECONDS TO DEBOUNCE BUTTONS (HIGHER VALUES WILL FILTER OUT MORE PRESSES)
uint16_t hold_time = 650;      //THRESHOLD MILLISECONDS DOWN FOR A PRESS TO REGISTER AS A HOLD
volatile unsigned long last_micros_f; volatile unsigned long last_micros_l; volatile unsigned long last_micros_r;
volatile uint8_t hold_f = 0; volatile uint8_t hold_l = 0; volatile uint8_t hold_r = 0; 
volatile uint8_t click_f = 0; volatile uint8_t click_l = 0; volatile uint8_t click_r = 0;

// MARK: - Clock vars
uint8_t days = 0; 
uint8_t date = 0; 
uint8_t months = 0; 
uint16_t years = 0; 
uint8_t hours = 0; 
uint8_t minutes = 0; 
uint8_t seconds = 0; 
uint8_t hoursLast = 0; 
uint8_t minutesLast = 0; 
uint8_t monthsLast = 0; 
uint8_t dateLast = 0;

// MARK: - Music sync vars
int SAMPLE_RATE_HZ = 9000;             // Sample rate of the audio in hertz.
float SPECTRUM_MIN_DB = 3.1;           // Audio intensity (in decibels) that maps to low LED brightness.
float SPECTRUM_MAX_DB = 63.2;          // Audio intensity (in decibels) that maps to high LED brightness.
int LEDS_ENABLED = 1;                  // Control if the LED's should display the spectrum or not.  1 is true, 0 is false.
const int FFT_SIZE = 256;              // Size of the FFT.  Realistically can only be at most 256 
const int AUDIO_INPUT_PIN = 3;         // Input L ADC pin for audio data.
const int AUDIO_INPUT_PIN2 = 0;        // Input R ADC pin for audio data.
const int ANALOG_READ_RESOLUTION = 10; // Bits of resolution for the ADC.
const int ANALOG_READ_AVERAGING = 16;  // Number of samples to average with each ADC reading.
const int MAX_CHARS = 65;              // Max size of the input command buffer
float samples[FFT_SIZE*2];
float magnitudes[FFT_SIZE];
int sampleCounter = 0;
char commandBuffer[MAX_CHARS];
float frequencyWindow[NUM_LEDS+1];
float hues[NUM_LEDS];
volatile int iterations = 0; 
volatile int sync_routine = 0;

// 9. SERIAL
String buffer; 
String process;
uint8_t buffer2[256];

// Prototypes
void ChangePalettePeriodically();
void FillLEDsFromPaletteColors(uint8_t colorIndex);
void SetupPurpleAndGreenPalette();
void SetupBlackAndWhiteStripedPalette();
void SetupTotallyRandomPalette();
void ISRSUB_F();
void ISRSUB_L();
void ISRSUB_R();
void ISR_F();
void ISR_L();
void ISR_R();
void addGlitter(fract8 chanceOfGlitter);
void adjustBrightness(uint8_t newVal);
int amb_convert(int inp);
int analog_convert(int inp);
void bc127_firmer();
void bc127_handler(String cmd);
String bc127_response();
void bc127_status();
void ble_advertise();
void ble_icon(int on);
void bleRead();
void bpm();
void bpm_extend();
int bright_revert(int inp);
void brightenOrDarkenEachPixel(fract8 fadeUpAmount, fract8 fadeDownAmount);
String bt_addr();
void bt_change(String cmd);
void bt_link(String cmd);
String bt_name();
String bt_remote();
void bt_state(String cmd);
void bt_status();
void btc_discoverable();
void btc_handler(String cmd);
void btc_icon(int on);
void btq_handler();
void busy_icon(int on);
int  char_count(const char *str, char div);
void charge_icon(int on);
void chooseColorPalette();
void clear_info();
void clear_stream();
void clockSet(unsigned long timeCode);
void colorPick();
void colorTone(int r, int g, int b);
void colorWheel();
void confetti();
void confetti_extend();
void cross_fade(int c0, int c1, int c2, int speed);
void crossfade_hue();
void detach_interrupts();
void displayNotification(String src, String head, String det, String elapsed);
void dfu_mode();
void draw_sketch(String cmd);
void eeprom_write_bytes(int startAddr, const byte* array, int numBytes);
void eeprom_write_string(int addr, const char* string);
int  eventDismissCheck();
void eventHandler();
void eventPerform(int id);
void event_banner(String txt);
void fadecross_leds();
void fadein_leds();
void fadeout_leds();
void fadepad_led(int id, int red, int green, int blue, int alpha, int sound);
void fadepad_leds();
void force_factory();
void force_provision();
int frequencyToBin(float frequency);
void front_btn(uint8_t type);
bool getPixelDirection(uint16_t i);
String get_sid();
void get_stream(String strid);
void home_pane();
String hrConvert(int inp);
int hsv_convert(int inp);
void init_adc();
void init_audio();
void init_bc127();
void init_bcpins();
void init_clock();
void init_coils();
void init_containers();
void init_interrupts();
void init_lcd();
void init_leds();
void init_press();
void init_settings();
void init_sys();
void init_ui();
void init_usb();
void init_usr();
void juggle();
void juggle_extend();
void lcd_scroll();
void ledChargeBegin();
void ledConnectionRoutine();
void animationStart(uint8_t type);
void animationStop();
void ledsAnimate();
void ledShow();
void ledfade_icon(int c1, int c2, int c3);
void leds_front(int r, int g, int b, int speed);
void leds_icon(int on);
void leds_left(int r, int g, int b, int speed);
void leds_off();
void leds_pulse();
void leds_right(int r, int g, int b, int speed);
void left_btn(uint8_t type);
void light_pad();
void light_swirl();
void loop();
CRGB makeBrighter(const CRGB& color, fract8 howMuchBrighter);
CRGB makeDarker(const CRGB& color, fract8 howMuchDarker);
void mcu_handler(String cmd);
void mcuRead();
String monthConvert(int inp, int abbrv);
void msg_pulse();
void fade_pulse();
void musicSync();
void music_sync();
void music_sync_extend();
void music_sync_start();
void music_sync_start_demo();
void music_sync_stop();
void music_sync_stop_demo();
void music_sync_x();
void nextPattern();
int next_slot(int type);
void notificationHandler(String cmd);
void notificationExpired();
void personal_back();
void personal_banner(String txt);
void personal_next();
void personal_pane();
void personal_print();
uint32_t pixelSetColor(float hue, float saturation, float value);
void play_tone(int tone_id);
void pop_message();
void power_off();
void power_on();
void print_info(String content);
void print_message();
void print_stream(String content_a, String content_b);
void print_track();
unsigned long processSyncMessage();
void rainbow();
void rainbowWithGlitter();
void reattach_interrupts();
void reset_personal();
void reset_social();
void right_btn(uint8_t type);
void samplingBegin();
void samplingCallback();
boolean samplingIsDone();
void scrollLeft();
void scrollRight();
void send_init();
void setPixelDirection(uint16_t i, bool dir);
void setup();
void sinelon();
void sinelon_extend();
void sketch_banner();
void sketch_pane();
void smart_charge();
void social_back();
void social_banner(String txt);
void social_next();
void social_pane();
void social_print();
void spectrumLoop();
void spectrumSetup();
void ssid_log(String cmd);
boolean startATSequence();
void stream();
void streams_banner();
String string_split(String parse, String div);
void sub_interrupts();
void switch_leds();
void synchronize();
void tone_play(String note, int octave, String tnote, int tempo, int timbre, int vol, int dec, int len);
void twinkle();
void update_bright();
void update_charge();
void update_clock();
void update_usb();
void usr_atmode();
boolean usr_cmdmode();
void usr_handler(String cmd);
String usr_response(int contentLength);
int usr_setup();
void validate_vars();
void vol_icon(int lev);
void wifi_icon(int on);
void wifi_status();
void windowMean(float* magnitudes, int lowBin, int highBin, float* windowMean, float* otherMean);

// MARK: - ANIMATIONS
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, twinkle };
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
volatile uint8_t gCurrentPatternNumber = 0;
volatile uint8_t gHue = 0;
volatile unsigned long hueTimer = millis();
unsigned long patternTimer = millis();
CRGBPalette16 gPalette;

// MARK: - Pixels library
Pixels pxs(320, 240);

// MARK: - Interval timers
IntervalTimer notificationTimer;
IntervalTimer samplingTimer;
IntervalTimer music_sync_timer;
IntervalTimer animationTimer;

Metro GUI = Metro(1070);
Metro LINK2 = Metro(17500);
Metro ROTATE = Metro(6050);
Metro HARDWARE = Metro(2085);
Metro STREAMS = Metro(20000);
Metro SAFE = Metro(1200000);

void setup() {
  buffer.reserve(200);
  process.reserve(200);
  init_containers();
  init_adc();
  init_bcpins();
  init_audio();
  init_coils();
  init_press();
  init_usb();
  init_clock();
  pinMode(21, INPUT);
  init_leds();
  init_lcd();
  init_sys();
  init_settings();
  init_bc127();
  // init_usr();
  init_interrupts();
  init_ui();
}

void loop() {
  stream();
  synchronize();
}













