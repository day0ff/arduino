#include <SoftwareSerial.h>
#include "FastLED.h"

SoftwareSerial HC05Module(10, 11); // TXD | RXD

#define NUM_LEDS 108
#define PIN 13

CRGB leds[NUM_LEDS];

void setLeds(String incomingString) {
  Serial.println(incomingString);

  int colorIndexStart = 0;
  int colorIndexEnd = 0;
  int hsvIndex = 0;
  int hsv[3];
  int ledIndex = 0;

  for (int i = 0; incomingString[i] != 0; i++) {
    if (incomingString[i] == ',') {
      hsv[hsvIndex] = incomingString.substring(colorIndexStart, colorIndexEnd + 1).toInt();
//      Serial.print(hsv[hsvIndex]);
      hsvIndex++;
      colorIndexStart = i + 1;
    } else if (incomingString[i] == ':') {
      hsv[hsvIndex] = incomingString.substring(colorIndexStart, colorIndexEnd + 1).toInt();
//      Serial.println(hsv[hsvIndex]);
      hsvIndex = 0;
      colorIndexStart = i + 1;
      if (ledIndex <= NUM_LEDS) {
        leds[ledIndex] = CHSV(hsv[0], hsv[1], hsv[2]);
      }
      ledIndex++;
    } else {
      colorIndexEnd = i;
    }
  };
  FastLED.show();
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
