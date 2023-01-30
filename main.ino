#include <Wire.h>
#include <RTClib.h>

int loop_delay = 10000; // 10s zeit, die die loop pausiert
int light_day = 600; // LDR-Wert ab dem die Klappe hochfahren kann

bool isUnten = false; // ist klappe unten ?
int sensorValue = 0; // Variable zur Speicherung des LDR-Werts

bool check_hoch() {
  if (isUnten && sensorValue > light_day) {
    return true;
  } 
  else {
    return false;
  }
}



void motor_down() {
  Serial.println("klappe runter");
  isUnten = true;
}

void motor_up() {
  Serial.println("klappe hoch");
  isUnten = false;
}

RTC_DS3231 rtc;




void setup() {
  Serial.begin(9600);
  pinMode(A1, INPUT);
  Wire.begin();
  rtc.begin();
}

int dunkel_time(int month){
  if(month == 1 || month == 11 || month == 12){
    // Januar
    return 18;
  }
  if(month == 2 || month == 3){
    return 19;
  }
  if(month == 4){ // zeitumstellung wird nicht von der rtc unterstützt, drum egal
    return 20;  // +1
  }
  if(month >= 5 && month <9){
    return 21; // +1
  }
  if(month == 9){
    return 20; // +1
  }
  if(month == 10){
    return 19; // +1
  }
}
void loop() {

  sensorValue = analogRead(A1); // ließt ldr value
  Serial.println(sensorValue);

  // Prüfe ob Klappe hochfahren kann
  bool can_hoch = check_hoch();

  // zeit von rtc
  DateTime now = rtc.now();
  Serial.println(dunkel_time(now.month()));
  // klappe fährt um 7 hoch, wenn sie kann
  if (now.hour() == 7) {
    if (can_hoch) {
      motor_up();
    }
  }
  // klappe fährt um 8 spätestens hoch
  else if (now.hour() == 8 && isUnten) {
    motor_up();
  }
  // klappe fährt zur gegebenen uhrzeit hoch, vom monat abhängig
  else if (now.hour() >= dunkel_time(now.month())) {
    if(!isUnten){
      motor_down();
    }
  }

  // wartet 10s
  delay(loop_delay);
}
