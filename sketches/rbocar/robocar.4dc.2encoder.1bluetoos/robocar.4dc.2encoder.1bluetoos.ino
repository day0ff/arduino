#include <SoftwareSerial.h>

char state[9] = "stop";

const int ENCODER_PIN1 = 2;
const int ENCODER_PIN2 = 3;
const boolean ENCODER_INTERRUPT1 = ENCODER_PIN1 % 2;
const boolean ENCODER_INTERRUPT2 = ENCODER_PIN2 % 2;
const unsigned int PULSE_PER_TURN = 20;
unsigned int pulses1;
unsigned int pulses2;
unsigned long oldTime;

const int EN = 6;
const int IN1 = 7;
const int IN2 = 10;
const int IN3 = 9;
const int IN4 = 8;



const unsigned int FORWARD = 24250;
const unsigned int BACKWARD = 31550;
const unsigned int STOP = 7760;
const unsigned int LEFT = 7550;
const unsigned int RIGHT = 12120;

SoftwareSerial HC05Module(11, 12); // TXD | RXD

void setup() {
  pinMode(ENCODER_PIN1, INPUT);
  pinMode(ENCODER_PIN2, INPUT);
  attachInterrupt(ENCODER_INTERRUPT1, pulseCounter1, FALLING);
  attachInterrupt(ENCODER_INTERRUPT2, pulseCounter2, FALLING);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(EN, OUTPUT);

  analogWrite(EN, 255);

  Serial.begin(9600);
  HC05Module.begin(9600);  //Default Baud for comm, it may be different for your Module.
  Serial.println("The bluetooth gates are open.");

}

void loop() {
  if (millis() - oldTime >= 2000) {
    detachInterrupt(0);
    if (pulses1 > 0) {
      char concatString1[80];
      sprintf(concatString1, "%s %s %i", state, "left", pulses1);
      Serial.println(concatString1);
      HC05Module.write(concatString1);
    }
    if (pulses2 > 0) {
      char concatString2[80];
      sprintf(concatString2, "%s %s %i", state, "right", pulses2);
      Serial.println(concatString2);
      HC05Module.write(concatString2);
    }
    oldTime = millis();
    pulses1 = 0;
    pulses2 = 0;
    attachInterrupt(ENCODER_INTERRUPT1, pulseCounter1, FALLING);
    attachInterrupt(ENCODER_INTERRUPT2, pulseCounter2, FALLING);
  }

  if (HC05Module.available() > 0) {
    switch (getStringInt() ) {
      case FORWARD:
        strcpy(state, "forward");
        drive(HIGH, LOW);
        break;
      case BACKWARD:
        strcpy(state, "backward");
        drive(LOW, HIGH);
        break;
      case STOP:
        // strcpy(state, "stop");
        drive(LOW, LOW);
        break;
      case LEFT:
        strcpy(state, "left");
        turn(HIGH, LOW);
        break;
      case RIGHT:
        strcpy(state, "right");
        turn(LOW, HIGH);
        break;
    }
  }

  if (Serial.available())
    HC05Module.write(Serial.read());

}


void drive(boolean forward, boolean backward) {
  digitalWrite(IN1, forward);
  digitalWrite(IN2, backward);
  digitalWrite(IN3, forward);
  digitalWrite(IN4, backward);
}

void turn(boolean left, boolean right) {
  digitalWrite(IN1, left);
  digitalWrite(IN2, right);
  digitalWrite(IN3, right);
  digitalWrite(IN4, left);
}

int getStringInt() {
  String incomingString = HC05Module.readString();

  Serial.println(incomingString);

  int stringInt = 0;

  for (int i = 0; incomingString[i] != 0; i++) {
    stringInt += int(incomingString[i]) * i * 10;
  };
  
  return stringInt;
}

void pulseCounter1() {
  pulses1++;
}

void pulseCounter2() {
  pulses2++;
}
