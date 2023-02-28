// Erstmal in Arduino IDE einfügen

// Dann müssen sie die Bibliotheken installieren:
// Tools => Manage Libraries
#include <Wire.h> // ist schon automatisch drauf
#include <RTClib.h> // RTClib von Adafruit
#include <Stepper.h> // Stepper von Arduino

// Einstellbare Werte zum Ausprobieren:
// alles in ms
int dauer_motor = 5000; // wie lanmge soll der motor sich drehen
int motorSpeed = 30; // Geschwindigkeit des Motors, nicht Übertreiben, sonst kann der motor kaputt gehen
int loop_delay = 5000; // zeit die die loop pausiert bei jedem durchgang, nur bei jedem durchgang wird geschaut ob der manuelle knopf gedrückt wird
int light_day = 600; // LDR-Wert ab dem die Klappe hochfahren kann
int warten_nach_motor = 120000; // hier wartet das skript 2min nach der motorbewegung


//  code:

bool isUnten = false; // ist klappe unten ?
int sensorValue = 0; // Variable zur Speicherung des LDR-Werts
const int stepsPerRevolution = 256;
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

bool check_hoch() {
  if (isUnten && sensorValue > light_day) {
    return true;
  } 
  else {
    return false;
  }
}


void fahreMotor(){
  
  if(isUnten){
    isUnten = false;
    unsigned long startTime = millis();
    while (millis() - startTime < dauer_motor) {
    myStepper.step(-stepsPerRevolution);
    }
  }
  else {
    isUnten = true;
    unsigned long startTime = millis();
    while (millis() - startTime < dauer_motor) {
    myStepper.step(stepsPerRevolution);
  }
  }
  delay(warten_nach_motor); // wenn die klappe bewegt wurde, werden 2 mins gewartet
}



RTC_DS3231 rtc;

void setup() {
  Serial.begin(9600);
  myStepper.setSpeed(motorSpeed);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT); 
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(A1, INPUT);
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // korrigiert die uhr wenns geht
  pinMode(2, INPUT);
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

  if (digitalRead(2) == 1) {
    Serial.println("fahren");
    fahreMotor();
    
  }
  Serial.println(digitalRead(2));




  sensorValue = analogRead(A1); // ließt ldr value
  Serial.println(sensorValue);

  // Prüfe ob Klappe hochfahren kann
  bool can_hoch = check_hoch();

  // zeit von rtc
  DateTime now = rtc.now();
  Serial.println(now.day());
  Serial.println(now.hour());
  Serial.println(now.month());
  Serial.println(now.year());
  Serial.println(dunkel_time(now.month()));
  // klappe fährt ab 7 hoch, wenn die helligkeit passt
  if (now.hour() == 7) {
    if (can_hoch) {
      fahreMotor();
    }
  }
  // klappe fährt um 8 spätestens hoch, egal wie der ldr wert ist
  else if (now.hour() == 8 && isUnten) {
    fahreMotor();
  }
  // klappe fährt zur gegebenen uhrzeit runter, vom monat abhängig
  else if (now.hour() >= dunkel_time(now.month()) || now.hour() < 7) {
    
    if(!isUnten){
      fahreMotor();
    }
  }

  // wartet 
  delay(loop_delay);
}
// Copyright Leonard Storkenmaier
// Alles erlaubt
// Keine Haftung
