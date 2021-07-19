#define LED_PIN 13
#define CONTROL_SYMBOL 's'
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

struct Time {
  int hours;
  int mins;
  int secs;
};

class RTC {
public:
  void setTime(Time time);
  Time getTime();
};

class Control {
public:
  void setTime(Time time);
};

int curHours;
int curMins;
int curSecs;

void setup() {
  lcd.init();                      // initialize the lcd
  lcd.backlight();
  Serial.begin(9600);
}

// parse and validate time
boolean parseTime(String timeStr, int &outHours, int &outMins, int &outSecs) {
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
  String time;
  char controlSymbol = Serial.read();
  if (controlSymbol =! CONTROL_SYMBOL) {
    return false;
  }
  // read 8 characters "HH:MM:SS"
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 10; j++) {
      if (Serial.available()) {
        break;
      }
      delay(10);
    }
    char nextChar = Serial.read();
    time += nextChar;
  }

  return parseTime(time, outHours, outMins, outSecs);
}

void loop() {
  if (Serial.available()) {
    readTime(curHours, curMins, curSecs);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Current time is:");
    lcd.setCursor(5,1);
    lcd.print(String(curHours) + String(":") + String(curMins) + ":" + String(curSecs));  
  }
  
}
