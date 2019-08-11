#include <SoftwareSerial.h>
#include "FastLED.h"

SoftwareSerial HC05Module(10, 11); // TXD | RXD

#define NUM_LEDS 108
#define PIN 13

const unsigned int NUM_LINES = 9;
const unsigned int NUM_ROWS = 12;

unsigned int colorful[NUM_LEDS];
CRGB leds[NUM_LEDS];
CHSV lhsv( 0, 0, 0 );

const unsigned int OFF = 2100;
const unsigned int ON = 780;
const unsigned int PICTURE = 16230;

const unsigned int LINE = 4360;
const unsigned int SHOW = 4910;

unsigned int state = OFF;

unsigned int code(String codeString) {
  Serial.println(codeString);

  unsigned int code = 0;

  for (int i = 0; codeString[i] != 0; i++) {
    code += int(codeString[i]) * i * 10;
  };
  Serial.println(code);
  return code;
}

void setLine(unsigned int line, String incomingString) {
  int colorIndexStart = 0;
  int colorIndexEnd = 0;
  int hsvIndex = 0;
  int hsv[4];
  int ledIndex = 0;

  for (int i = 0; incomingString[i] != 0; i++) {
    if (incomingString[i] == ',') {
      hsv[hsvIndex] = incomingString.substring(colorIndexStart, colorIndexEnd + 1).toInt();
      hsvIndex++;
      colorIndexStart = i + 1;
    } else if (incomingString[i] == ':') {
      hsv[hsvIndex] = incomingString.substring(colorIndexStart, colorIndexEnd + 1).toInt();
      hsvIndex = 0;
      colorIndexStart = i + 1;
      if (ledIndex <= NUM_LEDS) {
        leds[ledIndex + line * NUM_ROWS] = CHSV(hsv[0], hsv[1], hsv[2]);
        colorful[ledIndex + line * NUM_ROWS] = hsv[3];
      }
      ledIndex++;
    } else {
      colorIndexEnd = i;
    }
  };
  HC05Module.write("LINE\r");
  state = OFF;
}


void readIncomingString(String incomingString) {
  Serial.println(incomingString);
  state = code(incomingString.substring(0, incomingString.indexOf('#')));
  switch (state) {
    case LINE:
      setLine(
        incomingString.substring(incomingString.indexOf('#') + 1, incomingString.indexOf('[')).toInt(),
        incomingString.substring(incomingString.indexOf('[') + 1, incomingString.indexOf(']'))
      );
      break;
    case SHOW:
      FastLED.show();
      break;
  }
}

void setup() {
  FastLED.addLeds<WS2812, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(50);
  pinMode(PIN, OUTPUT);

  for (int i = 0; i < NUM_LEDS; i++ ) {
    leds[i] = CHSV(0, 0, 0);
    colorful[i] = 0;
  }

  Serial.begin(9600);
  HC05Module.begin(9600);  //Default Baud for comm, it may be different for your Module.
  Serial.println("The bluetooth gates are open.");
}

void loop() {
  if (HC05Module.available() > 0) {
    readIncomingString(HC05Module.readString());
  }

  if (state == SHOW) {
    for (int i = 0; i < NUM_LEDS; i++ ) {
      if (colorful[i] == 1) {
        lhsv = rgb2hsv_approximate( leds[i]);
        leds[i] = CHSV(lhsv.h + 2, lhsv.s, lhsv.v);
      }
    }
    FastLED.show();
    delay(10);
  }

}
