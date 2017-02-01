/*
  RGBW NeoPixel HSI-to-RGB conversion and fade

  This sketch fades the hue and saturation of a neoPixel device

  It converts HSI values to a single RGB color and sets a neoPixel string

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  Based on HSI conversion from Saiko LED, by Brian Neitner:
  http://blog.saikoled.com/post/44677718712/how-to-convert-from-hsi-to-rgb-white
  http://blog.saikoled.com/post/43693602826/why-every-led-light-should-be-using-hsi

  created 31 Jan 2017
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 5;  // control pin
const int numPixels = 7;    // number of pixels

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRBW + NEO_KHZ800);

int h = 0;
int s = 40;
int i = 50;        // hue, saturation,intensity

void setup() {
  strip.begin();        // initialize pixel strip
  strip.clear();        // turn all LEDs off
  strip.show();         // update strip
  Serial.begin(9600);
}

void loop() {
  // loop over all the pixels:
  for (int pixel = 0; pixel < numPixels; pixel++) {
    h = h + random(6) - 3;    // h = +/-3
    h = constrain(h, 0, 45);  // constrain to 0-45
    s = s + random(6) - 3;    // s = +/-3
    s = constrain(s, 25, 80);  // constrain to 35-80
    // convert the values to RGBW:
    unsigned long color = hsiToRgb(h, s, i);

    strip.setPixelColor(pixel, color);    // set the color for this pixel
  }
  strip.show();   // update the strip
  delay(10);
}


unsigned long hsiToRgbw(float hue, float saturation, float intensity) {
  unsigned long result = 0;                  // the RGBW result you'll return
  unsigned long r, g, b, w;                            // red, green, blue, white
  hue = constrain(hue, 0, 360);              // constrain hue to 0-360
  hue = hue * PI / 180;                      // Convert to radians.
  saturation = constrain(saturation / 100, 0, 1); // constrain to 0-1
  intensity = constrain(intensity / 100, 0, 1);  // constrain to 0-1

  // if hue is in the red/green sector:
  if (hue < 2 * PI / 3) {
    r = saturation * 255 * intensity / 3 * (1 + cos(hue) / cos((PI / 3) - hue));
    g = saturation * 255 * intensity / 3 * (1 + (1 - cos(hue) / cos((PI / 3) - hue)));
    b = 0;
    // if hue is in the green/blue sector:
  } else if (hue < 4 * PI / 3) {
    hue = hue - (2 * PI / 3);
    g = saturation * 255 * intensity / 3 * (1 + cos(hue) / cos((PI / 3) - hue));
    b = saturation * 255 * intensity / 3 * (1 + (1 - cos(hue) / cos((PI / 3) - hue)));
    r = 0;
    // if hue is in the red/blue sector:
  } else {
    hue = hue - (4 * PI / 3);
    b = saturation * 255 * intensity / 3 * (1 + cos(hue) / cos((PI / 3) - hue));
    r = saturation * 255 * intensity / 3 * (1 + (1 - cos(hue) / cos((PI / 3) - hue)));
    g = 0;
  }
  // white is a function of saturation and intensity regardless of hue:
  w = 255 * (1 - saturation) * intensity;

  // assemble the results into a single 4-byte number:
  result = (w << 24) | (r << 16) | (g << 8) | b;
  // return result:
  return result;
}


unsigned long hsiToRgb(float hue, float saturation, float intensity) {
  unsigned long result = 0;                  // the RGB result you'll return
  unsigned long r, g, b;                               // red, green, blue
  hue = constrain(hue, 0, 360);              // constrain hue to 0-360
  hue = hue * PI / 180;                      // Convert to radians.
  saturation = constrain(saturation / 100, 0, 1); // constrain to 0-1
  intensity = constrain(intensity / 100, 0, 1);  // constrain to 0-1


  // if hue is in the red/green sector:
  if (hue < 2 * PI / 3) {
    r = 255 * intensity / 3 * (1 + saturation * cos(hue) / cos((PI / 3) - hue));
    g = 255 * intensity / 3 * (1 + saturation * (1 - cos(hue) / cos((PI / 3) - hue)));
    b = 255 * intensity / 3 * (1 - saturation);
    // if hue is in the green/blue sector:
  } else if (hue < 4 * PI / 3) {
    hue = hue - (2 * PI / 3);
    g = 255 * intensity / 3 * (1 + saturation * cos(hue) / cos((PI / 3) - hue));
    b = 255 * intensity / 3 * (1 + saturation * (1 - cos(hue) / cos((PI / 3) - hue)));
    r = 255 * intensity / 3 * (1 - saturation);
    // if hue is in the red/blue sector:
  } else {
    hue = hue - (4 * PI / 3);
    b = 255 * intensity / 3 * (1 + saturation * cos(hue) / cos((PI / 3) - hue));
    r = 255 * intensity / 3 * (1 + saturation * (1 - cos(hue) / cos((PI / 3) - hue)));
    g = 255 * intensity / 3 * (1 - saturation);
  }


  // assemble the results into a single 3-byte number:
  result =  (r << 16) | (g << 8) | b;
  // return result:
  return result;
}

