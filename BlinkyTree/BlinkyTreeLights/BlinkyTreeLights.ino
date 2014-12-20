/*
  BlinkyTree

  This version maintains the candle flicker effect, and the slow fade to
  reddish orange over several hours, and re-introduces the twinkle to white
  effect.

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 14 Dec 2014
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>
#define PIN 13  // the I/O pin that the neopixel data signal is on

const int numPixels = 50;    // number of pixels in the strip
// initialize the strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, PIN, NEO_RGB + NEO_KHZ800);

unsigned long targetColor[numPixels];    // next target for each pixel
unsigned long pixelColor[numPixels];     // current color for each pixel

// range of keyframe colors for the pixels to flicker to:
unsigned long keyColors[] = {0xCB500F, 0xB4410C, 0x95230C, 0x853E0B};
// initial reference color range:
unsigned long referenceColors[] = {0xCB5D0F, 0xB4470C, 0x95310C, 0x854E0B};

// count of keyframe colors:
int numColors = sizeof(keyColors) / 4;
unsigned long fiveMinutes =  300000;   // five minutes, in millis
unsigned long lastFade = 0;

void setup() {
  Serial.begin(9600);     // initialize serial communication
  Serial.setTimeout(10);  // set serial timeout
  Serial.println("Starting");
  strip.begin();          // initialize pixel strip
  resetStrip();           // reset the strip
}

void loop() {
  // read serial input:
  while (Serial.available() > 0) {
    int input = Serial.read() {
      if (input == 'x' || input == 'X') {    // x signals that you should twinkle a pixel
        int thisPixel = random(numPixels);   // pick a random pixel
        pixelColor[thisPixel] = 0xFFFFFF;    // set its color to white
        change[thisPixel] = 1;               // set its change value to true
      }
      if (input == 'z') {                    // z signals you should reset the whole strip
        resetStrip();
      }
    }

    // create the flicker effect:
    if (millis() % 30 < 2) {
      flickerPixels();
    }

    // gradually fade the keyframe colors lower and more toward the red:
    if (millis() - lastFade >= fiveMinutes) {
      Serial.println("Fading");
      fadeToRed();
    }
  }

  /*
    this function creates the flicker effect:
  */
  void flickerPixels() {
    // iterate over all pixels:
    for (int thisPixel = 0; thisPixel < numPixels; thisPixel++) {
      // if the target color matches the current color for this pixel,
      // then pick a new target color randomly:
      while (targetColor[thisPixel] == pixelColor[thisPixel]) {
        int nextColor = random(numColors);
        targetColor[thisPixel] = keyColors[nextColor];
      }
      // fade the pixel one step from the current color to the target color:
      pixelColor[thisPixel] = compare(pixelColor[thisPixel], targetColor[thisPixel]);
      // set the pixel color in the strip:
      strip.setPixelColor(thisPixel, pixelColor[thisPixel]);// set the color for this pixel
    }

    // refresh the strip:
    strip.show();
  }

  /*
    This function fades all the key colors toward a low reddish orange
  */
  void fadeToRed() {
    // iterate over all pixels:
    for (int thisColor = 0; thisColor < numColors; thisColor++) {
      // separate the  color:
      byte r = keyColors[thisColor] >> 16;
      byte g = keyColors[thisColor] >>  8;
      byte b = keyColors[thisColor];

      // the reddish-orange glow you're aiming for is 0x1F0F04
      // fade the first color toward the second color:
      if (r > 0x1F) r--;
      if (g > 0x0F) g--;
      if (b > 0x04) b--;

      // combine the values to get the new color:
      keyColors[thisColor] = ((unsigned long)r << 16) | ((unsigned long)g << 8) | b;
      Serial.println(keyColors[thisColor], HEX);
    }
    lastFade = millis();
  }

  /*
    This function resets all the pixels to their base colors:
  */
  void resetStrip() {
    // iterate over the pixels:
    for (int pixel = 0; pixel < numPixels; pixel++) {
      // pick two random colors from the keyframe colors:
      int nextColor = random(numColors);
      int lastColor = random(numColors);
      targetColor[pixel] = keyColors[nextColor];
      pixelColor[pixel] = keyColors[lastColor];
      // set the pixel color:
      strip.setPixelColor(pixel, pixelColor[pixel]);// set the color for this pixel
    }
    // update the strip:
    strip.show();
  }

  /*
    takes two colors and returns a color that's a point on each axis (r, g, b)
    closer to the target color
  */
  unsigned long compare(unsigned long thisColor, unsigned long thatColor) {

    // separate the first color:
    byte r = thisColor >> 16;
    byte g = thisColor >>  8;
    byte b = thisColor;

    // separate the second color:
    byte targetR = thatColor >> 16;
    byte targetG = thatColor >>  8;
    byte targetB = thatColor;

    // fade the first color toward the second color:
    if (r > targetR) r--;
    if (g > targetG) g--;
    if (b > targetB) b--;

    if (r < targetR) r++;
    if (g < targetG) g++;
    if (b < targetB) b++;

    // combine the values to get the new color:
    unsigned long result = ((unsigned long)r << 16) | ((unsigned long)g << 8) | b;
    return result;
  }
