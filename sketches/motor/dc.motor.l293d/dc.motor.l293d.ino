const int in3 = 2;
const int in4 = 3;
const int en2 = 5;

const int FORWARD = 23240;
const int BACKWARD = 30410;
const int STOP = 7140;

void setup() {
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(en2, OUTPUT);

  analogWrite(en2, 255);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    switch (getStringInt() ) {
      case FORWARD:
        drive(HIGH, LOW);
        break;
      case BACKWARD:
        drive(LOW, HIGH);
        break;
      case STOP:
        drive(LOW, LOW);
        break;
    }
  }

}

void drive(boolean forward, boolean backward) {
  digitalWrite(in3, forward);
  digitalWrite(in4, backward);
  
}

int getStringInt() {
  String incomingString = Serial.readString();
  int stringInt = 0;

  for (int i = 0; incomingString[i] != 0; i++) {
    stringInt += int(incomingString[i]) * i * 10;
  };

  return stringInt;
}