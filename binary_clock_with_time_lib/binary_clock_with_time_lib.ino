#include <Adafruit_NeoPixel.h>
#include <TimeLib.h>
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

Adafruit_NeoPixel seconds(NUMPIXELSSMALL, SECONDS_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel minutes(NUMPIXELSSMALL, MINUTES_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel hours(NUMPIXELSBIG, HOURS_PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

int seconds_clr[3] = {0, 255, 0};
int mins_clr[3] = {255, 0, 0};
int hours_clr[3] = {255, 20, 255};

void _light_one(int idx, Adafruit_NeoPixel *time_type, const int clr[3]);
void _light_zero(int idx, Adafruit_NeoPixel *time_type, const int clr[3]);
void writeBlock(int num, Adafruit_NeoPixel *time_type, int size_block, const int clr[3]);
void writeTime(int second_time, int minute_time, int hour_time);


void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  seconds.begin();
  minutes.begin();
  hours.begin();
  setTime(16,34,0,19,7,2021);
//    setTime(hr,min,sec,day,mnth,yr);
}

void loop() {
  writeTime(second(), minute(), hour());
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
  delay(1000);
  seconds.clear();
  minutes.clear();
  hours.clear();
}
