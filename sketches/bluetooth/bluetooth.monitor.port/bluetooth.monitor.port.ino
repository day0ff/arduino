#include <SoftwareSerial.h>

SoftwareSerial HC05Module(10, 11); // TXD | RXD

void setup() {
  Serial.begin(9600);
  HC05Module.begin(9600);  //Default Baud for comm, it may be different for your Module.
  Serial.println("The bluetooth gates are open.");

}

void loop() {
  // Feed any data from bluetooth to Terminal.
  if (HC05Module.available())
    Serial.write(HC05Module.read());

  // Feed all data from termial to bluetooth
  if (Serial.available())
    HC05Module.write(Serial.read());

}