#include <Adafruit_LEDBackpack.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_SPITFT.h>
#include <gfxfont.h>

/*************************************************** 
  This is a library for our I2C LED Backpacks

  Designed specifically to work with the Adafruit LED Matrix backpacks 
  ----> http://www.adafruit.com/products/872
  ----> http://www.adafruit.com/products/871
  ----> http://www.adafruit.com/products/870

  These displays use I2C to communicate, 2 pins are required to 
  interface. There are multiple selectable I2C addresses. For backpacks
  with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
  with 3 Address Select pins: 0x70 thru 0x77

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();
bool mainLoop = false;

void setup()
{
    Serial.begin(9600);
    Serial.println("Ising Model Test");

    matrix.begin(0x70); // pass in the address
}

void loop()
{

    matrix.clear(); // clear display
    matrix.drawPixel(0, 0, LED_GREEN);
    matrix.writeDisplay(); // write the changes we just made to the display
    delay(500);

    if (!mainLoop)
    {
        matrix.setTextWrap(false); // we dont want text to wrap so it scrolls nicely
        matrix.setTextSize(1);
        matrix.setTextColor(LED_GREEN);
        for (int8_t x = 7; x >= -36; x--)
        {
            matrix.clear();
            matrix.setCursor(x, 0);
            matrix.print("Ising");
            matrix.writeDisplay();
            delay(100);
        }
    }
}