String incomingString = String("");

void setup() {
    Serial.begin(9600);
}
 
 
void loop() {
    if (Serial.available() > 0) {
        incomingString = Serial.readString();
 
        Serial.print("I received: ");
        Serial.println(incomingString);
    }
}
