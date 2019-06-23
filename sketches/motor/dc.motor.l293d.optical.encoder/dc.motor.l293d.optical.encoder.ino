const int ENCODER_PIN = 2;
const boolean ENCODER_INTERRUPT = ENCODER_PIN % 2;
const unsigned int PULSE_PER_TURN = 20;

unsigned int rpm;  // number of revolutions per minute
volatile byte pulses;
unsigned long oldTime;

const int in3 = 6;
const int in4 = 7;
const int en2 = 8;

const int FORWARD = 23240;
const int BACKWARD = 30410;
const int STOP = 7140;

void setup() {
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(en2, OUTPUT);

  analogWrite(en2, 255);

  Serial.begin(9600);

  pinMode(ENCODER_PIN, INPUT);

  attachInterrupt(ENCODER_INTERRUPT, pulseCounter, FALLING); // interrupt and run pulse сounter function
}

void loop() {
  if (millis() - oldTime >= 1000) {
    detachInterrupt(0);
    rpm = (60 * 1000 / PULSE_PER_TURN ) / (millis() - oldTime) * pulses;
    oldTime = millis();
    pulses = 0;
    Serial.print("RPM = ");
    Serial.println(rpm, DEC);
    attachInterrupt(ENCODER_INTERRUPT, pulseCounter, FALLING);
  }

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

void pulseCounter() {
  pulses++;
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
