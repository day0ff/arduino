#include <SoftwareSerial.h>

char incomingString;

SoftwareSerial HC05Module(10, 11); // TXD | RXD

void setup() {
  Serial.begin(9600);
  HC05Module.begin(9600);  //Default Baud for comm, it may be different for your Module.
  Serial.println("The bluetooth gates are open.");

}

void loop() {
  // Feed any data from bluetooth to Terminal.
  if (HC05Module.available()){
    incomingString = HC05Module.read();
    Serial.print(incomingString);
    HC05Module.print(incomingString);
  }
  // Feed all data from termial to bluetooth
  if (Serial.available())
    HC05Module.write(Serial.read());

}
