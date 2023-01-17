void setup() {
  // import time
  // IMPORT MOTOREN PORTS
  pinMode(A1, INPUT);
  Serial.begin(9600);
}

void loop() {
  delay(2000);
  int ldr_value = analogRead(A1);
  Serial.println(ldr_value);
  bool isUnten = false;
  int delay = 1000 * 60 * 5; // alle 5 mins
  bool can_runter = false;
  bool can_hoch = true;

  
 switch (isUnten) {
  case false:
    if (can_runter) {
      motor_down();
    }
    break;
  case true:
    if (can_hoch) {
      motor_up();
    }
    break;
}
  
}

void motor_down(){
  
}
void motor_up(){
  
}
