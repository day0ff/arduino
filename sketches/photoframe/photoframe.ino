#include <SoftwareSerial.h>
#include "FastLED.h"

SoftwareSerial HC05Module(10, 11); // TXD | RXD

#define NUM_LEDS 108
#define PIN 13

CRGB leds[NUM_LEDS];

void setLeds(String incomingString) {
  Serial.println(incomingString);

  int strLength = incomingString.length() + 1;
  char str[strLength] ;
  incomingString.toCharArray(str, strLength);

  char * command;
  command = strtok(str, "#");
  int index = -1;
  while (command != NULL) {
    Serial.println(command);
    if (index != -1) {
      char * hsv = strchr(command, ',');
      Serial.println(hsv[0]);
      Serial.println(hsv[1]);
      Serial.println(hsv[2]);
    }
    command = strtok(NULL, ":");
    index++;
  }

}

void setup() {
  FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(50);
  pinMode(PIN, OUTPUT);

  Serial.begin(9600);
  HC05Module.begin(9600);  //Default Baud for comm, it may be different for your Module.
  Serial.println("The bluetooth gates are open.");
}

void loop() {

  if (HC05Module.available() > 0)
    setLeds(HC05Module.readString());


  if (Serial.available())
    HC05Module.write(Serial.read());
}
