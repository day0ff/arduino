int ENCODER_PIN = 2;
boolean ENCODER_INTERRUPT = ENCODER_PIN % 2;
unsigned int rpm;  // number of revolutions per minute
volatile byte pulses;
unsigned long oldTime;
unsigned int PULSE_PER_TURN = 20;

void setup(){
   Serial.begin(9600);
   pinMode(ENCODER_PIN, INPUT);
   attachInterrupt(ENCODER_INTERRUPT, pulseCounter, FALLING); // interrupt and run pulse сounter function
}

void loop(){
   if (millis() - oldTime >= 1000) {
      detachInterrupt(0);
      rpm = (60 * 1000 / PULSE_PER_TURN ) / (millis() - oldTime)* pulses;
      oldTime = millis();
      pulses = 0;
      Serial.print("RPM = ");
      Serial.println(rpm,DEC);
      attachInterrupt(ENCODER_INTERRUPT, pulseCounter, FALLING);
   }
}

void pulseCounter(){
   pulses++;
}
