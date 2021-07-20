#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include<SoftwareSerial.h>

//Started SoftwareSerial at RX and TX pin of ESP8266/NodeMCU
SoftwareSerial SUART(3,1);

#define D2 4

const char *ssid     = "UCU_Guest";
const char *password = "";

const long utcOffsetInSeconds = 10800; // 3 * 60 * 60
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

  
volatile bool state = false;
uint8_t GPIO_Pin = D2;

void ICACHE_RAM_ATTR IntCallback(); 

void setup() {
 Serial.begin(9600);
 SUART.begin(9600);
 attachInterrupt(digitalPinToInterrupt(GPIO_Pin), IntCallback, RISING);
}


void loop() {
  if (state) {
    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", utcOffsetInSeconds);

    WiFi.begin(ssid, password);
    while ( WiFi.status() != WL_CONNECTED ) {
      delay(500);
      Serial.println("...");
    }
    Serial.println();

    timeClient.begin();
    timeClient.update();

    Serial.print(daysOfTheWeek[timeClient.getDay()]);
    Serial.print("s");
    Serial.print(timeClient.getHours());
    Serial.print(":");
    Serial.print(timeClient.getMinutes());
    Serial.print(":");
    Serial.print(timeClient.  getSeconds());
    Serial.print("\n");
    Serial.println(timeClient.getFormattedTime());

    // this data will be send to the arduino via Serial port
    SUART.print("s");
    SUART.print(timeClient.getHours());
    SUART.print(":");
    SUART.print(timeClient.getMinutes());
    SUART.print(":");
    SUART.println(timeClient.getSeconds());

    // TODO: there might be a need to do some additional cleaning
    // Cleaning up
    WiFi.disconnect();
    state = !state;
    }
}

void ICACHE_RAM_ATTR IntCallback(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200)
  {
    state = !state;
  }
  last_interrupt_time = interrupt_time;
}
