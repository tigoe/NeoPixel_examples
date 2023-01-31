
/*
  This sketch makes neoPixels chase, with a fading trail.
  The trail changes directions with the chasing pixel.

  created 28 March 2016
  modified 31 Jan 2023
  by Tom Igoe
*/
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 5;  // control pin
const int pixelCount = 144;    // number of pixels
const int trailLength = pixelCount/10;  // length of the fading trail

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);

unsigned long trailColor[pixelCount];  // array of colors for the trail
int currentPixel = 0;                 // the current pixel
int trailingPixel = pixelCount;        // the final trailing pixel
int trailDirection = 1;               // direction of the trail

void setup()  {
  // set the trail colors:
  Serial.begin(9600);
  if (!Serial) delay(3000);
  for (int p = 0; p < trailLength; p++) {

    trailColor[p] = (p * 255) / 8 ;
    Serial.println(trailColor[p]);
  }
  Serial.println(" " );
  for (int p = 9; p < 12; p++) {
    trailColor[p] = 255 / (p -8);
    Serial.println(trailColor[p]);
  }
  Serial.println(" " );
  // fill the rest with zeroes:
  for (int p = 12; p < pixelCount; p++) {
    trailColor[p] = 0;
  //  Serial.println(trailColor[p]);
  }

  //while (true);
  strip.begin();  // initialize pixel strip
  strip.clear();  // turn off pixels
  strip.show();   // refresh the strip
}

void loop() {
  // increment the current pixel number:
  currentPixel += trailDirection;

  // if the current pixel is at either end of the strip,
  // reverse the direction of counting:
  if (currentPixel < 0 || currentPixel >= pixelCount) {
    trailDirection = -trailDirection;
  }

  // set the current pixel:
  strip.setPixelColor(currentPixel, trailColor[currentPixel]);

  // copy the current pixel number for setting the trail:
  trailingPixel = currentPixel;

  // while the trailing pixel number is still in range:
  while (trailingPixel < pixelCount && trailingPixel >= 0) {
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
