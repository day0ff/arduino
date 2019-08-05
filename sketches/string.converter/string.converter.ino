void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    String incomingString = Serial.readString();
    unsigned int stringInt = 0;
    
    for (int i = 0; incomingString[i] != 0; i++) {
      stringInt += int(incomingString[i]) * i * 10;
    };
    
    Serial.print(incomingString);
    Serial.println(stringInt);
  }
}
