#include <SoftwareSerial.h>

char incomingMessage;

SoftwareSerial HC05Module(10, 11); // TXD | RXD

const byte LED = 2;

void setup() {
  pinMode( LED, OUTPUT );

  Serial.begin(9600);
  HC05Module.begin(9600);  //Default Baud for comm, it may be different for your Module.
  Serial.println("The bluetooth gates are open.");
}

void loop() {
  // Feed any data from bluetooth to Terminal.
  if (HC05Module.available()) {
    incomingMessage = HC05Module.read();
    Serial.println(incomingMessage);

    switch (incomingMessage) {
      case 'b':
        Serial.println("BLINK");
        ledBlink();
        HC05Module.println("BLINK");
        break;
      case 'o':
        Serial.println("ON");
        ledOn();
        HC05Module.println("ON");
        break;
      case 'f':
        Serial.println("OFF");
        ledOff();
        HC05Module.println("OFF");
        break;
    }
  }
  if (Serial.available())
    HC05Module.write(Serial.read());

}

void ledBlink() {
  digitalWrite( LED, HIGH );
  delay( 1000 );
  digitalWrite( LED, LOW );
}


void ledOn() {
  digitalWrite( LED, HIGH );
}


void ledOff() {
  digitalWrite( LED, LOW );
}
