#include <SoftwareSerial.h>

// Disconnect the +5v line from the modules end as shown in the hook diagram above.
// Press and hold the switch (DO NOT RELEASE IT!).
// Reconnect the +5v connection.
// In port monitor choose NL & CR option.
// Use 9600 port on Serial Monitor.
// For Arduino Nano 3.0 use Tools->Processor->Atmega 328P(Old Bootloader)

SoftwareSerial HC05Module(10, 11); // TXD | RXD

void setup() {
  Serial.begin(9600);
  HC05Module.begin(38400);  //Default Baud for comm, it may be different for your Module.
  Serial.println("Enter AT commands!.");

}

void loop() {
  // Feed any data from bluetooth to Terminal.
  if (HC05Module.available())
    Serial.write(HC05Module.read());

  // Feed all data from termial to bluetooth
  if (Serial.available())
    HC05Module.write(Serial.read());

}
