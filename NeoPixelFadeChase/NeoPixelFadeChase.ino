
/*
  This sketch makes neoPixels chase, with a fading trail.
  The trail changes directions with the chasing pixel.
  
  created 28 March 2016
  by Tom Igoe
*/
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 6;  // control pin
const int numPixels = 8;    // number of pixels

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRB + NEO_KHZ800);

unsigned long trailColor[numPixels];  // array of colors for the trail
int currentPixel = 0;                 // the current pixel
int trailingPixel = numPixels;        // the final trailing pixel
int trailDirection = 1;               // direction of the trail

void setup()  {
  // set the trail colors:
  trailColor[0] = 0x0000FF;
  trailColor[1] = 0x000099;
  trailColor[2] = 0x000055;
  trailColor[3] = 0x000022;
  // fill the rest with zeroes:
  for (int p = 4; p < numPixels; p++) {
    trailColor[p] = 0;
  }

  strip.begin();  // initialize pixel strip
  strip.clear();  // turn off pixels
  strip.show();   // refresh the strip
}

void loop() {
  // increment the current pixel number:
  currentPixel += trailDirection;

  // if the current pixel is at either end of the strip,
  // reverse the direction of counting:
  if (currentPixel < 0 || currentPixel >= numPixels) {
    trailDirection = -trailDirection;
  }

  // set the current pixel:
  strip.setPixelColor(currentPixel, trailColor[currentPixel]);

  // copy the current pixel number for setting the trail:
  trailingPixel = currentPixel;

  // while the trailing pixel number is still in range:
  while (trailingPixel < numPixels && trailingPixel >= 0) {
    // color number is the distance from current pixel in either direction:
    int colorNum = abs(currentPixel - trailingPixel);
    // set the color:
    strip.setPixelColor(trailingPixel, trailColor[colorNum]);
    // move to the next trailing pixel:
    trailingPixel -= trailDirection;
  }

  // update the strip:
  strip.show();
  // delay 0.1 seconds:
  delay(100);
}

