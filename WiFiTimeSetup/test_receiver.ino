#define CONTROL_SYMBOL "s"

String data;

int curHours, curMins, curSecs;


bool parseTime(String timeStr, int &outHours, int &outMins, int &outSecs); 
bool readTime(int &outHours, int &outMins, int &outSecs);

void setup() {
  Serial.begin(9600);
}


void loop() {
  if (Serial.available()) {
    Serial.println("Main received: ");
    readTime(curHours, curMins, curSecs);
    Serial.print(curHours); Serial.print(" "); 
    Serial.print(curMins); Serial.print(" "); 
    Serial.print(curSecs); Serial.println(" "); 
  }
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
