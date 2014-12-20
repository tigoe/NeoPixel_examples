/*
  BlinkyTree

  Makes a random pixel twinkle white, while the rest remain blue-green.
  Continually fades the previously twinkled pixel.
  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 4 Dec 2014
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>

#define PIN 13

const int numPixels = 50;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, PIN, NEO_RGB + NEO_KHZ800);

int red = 0;              // red value
int green = 34;           // green value
int blue = 12;            // blue value
int starColors[3];
int star;                 // random pixel number to twinkle
boolean fading = false;

void setup() {
  Serial.begin(9600);     // initialize serial communication
  Serial.setTimeout(10);  // set serial timeout
  strip.begin();          // initialize pixel strip
  
  // Initialize all pixels:
  for (int pixel = 0; pixel < numPixels; pixel++) {
    strip.setPixelColor(pixel, red, green, blue);// set the color for this pixel
    strip.show();
  }
}

void loop() {
// if you're currently fading a pixel
  if (!fading) {
    star = random(numPixels);    // pick a random pixel
    starColors[0] = 255;         // make it white
    starColors[1] = 255;
    starColors[2] = 255;
    fading = true;               // set fading to true
    
  } else {                       // if you are fading:
    if (starColors[0] > red) {   // dim the red one point
      starColors[0]--;    
    }
    if (starColors[1] > green) { // dim the green one point
      starColors[1]--;
    }
    if (starColors[2] > blue) {  // dim the blue one point
      starColors[2]--;
    }
    // when the twinkling pixel matches the base color, 
    // stop fading and wait a random amount of time before
    // twinkling another:
    if (starColors[0] == red &&
        starColors[1] == green &&
        starColors[2] == blue) {
      delay(random(3000) + 750);
      fading = false;
    }
    // update the twinkling pixel:
    strip.setPixelColor(star, starColors[0], starColors[1], starColors[2]);
    // refresh all pixels:
    strip.show();
  }

  delay(5);
}



