# NeoPixel Examples

This repository includes examples for controlling WS218x and SK681x addressable LEDs using Adafruit's NeoPixel library. 

Addressable LEDs come in many forms, but the most common is a flexible strip, so you'll often see a device listed as `strip` in code examples. These will test a string, ring, or whatever form you have). 

## How to use the Examples 

To use any of these examples: 
* set the `neoPixelPin` variable to the pin you're controlling the strip with
* set the `numPixels` value to the number of LEDs on your strip
* In the strip setup line, change the `NEO_RGBW` constant according to your needs. The line is typically as follows by default:

````arduino
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRBW + NEO_KHZ800);
````

The `NEO_RGBW` sets the order for the control of colors. it varies from manufacturer to manufacturer of the LEDs. If you are not controlling RGBW LEDs, take away the W. If your LEDs light up in an order that you don't expect, change from GRB to RGB or BRG or whatever produces the results you expect. The NeoPixelTester sketch is a good one to set this with, as it is written to turn on each LED in a strip, one at a time, in the sequence Blue, Green, Red, White.


[NeoPixelTester](https://github.com/tigoe/NeoPixel_examples/tree/main/NeoPixelTester) - This sketch tests all the LEDs in a given strip 

[NeoPixelSimpleRGBW](https://github.com/tigoe/NeoPixel_examples/tree/main/NeoPixelSimpleRGBW) - This sketch turns on all the LEDs, one by one until they are all on. It uses four separate byte variables for red, green, blue, and white. 

[NeoPixelSimpleRGBWHex](https://github.com/tigoe/NeoPixel_examples/tree/main/NeoPixelSimpleRGBWHex) - This sketch shows how to control the LEDs' color with a single variable, rather than four variables. This can be useful when you want to change color spaces. LED colors are represented in a single unsigned long integer, which is written in the code hexadecimal style, as in HTML colors, like so: 0x00FF0000 is red, 0x0000FF00 is green, and 0x000000FF is blue, and 0xFF000000 is white.

[NeoPixelRGBWSerialIn](https://github.com/tigoe/NeoPixel_examples/tree/main/NeoPixelRGBWSerialIn) - This sketch waits for a serial string with five numbers: pixel number, red, green, blue, and white. Then it lights up the appropriate LED with the color.

[NeoPixelBounce](https://github.com/tigoe/NeoPixel_examples/tree/main/NeoPixelBounce) - This sketch bounces a color up and down the strip. 

[NeoPixelFadeAll](https://github.com/tigoe/NeoPixel_examples/tree/main/NeoPixelFadeAll) - This sketch shows how to fade all the LEDs in a strip at once using the `setBrightness()` command. 

[NeoPixelFadeChase](https://github.com/tigoe/NeoPixel_examples/tree/main/NeoPixelFadeChase) - This sketch shows how to a chase with a fading trail of LEDs behind the brightest one. 

[NeoPixelHSI](https://github.com/tigoe/NeoPixel_examples/tree/main/NeoPixelHSI) uses my [ColorConverter](https://github.com/tigoe/ColorConverter) library to take serial input in Hue, Saturation and Intensity (HSI) colorspace values.


[NeoPixelHSIController](https://github.com/tigoe/NeoPixel_examples/tree/main/NeoPixelHSIController) uses my [ColorConverter](https://github.com/tigoe/ColorConverter) library to take input from three potentiometers to control a strip in Hue, Saturation and Intensity (HSI) colorspace values.


[NeoPixelRGBController](https://github.com/tigoe/NeoPixel_examples/tree/main/NeoPixelRGBController) takes input from three potentiometers to control a stip in red, green, and blue values.

[WWANeoPixel](https://github.com/tigoe/NeoPixel_examples/tree/main/WWANeoPixel) controls a strip of white, white, amber LEDs using my [ColorConverter](https://github.com/tigoe/ColorConverter) library