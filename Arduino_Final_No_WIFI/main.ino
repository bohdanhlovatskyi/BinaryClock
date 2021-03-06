#include <Adafruit_NeoPixel.h>
#include <TimeLib.h>
#include <RTClib.h>
#include <Wire.h>

#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define SECONDS_PIN 7 // On Trinket or Gemma, suggest changing this to 1
#define MINUTES_PIN 8
#define HOURS_PIN 9

#define NUMPIXELSSMALL 30
#define NUMPIXELSBIG 60
#define DIGIT 5
#define SECONDS_BLOCKS 6
#define MINUTES_BLOCKS 6
#define HOURS_BLOCKS 12
#define CENTRE DIGIT/2
#define LED_PIN 13
#define CONTROL_SYMBOL 's'
#define DELAYVAL 500 // Time (in milliseconds)


RTC_DS3231 rtc;
char t[32];

// for the uart setup time function (serial port)
int curHours;
int curMins;
int curSecs;

Adafruit_NeoPixel seconds(NUMPIXELSSMALL, SECONDS_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel minutes(NUMPIXELSSMALL, MINUTES_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel hours(NUMPIXELSBIG, HOURS_PIN, NEO_GRB + NEO_KHZ800);

// colors for the strips
int seconds_clr[3] = {0, 255, 0};
int mins_clr[3] = {255, 0, 0};
int hours_clr[3] = {0, 0, 255};

// --------------------- Functions declaration -----------------------------------------
void _light_one(int idx, Adafruit_NeoPixel *time_type, const int clr[3]);
void _light_zero(int idx, Adafruit_NeoPixel *time_type, const int clr[3]);
void writeBlock(int num, Adafruit_NeoPixel *time_type, int size_block, const int clr[3]);
void writeTime(int second_time, int minute_time, int hour_time);
bool readTime(int &outHours, int &outMins, int &outSecs);
bool parseTime(String timeStr, int &outHours, int &outMins, int &outSecs);


void setup() {
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  seconds.begin();
  minutes.begin();
  hours.begin();

  Serial.begin(9600);
  Wire.begin();
  // rtc is connected to the default sda and sclk pins !
  if (!rtc.begin()) {
    Serial.println("Could not start the clock");
    while(1);
  }
  // adjusts the rtc with the time of file compilation
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));

}


void loop() {
  DateTime now = rtc.now();
  if (Serial.available()) {
    readTime(curHours, curMins, curSecs);
    rtc.adjust(DateTime(now.year(), now.month(), now.day(), curHours, curMins, curSecs));
  }

  sprintf(t, "%02d:%02d:%02d %02d/%02d/%02d",  now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());  
  Serial.println(t);

  writeTime(now.second(), now.minute(), now.hour());
}


void _light_one(int idx, Adafruit_NeoPixel *time_type, const int clr[3]){
  for (int i = idx * DIGIT; i < (idx + 1) * DIGIT; i++) {
    time_type->setPixelColor(i, seconds.Color(clr[0], clr[1], clr[2]));
  }
}

void _light_zero(int idx, Adafruit_NeoPixel *time_type, const int clr[3]){
   time_type->setPixelColor(idx * DIGIT+CENTRE, seconds.Color(clr[0], clr[1], clr[2]));
}

void writeBlock(int num, Adafruit_NeoPixel *time_type, int size_block, const int clr[3]){
  int one_bit = 0;

  for (int i = 0; i < size_block; i++){
    one_bit = bitRead(num, i);

    switch (one_bit){
      case 0:
        _light_zero(i, time_type, clr);
        break;
      case 1:
        _light_one(i, time_type, clr);
        break;
    }
  }
  
}

void writeTime(int second_time, int minute_time, int hour_time){
  writeBlock(second_time, &seconds, SECONDS_BLOCKS, seconds_clr);
  writeBlock(minute_time, &minutes, MINUTES_BLOCKS, mins_clr);
  writeBlock(hour_time, &hours, HOURS_BLOCKS, hours_clr);
  seconds.show();
  minutes.show();
  hours.show();
  delay(497);
  seconds.clear();
  minutes.clear();
  hours.clear();
}

// parse and validate time
bool parseTime(String timeStr, int &outHours, int &outMins, int &outSecs) {
  if ((timeStr.charAt(2) != ':') || (timeStr.charAt(5) != ':')) {
    return false;
  }
  if (timeStr.length() != 8) {
    return false;
  }
  int hours = timeStr.substring(0, 2).toInt();
  int mins = timeStr.substring(3, 5).toInt();
  int secs = timeStr.substring(6, 8).toInt();
  if (!((0 <= hours) && (hours < 24) && (0 <= mins) && (mins < 60) && (0 <= secs) && (secs < 60))) {
    return false;
  }
  outHours = hours;
  outMins = mins;
  outSecs = secs;
  return true;
}

bool readTime(int &outHours, int &outMins, int &outSecs) {
  char controlSymbol = Serial.read();
  if (controlSymbol =! CONTROL_SYMBOL) {
    return false;
  }
  String timeStr;
  // read 8 characters "HH:MM:SS"
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 10; j++) {
      if (Serial.available()) {
        break;
      }
      delay(10);
    }
    char nextChar = Serial.read();
    timeStr += nextChar;
  }

  return parseTime(timeStr, outHours, outMins, outSecs);
}
