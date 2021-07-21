#include <FastLED.h>

#define arr_size(a) sizeof(a) / sizeof(a[0])

#define SECONDS_BLOCK_PIN 7
#define MINUTES_BLOCK_PIN 8
#define HOURS_BLOCK_PIN 9
#define NUM_LEDS    30
#define DIGIT 5
#define SECONDS_BLOCKS 6
#define CENTRE DIGIT/2

#define NUM_STRIPS 3
#define NUM_LEDS_PER_STRIP 30
CRGB leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];


CRGB SECONDS[NUM_LEDS];
CRGB MINUTES[NUM_LEDS];
CRGB HOURS[NUM_LEDS];
int seconds_clr[3] = {0, 255, 0};
int mins_clr[3] = {255, 0, 0};

void _light_one(int idx, CRGB *arr);
void _light_zero(int idx, CRGB *arr);
void writeBlock(int num, CRGB *arr, size_t arr_size);


void setup() {
  FastLED.clear();
  FastLED.addLeds<WS2812, SECONDS_BLOCK_PIN, GRB>(SECONDS, NUM_LEDS);
  FastLED.addLeds<WS2812, MINUTES_BLOCK_PIN, GRB>(MINUTES, NUM_LEDS);
  FastLED.addLeds<WS2812, HOURS_BLOCK_PIN, GRB>(HOURS, NUM_LEDS);
  FastLED.clear();
}


void loop() {
  for(int i = 1; i <= 60; i++){
    writeBlock(i, SECONDS, arr_size(SECONDS), seconds_clr);
    // writeBlock(i, MINUTES, arr_size(MINUTES), mins_clr);
    FastLED.show();
    delay(1000);
    FastLED.clear();
  }

}


/* 
 *  Function to light zero at certain position
 *  (5 leds are combined to create one position, which
 *  will be equal to 0 or 1)
 */
void _light_one(int idx, CRGB *arr, const int clr[3]){
   for (int i = idx * DIGIT; i < (idx + 1) * DIGIT; i++) {
    arr[i] = CRGB(clr[0], clr[1], clr[2]);
  }
} 


/* 
 *  Function to light zero (central led) at certain position
 *  (5 leds are combined to create one position, which
 *  will be equal to 0 or 1)
 */
void _light_zero(int idx, CRGB *arr, const int clr[3]){
   arr[idx * DIGIT+CENTRE] = CRGB(clr[0], clr[1], clr[2]);
}


/*
 * 
 * 
 * 
 */
void writeBlock(int num, CRGB *arr, size_t arr_size, const int clr[3]){
  int one_bit = 0;

  for (int i = 0; i < arr_size; i++){
    one_bit = bitRead(num, i);

    switch (one_bit){
      case 0:
        _light_zero(i, arr, clr);
        break;
      case 1:
        _light_one(i, arr, clr);
        break;
    }
  }
  
}
