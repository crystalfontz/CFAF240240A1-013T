//===========================================================================
//
//  Code written for Seeeduino v4.2 set to 3.3v (important!)
//
//  CRYSTALFONTZ CFAF240240A1-013T 240X240 SPI COLOR 1.3" TFT
//  ref: https://www.crystalfontz.com/product/cfaf240240a1013t
//
//  This code uses 4-wire SPI mode.
//
// The controller is a Sitronix ST7789H2
//   http://www.crystalfontz.com/controllers/Sitronix/ST7789H2
//
//  2018-05-04 Max Roberg
//===========================================================================
//This is free and unencumbered software released into the public domain.
//
//Anyone is free to copy, modify, publish, use, compile, sell, or
//distribute this software, either in source code form or as a compiled
//binary, for any purpose, commercial or non-commercial, and by any
//means.
//
//In jurisdictions that recognize copyright laws, the author or authors
//of this software dedicate any and all copyright interest in the
//software to the public domain. We make this dedication for the benefit
//of the public at large and to the detriment of our heirs and
//successors. We intend this dedication to be an overt act of
//relinquishment in perpetuity of all present and future rights to this
//software under copyright law.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//OTHER DEALINGS IN THE SOFTWARE.
//
//For more information, please refer to <http://unlicense.org/>
//============================================================================
//
// Display is Crystalfontz  CFAF240240A1-013T 240X240 SPI COLOR 1.3" TFT
//   https://www.crystalfontz.com/product/cfaf240240a1013t
//
//============================================================================
//#include <avr/io.h>

//#include <SPI.h>
// C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\SPI\src\SPI.cpp
// C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\SPI\src\SPI.h

#include <SD.h>
//#include <avr/io.h>
// C:\Program Files (x86)\Arduino\libraries\SD\src\SD.cpp
// C:\Program Files (x86)\Arduino\libraries\SD\src\SD.h

#include "atsamd21g18.h"
#include "st7789h2.h"

//============================================================================
void setup()
{
  //debug console
  Serial.begin(9600);
  Serial.println("setup()");

  // Pin tests
  //  Do I have control on the pins?
  //    REG_PORT_OUTSET0 for PORTA
  //    REG_PORT_OUTSET1 for PORTB
  // TE PASSED, IM3
  //REG_PORT_DIRSET1 = LCD_TE; //TE
  
  // while(1)
  // {
  //   Serial.println("Setting Pin");
  //   SET_IM3;
  //   delay(500); // delay for 1 second
  //   Serial.println("Clearing Pin");
  //   CLR_IM3;
  //   delay(500); // delay for 1 second
  // }

  //Set up port B as ALL inputs
  //pinMode(3, OUTPUT);   //LCD_EN TIED to GND
  //pinMode(5, INPUT);   //LCD_TE
  REG_PORT_DIRSET1 = LCD_IM3;   //LCD_IM3
  REG_PORT_DIRSET0 = uSD_CS;;   //uSD_CS
  
  //CLR_IM3; //SDI/SDO share MISO
  SET_IM3;  //SDI/SDO on different pins

  REG_PORT_DIRSET1 = LCD_RS;   //LCD_RS
  REG_PORT_DIRSET0 = LCD_RESET;   //LCD_RESET
  REG_PORT_DIRSET0 = LCD_CS;;  //LCD_CS
  
  //Drive the ports to a reasonable starting state.
  CLR_RESET;  //Active low
  SET_CS;     //Active low
  CLR_RS;
  CLR_MOSI;
  CLR_SCK;

  // For the Seeduino I am using, the default speed of SPI_HALF_SPEED
  // set in C:\Program Files (x86)\Arduino\libraries\SD\src\SD.cpp
  // results in a 4MHz clock.
  //
  // If you change this function call in SDClass::begin() of SD.cpp
  // from:
  //
  //  return card.init(SPI_HALF_SPEED, csPin) &&
  //         volume.init(card) &&
  //         root.openRoot(volume);
  //
  // to:
  //
  //  return card.init(SPI_FULL_SPEED, csPin) &&
  //         volume.init(card) &&
  //         root.openRoot(volume);
  //
  // That appears to make the SD library talk at 8MHz.
  //

  if (!SD.begin(7))
    {
    Serial.println("Card failed to initialize, or not present");
    //Reset the SPI clock to fast. SD card library does not clean up well.
    //Bump the clock to 8MHz. Appears to be the maximum.
    }
  else
    {
    Serial.println("Card initialized.");
    }

  //Bump the clock to 8MHz. Appears to be the maximum.
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

  //Initialize the LCD controller
  displayInit();
}
//============================================================================
void loop()
{
  Serial.println("Writing a white screen");
  fillScreen(WHITE);
  delay(500);

  Serial.println("Writing a red screen");
  fillScreen(RED);
  delay(500);

  Serial.println("Writing a green screen");
  fillScreen(GREEN);
  delay(500);

  Serial.println("Writing a blue screen");
  fillScreen(BLUE);
  delay(500);

  Fill_OLED_Gamma_Gradient(240, 240);
  delay(1000);

  Serial.println("Writing a black screen");
  fillScreen(BLACK);
  delay(500);

  writeColorBars(240, 240);
  delay(1000);

  Serial.println("Circle tests");
  //Draw a cyan circle
  LCD_Circle(64, 64, 63,0x00,0xFF,0xFF);
  //Draw a green circle
  LCD_Circle(21, 64, 20,0x00,0xFF,0x00);
  //Draw a white circle
  LCD_Circle(64, 64, 20,0xFF,0xFF,0xFF);
  //Draw a red circle
  LCD_Circle(107, 64, 20,0xFF,0x00,0x00);
  //Draw a purple circle
  LCD_Circle(64, 107, 16,0xFF,0x00,0xFF);
  //Draw a orange circle
  LCD_Circle(64, 21, 14,0xFF,0xA5,0x00);
  delay(1000);

  Serial.println("draw some circles");
  for(uint8_t i = 2; i < 60; i += 2)
  {
    LCD_Circle(i+2, 64, i,i<<2,0xff-(i<<2),0x00);
  }
  delay(1000);

  show_BMPs_in_root();
} // void loop()
//============================================================================
