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

#include "atmega328.h"
#include "st7789h2.h"

//============================================================================
void setup()
{
  // LCD SPI & control lines
  //   ARD   | Port | LCD                      | wire
  // --------+------+--------------------------+------------
  //  #5/D5  |  PD5 | SYNC_PIN_SLAVE  (optional -- generally not used)
  //  #6/D6  |  PD6 | SYNC_PIN_MASTER (optional -- generally not used)
  //  #7/D7  |  PD7 | SD_CS                    | Purple
  //  #8/D8  |  PB0 | LCD_RS                   | Yellow
  //  #9/D9  |  PB1 | LCD_RESET                | Purple
  // #10/D10 |  PB2 | LCD_CS_NOT (or SPI SS)   | Grey
  // #11/D11 |  PB3 | LCD_MOSI (hardware SPI)  | Green 
  // #12/D12 |  PB4 | not used (would be MISO) | Blue
  // #13/D13 |  PB5 | LCD_SCK (hardware SPI)   | White
  //==========================================================================
  //Set up port B as ALL inputs
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_RESET, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_MOSI, OUTPUT);
  pinMode(LCD_SCK, OUTPUT);
  pinMode(uSD_CS, OUTPUT);
  //DDRB = 0x3F;
  
  //Drive the ports to a reasonable starting state.
  CLR_RESET;  //Active low
  SET_CS;     //Active low
  CLR_RS;     //
  CLR_MOSI;
  CLR_SCK;

  //debug console
  Serial.begin(9600);
  Serial.println("setup()");

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
    //Pin 7 is used for the SD card CS.
  //pinMode(7, OUTPUT);

  // Initialize SPI. By default the clock is 4MHz. The chip is good to 10 MHz
  SPI.begin();
  //Bump the clock to 8MHz. Appears to be the maximum.
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));


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

  //Initialize the LCD controller
  displayInit();
}
//============================================================================
void loop()
{
  Serial.println("Writing a white screen");
  fillScreen(WHITE);
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

