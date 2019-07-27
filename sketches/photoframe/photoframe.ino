#include <SoftwareSerial.h>
#include "FastLED.h"

SoftwareSerial HC05Module(10, 11); // TXD | RXD

#define NUM_LEDS 108
#define PIN 13

CRGB leds[NUM_LEDS];
byte counter;

const unsigned int TERNON = 13210;
const unsigned int TERNOFF = 17240;
const unsigned int RAINBOW = 17970;
const unsigned int STOP = 5840;

unsigned int state = RAINBOW;

void rainbow() {
  for (int i = 0; i < NUM_LEDS; i++ ) {
    leds[i] = CHSV(counter + i * 2, 255, 255);
  }
  counter++;
  FastLED.show();
  delay(5);
}

void ternoff() {
  for (int i = 0; i < NUM_LEDS; i++ ) {
    leds[i] = CHSV(0, 0, 0);
  }
  FastLED.show();
}

int getStringInt() {
  String incomingString = HC05Module.readString();

  Serial.println(incomingString);

  int stringInt = 0;

  for (int i = 0; incomingString[i] != 0; i++) {
    stringInt += int(incomingString[i]) * i * 10;
  };

  Serial.println(stringInt);
  return stringInt;
}

void executeCommand(int command) {
  switch ( command ) {
    case RAINBOW:
      state = RAINBOW;
      rainbow();
      break;
    case TERNOFF:
      if (state != TERNOFF) {
        ternoff();
        state = TERNOFF;
      }
      break;
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
  executeCommand(state);

  if (HC05Module.available() > 0)
    executeCommand(getStringInt());

  if (Serial.available())
    HC05Module.write(Serial.read());

}
