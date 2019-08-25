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


String incomingMessage;

String PICTURE = "PICTURE";
String LINE = "LINE";
String SHOW = "SHOW";
String OFF = "OFF";

String state = OFF;

unsigned int lineIndex = 0;

void setLineIndex(String index) {
  lineIndex = index.toInt();
//  Serial.println(lineIndex);
  incomingMessage = "";
}

void setState(String incomingString) {
  state = incomingString;
  if (state.equalsIgnoreCase(SHOW)) {
    FastLED.show();
  }
  if (state.equalsIgnoreCase(OFF)) {
    FastLED.clear();
  }
  if (state.equalsIgnoreCase(PICTURE)) {
    HC05Module.write("PICTURE\r");
  }
//  if (state.equalsIgnoreCase(LINE)) {
//    HC05Module.write("LINE\r");
//  }
//  Serial.println(state);
  incomingMessage = "";
}

void setLine(String incomingString) {

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
        leds[ledIndex + lineIndex * NUM_ROWS] = CHSV(hsv[0], hsv[1], hsv[2]);
        colorful[ledIndex + lineIndex * NUM_ROWS] = hsv[3];
      }
      ledIndex++;
    } else {
      colorIndexEnd = i;
    }
  };
  //  Serial.println(incomingString);
  incomingMessage = "";
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
    char letter = HC05Module.read();

    switch (letter) {
      case '#':
        setState(incomingMessage);
        break;
      case '[':
        setLineIndex(incomingMessage);
        break;
      case ']':
        setLine(incomingMessage);
        break;
      case '\r':
      case '\n':
        incomingMessage = "";
        break;
      default:
        incomingMessage += letter;
    }

  }

  if (state.equalsIgnoreCase(SHOW)) {
    for (int i = 0; i < NUM_LEDS; i++ ) {
      if (colorful[i] == 1) {
        lhsv = rgb2hsv_approximate( leds[i]);
        leds[i] = CHSV(lhsv.h + 2, lhsv.s, lhsv.v);
      }
    }
    FastLED.show();
    delay(20);
  }
}
