#include <FastLED.h>

#define LED_PIN     7
#define NUM_LEDS    30
#define DIGIT 5
#define SECONDS_BLOCKS 6
#define CENTRE DIGIT/2



CRGB leds[NUM_LEDS];
void light_one(int, int clr[3]);
void write_second(int num, int clr[3]);


void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  
}


void loop() {
  int clr[3] = {0, 255, 0};
//  for (int i = 0; i < SECONDS_BLOCKS; i+=2) {
//    light_one(i, clr);
//    light_zero(i+1, clr);
//  }
  for(int i = 1; i <= 60; i++){
    write_second(i, clr);
    delay(1000);
    FastLED.clear();
  }

}


void light_one(int idx, int clr[3]){
   for (int i = idx * DIGIT; i < (idx + 1) * DIGIT; i++) {
    // leds[i] = CRGB(clr->red, clr->green, clr->blue);
    leds[i] = CRGB(clr[0], clr[1], clr[2]);
  }
   FastLED.show();
} 

void light_zero(int idx, int clr[3]){
   leds[idx * DIGIT+CENTRE] = CRGB(clr[0], clr[1], clr[2]);
   FastLED.show();
}

void write_second(int num, int clr[3]){
  int one_bit;
  for (int i = 0; i < SECONDS_BLOCKS; i++){
    one_bit = bitRead(num, i);
    switch (one_bit){
      case 0:
        light_zero(i, clr);
        break;
      case 1:
        light_one(i, clr);
        break;
    }
}
  
}
