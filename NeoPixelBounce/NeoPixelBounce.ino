
/*
  This sketch makes neoPixels bounce from one end to the other of a strip

  created 28 March 2016
  by Tom Igoe
*/
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 6;  // control pin
const int numPixels = 8;    // number of pixels

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRB + NEO_KHZ800);

// count of keyframe colors:
int thisPixel = 0;
int lastPixel = numPixels-1;
int countDirection = 1;

void setup()  {
  strip.begin();  // initialize pixel strip
  strip.clear();  // clear pixels       
  strip.show();   // refresh the strip

  delay(1000);    // delay to see the LEDs reset before main loop
}

void loop() {
  // increment the current pixel number:
  thisPixel += countDirection; 

  // if the current pixel is at either end of the strip,
  // reverse the direction of counting:
  if (thisPixel < 0 || thisPixel >= numPixels) {
    countDirection = -countDirection;
  }
  // set the current and last pixels:
  strip.setPixelColor(thisPixel, 0x0000FF);
  strip.setPixelColor(lastPixel, 0x000000);
  lastPixel = thisPixel;

  // update the strip:
  strip.show();
  // delay 0.1 seconds:
  delay(100);
}
