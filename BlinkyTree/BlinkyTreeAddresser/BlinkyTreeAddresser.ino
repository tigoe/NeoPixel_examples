/*
  BlinkyTree Addresser

  This sketch turns on a string of NeoPixels one at a time in response
  to serial commands as follows:
  c0 - turn off current pixel
  c1 - turn on current pixel
  space - increment current pixel number
  p - print list of pixels that are currently turned on
  
  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 16 Dec 2014
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>

// the I/O pin that the neopixel data signal is on:
#define PIN 12

const int numPixels = 50;  // number of pixels in the strip
// initialize the strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, PIN, NEO_RGB + NEO_KHZ800);

int pixelStates[numPixels];
int currentPixel = 0;
int pinState = -1;

void setup() {
  Serial.begin(9600);     // initialize serial communication
  Serial.setTimeout(10);  // set serial timeout
  Serial.println("Starting");
  strip.begin();          // initialize pixel strip
  resetStrip();           // reset the strip
}

void loop() {


  while (Serial.available() > 0) {
    char input = Serial.read();
    Serial.print(input);
    // serial triggers:
    if (input == 'c') {    // string should start with C
      pinState = Serial.parseInt();
      Serial.print(currentPixel);
      Serial.print(" ");
      Serial.println(pinState);
    }
    if (input == 'z') {
      resetStrip();
    }
    if (input == 'p') {
      printArray();
    }
    if (input == ' ') {
      if (currentPixel < numPixels) {
        currentPixel++;
      } else {
        currentPixel = 0;
      }
      Serial.print("next pixel: ");
      Serial.println(currentPixel);
    }
  }


  if (pinState > -1) {
    int pixelColor = pinState * 0x0000FF;
    strip.setPixelColor(currentPixel, pixelColor);
    pixelStates[currentPixel] = pinState;

    pinState = -1;
  }
  strip.show();
}


void resetStrip() {
  // iterate over all pixels:
  for (int thisPixel = 0; thisPixel < numPixels; thisPixel++) {

    // set the pixel color in the strip:
    strip.setPixelColor(thisPixel, 0);// set the color for this pixel
  }

  // refresh the strip:
  strip.show();
}

void printArray() {
  for (int thisPixel = 0; thisPixel < numPixels; thisPixel++) {
    if (pixelStates[thisPixel] > 0) {
      Serial.print(thisPixel);
      Serial.print(",");
    }
  }
  Serial.println();
}
