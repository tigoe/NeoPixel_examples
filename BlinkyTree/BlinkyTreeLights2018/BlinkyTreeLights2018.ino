/*
  BlinkyTree 2018
  This version maintains:
    mailbox support for web interface
    mailbox support for hue, saturation, intensity, 
    so you can change the whole tree to one color. This year's 
    HTML page has sliders for that.

  This year is an odd mix of HSI to RGB and plain RGB colors.
  I'm not happy with the code, but the colors are nice.
  The colors are turquoise, salmon, and lavender

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 15 Dec 2018
  by Tom Igoe
*/

#include <Adafruit_NeoPixel.h>
#include <ColorConverter.h>
#include <Bridge.h>
#include <Mailbox.h>
#include <Console.h>

const int controlPin = 3;  // the I/O pin that the neopixel data signal is on
const int pixelCount = 100;    // number of pixels in the strip

// initialize the strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, controlPin,
                          NEO_RGB + NEO_KHZ800);
// initalize a color converter:
ColorConverter converter;

// make an array of RGB colors:
RGBColor colors[] = {converter.HSItoRGB(7, 95, 100),
                     converter.HSItoRGB(168, 100, 100),
                     converter.HSItoRGB(285, 22, 40)
                    };

// bake an array of base colors (unsigned longs):
unsigned long baseColor[pixelCount];  // base colors of the strip
int colorCount = 3;                   // count of colors on the tree
boolean running = true;               // whether or not the tree is running

void setup() {
  Bridge.begin();         // initialize Bridge
  Mailbox.begin();        // initialize Mailbox
  Console.begin();        // initialize Console
  strip.begin();          // initialize pixel strip
  strip.clear();          // clear the strip
  resetLights();          // reset the lights
}

void loop() {
  int h = 10;         // hue
  int s = 100;        // saturation
  int i = 100;        // intensity

  String message;
  // read the next message present in the queue
  while (Mailbox.messageAvailable()) {
    Mailbox.readMessage(message);

    if (message.startsWith("h/")) {
      String hue = message.substring(2); // get the substring after the /
      h = hue.toFloat();             // convert to a number
      changeString(h, s, i);
    }

    if (message.startsWith("s/")) {
      String sat = message.substring(2); // get the substring after the /
      s = sat.toFloat();             // convert to a number
      changeString(h, s, i);
    }

    if (message.startsWith("i/")) {
      String intensity = message.substring(2); // get the substring after the /
      i = intensity.toFloat();             // convert to a number
      changeString(h, s, i);
    }
    // A message of /on starts the tree:
    if (message.startsWith("on")) {
      running = true;
      resetLights();
    }

    // A message of /off stops the tree:
    if (message.startsWith("off")) {
      Console.println("off");
      treeShutdown();
    }
    // the twinkle routine fades to white:
    if (message.startsWith("twinkle")) {
      twinkle();
    }
  }
  // fade pixels back to base color one point per 30ms:
  if (running && (millis() % 30 < 2)) {
    fadePixels();
  }
  // random twinkle. It's very rare:
  if (running && (random(50) == millis())) {
    twinkle();
  }
  // update the strip:
  if (running) strip.show();
}

void treeShutdown() {
  // iterate over all pixels, shut down one every
  // 300 - 500 ms:
  for (int thisPixel = 0; thisPixel < pixelCount; thisPixel++) {
    strip.setPixelColor(thisPixel, 0);
    strip.show();
    delay(random(300) + 200);
  }
  running = false;
}
/*
  this function creates the twinkle effect:
*/
void twinkle() {
  Console.println("twinkle");
  int thisPixel = random(pixelCount);
  unsigned long currentColor = strip.getPixelColor(thisPixel);
  strip.setPixelColor(thisPixel, 0xFFEEEE);
}

// fade the pixels:
void fadePixels() {
  // iterate over all pixels:
  for (int thisPixel = 0; thisPixel < pixelCount; thisPixel++) {
    // fade the pixel one step from the current color to the target color:
    compare(thisPixel, strip.getPixelColor(thisPixel), baseColor[thisPixel]);
  }
}

/*
  takes two colors and returns a color that's a point on each axis (r, g, b)
  closer to the target color
*/
void compare(int thisPixel, unsigned long thisColor, unsigned long thatColor) {
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
  strip.setPixelColor(thisPixel, result);
}

// reset the lights to a random array of the three colors:
void resetLights() {
  Console.println("on");
  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    int colorNumber = random(colorCount);
    RGBColor color = colors[colorNumber];
    baseColor[pixel]  = ((unsigned long)color.red << 16)
                        | ((unsigned long)color.green << 8)
                        | (color.blue);
    strip.setPixelColor(pixel, color.red, color.green, color.blue);    // set the color for this pixel
  }
}

// change the string to a single color:
void changeString(int hue, int sat, int intensity) {
  // create a single color from hue, sat, intensity:
  RGBColor color = converter.HSItoRGB(hue, sat, intensity);

  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    strip.setPixelColor(pixel, color.red, color.green, color.blue);    // set the color for this pixel
  }
}
