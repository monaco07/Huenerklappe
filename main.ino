int pinLDR = A1; // select the input pin for LDR
int sensorValue = 0; // variable to store the value coming from the sensor

int light_day = 600;
// Ab der Variable kann die klappe durch den ldr wert hoch
// mindeste für hochfahren morgen


int night_day = 300;
// Ab der Variable kann die klappe durch den ldr wert runter
// mindeste für runterfahren abend

bool isUnten = false;



bool check_hoch(){
  if(isUnten && sensorValue > light_day){
    return true;
  }
  else{
    return false;
  }
}

bool check_runter(){
  if(!(isUnten) && sensorValue < night_day){
    return true;
  }
  else{
    return false;
  }
}



void motor_down(){
  Serial.println("klappe hoch");
  isUnten = !isUnten;
}

void motor_up(){
  Serial.println("klappe runter");
  isUnten = !isUnten;
}

void setup() {
  Serial.begin(9600); //sets serial port for communication
  pinMode(A1, INPUT);

}

void loop() {
  sensorValue = analogRead(pinLDR); // read the value from the sensor
  Serial.println(sensorValue);

  bool can_hoch = check_hoch();
  bool can_runter = check_runter();

  switch (isUnten) {
    case false: {
      if (can_runter) {
        motor_up();
      }
      break;
    }
    case true: {
      if (can_hoch) {
        motor_down();
      }
      break;
    }
  }


  delay(2000);
}

