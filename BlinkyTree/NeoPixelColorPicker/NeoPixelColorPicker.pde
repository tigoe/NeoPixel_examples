/*
  NeoPixel Color Picker
  
  Allows you to pick a color, then sends the color out the serial
  port in the following format: Cr,g,b\n
  
  Can be used to address NeoPixels run by an Arduino using this sketch:
  
  based on http://forum.processing.org/one/topic/processing-color-picker.html
  by Processing forum user owaunandrew
  
  modified for serial by Tom Igoe
  12 Dec 2014

*/

import processing.serial.*;

Serial myPort;

ColorPicker cp;

void setup() 
{
  size( 500, 500 );
  frameRate( 100 );

  cp = new ColorPicker( 10, 10, 400, 400, 255 );
  myPort = new Serial(this, "/dev/tty.usbmodem1421", 9600);
}

void draw ()
{
  background( 80 );
  cp.render();
}

public class ColorPicker 
{
  int x, y, w, h, c;
  PImage cpImage;

  public ColorPicker ( int x, int y, int w, int h, int c )
  {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
    this.c = c;

    cpImage = new PImage( w, h );

    init();
  }

  private void init ()
  {
    // draw color.
    int cw = w - 60;
    for ( int i=0; i<cw; i++ ) 
    {
      float nColorPercent = i / (float)cw;
      float rad = (-360 * nColorPercent) * (PI / 180);
      int nR = (int)(cos(rad) * 127 + 128) << 16;
      int nG = (int)(cos(rad + 2 * PI / 3) * 127 + 128) << 8;
      int nB = (int)(Math.cos(rad + 4 * PI / 3) * 127 + 128);
      int nColor = nR | nG | nB;

      setGradient( i, 0, 1, h/2, 0xFFFFFF, nColor );
      setGradient( i, (h/2), 1, h/2, nColor, 0x000000 );
    }

    // draw black/white.
    drawRect( cw, 0, 30, h/2, 0xFFFFFF );
    drawRect( cw, h/2, 30, h/2, 0 );

    // draw grey scale.
    for ( int j=0; j<h; j++ )
    {
      int g = 255 - (int)(j/(float)(h-1) * 255 );
      drawRect( w-30, j, 30, 1, color( g, g, g ) );
    }
  }

  private void setGradient(int x, int y, float w, float h, int c1, int c2 )
  {
    float deltaR = red(c2) - red(c1);
    float deltaG = green(c2) - green(c1);
    float deltaB = blue(c2) - blue(c1);

    for (int j = y; j< (y+h); j++)
    {
      int c = color( red(c1)+(j-y)*(deltaR/h), green(c1)+(j-y)*(deltaG/h), blue(c1)+(j-y)*(deltaB/h) );
      cpImage.set( x, j, c );
    }
  }

  private void drawRect( int rx, int ry, int rw, int rh, int rc )
  {
    for (int i=rx; i<rx+rw; i++) 
    {
      for (int j=ry; j<ry+rh; j++) 
      {
        cpImage.set( i, j, rc );
      }
    }
  }

  public void render ()
  {
    image( cpImage, x, y );
    if ( mousePressed &&
      mouseX >= x && 
      mouseX < x + w &&
      mouseY >= y &&
      mouseY < y + h )
    {
      c = get( mouseX, mouseY );
    }
    fill( c );
    rect( x, y+h+10, 20, 20 );
    myPort.write("C" + int(red(c)) + "," + 
      int(green(c)) + "," + 
      int(blue(c)) + "\n");
  }
}


void keyReleased() {
 myPort.write(key);
 println(key);
}

