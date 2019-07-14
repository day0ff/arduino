#define NUM_LEDS 12
#include "FastLED.h"
#define PIN 13
CRGB leds[NUM_LEDS];
byte counter;
void setup() {
  FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(50);
  pinMode(13, OUTPUT);
}
void loop() {
  for (int i = 0; i < NUM_LEDS; i++ ) {
    leds[i] = CHSV(counter + i * 2, 255, 255);
  }
  counter++;
  FastLED.show();
  delay(5);
}