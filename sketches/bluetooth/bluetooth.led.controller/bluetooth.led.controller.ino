#include <SoftwareSerial.h>

char incomingMessage;

SoftwareSerial HC05Module(10, 11); // TXD | RXD

const byte RED_LED = 2;
const byte YELLOW_LED = 3;
const byte GREEN_LED = 4;
const byte BLUE_LED = 5;

void setup() {
  pinMode( RED_LED, OUTPUT );
  pinMode( YELLOW_LED, OUTPUT );
  pinMode( GREEN_LED, OUTPUT );
  pinMode( BLUE_LED, OUTPUT );
      
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
      case 'r':
        Serial.println("RED");
        ledSwitch(RED_LED);
        HC05Module.println("RED");
        break;
     case 'y':
        Serial.println("YELLOW");
        ledSwitch(YELLOW_LED);
        HC05Module.println("YELLOW");
        break;
     case 'g':
        Serial.println("GREEN");
        ledSwitch(GREEN_LED);
        HC05Module.println("GREEN");
        break;
     case 'b':
        Serial.println("BLUE");
        ledSwitch(BLUE_LED);
        HC05Module.println("BLUE");
        break;
    }
  }
  if (Serial.available())
    HC05Module.write(Serial.read());

}

void ledSwitch(byte led){
  if(!digitalRead(led))digitalWrite( led, HIGH );
  else digitalWrite( led, LOW );
}
