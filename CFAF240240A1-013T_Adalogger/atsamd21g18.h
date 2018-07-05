#ifndef __ATMEGA328_H__
#define __ATMEGA328_H__

#include <Arduino.h>
#include <SD.h>
#include <avr/io.h>
// #include <avr/interrupt.h>
// #include <avr/pgmspace.h>
//#include "MMC_SD.h"

extern uint16_t SectorsPerClust;
extern uint16_t FirstDataSector;
extern uint8_t FAT32_Enable;

class pixel_t
{
  public:
    uint8_t r, g, b;
};

// struct FileInfoStruct FileInfo;
// struct direntry PictureInfo;

//============================================================================
//
// LCD SPI & control lines
//   ARD   | Port | LCD                      | Wire
// --------+------+--------------------------+------------
//  #4/D4  |  PD4 | LCD_EN                   |
//  #4/D5  |  PD5 | TE (Input)               |
//  #4/D6  |  PD6 | IM3                      |
//  #7/D7  |  PD7 | SD_CS                    | Grey   
// --------+------+--------------------------+------------
//  #8/D8  |  PB0 | LCD_RS                   | Yellow
//  #9/D9  |  PB1 | LCD_RESET                | Purple
// #10/D10 |  PB2 | LCD_CS_NOT (or SPI SS)   | Grey
// #11/D11 |  PB3 | LCD_MOSI (hardware SPI)  | Green 
// #12/D12 |  PB4 | not used (would be MISO) | Blue
// #13/D13 |  PB5 | LCD_SCK (hardware SPI)   | White
// ----+--------+-----
//#define LCD_EN    0x08  // Currently tied to GND
// ----+--------+-----
#define	LCD_RESET	PORT_PA04
#define	LCD_CS	  PORT_PA05
#define uSD_CS    PORT_PA08
#define LCD_MISO  PORT_PA12
// ----+--------+-----
#define LCD_RS    PORT_PB02
#define LCD_TE    PORT_PB08  
#define LCD_IM3   PORT_PB09
#define LCD_MOSI  PORT_PB10
#define LCD_SCK   PORT_PB11
// #define CLR_EN    (REG_PORT_OUT0 &= ~(LCD_EN))
// #define SET_EN    (PORTD |=  (LCD_EN))

// ----+--------+-----
#define CLR_RESET (REG_PORT_OUT0 &= ~(LCD_RESET))
#define SET_RESET (REG_PORT_OUT0 |= LCD_RESET)
#define CLR_CS    (REG_PORT_OUT0 &= ~(LCD_CS))
#define SET_CS    (REG_PORT_OUT0 |= LCD_CS)
#define CLR_uCS   (REG_PORT_OUT0 &= ~(uSD_CS)))
#define SET_uCS   (REG_PORT_OUT0 |= uSD_CS)
#define CLR_MISO  (REG_PORT_OUT0 &= ~(LCD_MISO))
#define SET_MISO  (REG_PORT_OUT0 |= LCD_MISO)
// ----+--------+-----
#define CLR_RS    (REG_PORT_OUT1 &= ~(LCD_RS))
#define SET_RS    (REG_PORT_OUT1 |= LCD_RS)
#define CLR_TE    (REG_PORT_OUT1 &= ~(LCD_TE))
#define SET_TE    (REG_PORT_OUT1 |= (LCD_TE))
#define CLR_IM3   (REG_PORT_OUT1 &= ~(LCD_IM3))
#define SET_IM3   (REG_PORT_OUT1 |= LCD_IM3)
#define CLR_MOSI  (REG_PORT_OUT1 &= ~(LCD_MOSI))
#define SET_MOSI  (REG_PORT_OUT1 |= LCD_MOSI)
#define CLR_SCK   (REG_PORT_OUT1 &= ~(LCD_SCK))
#define SET_SCK   (REG_PORT_OUT1 |= LCD_SCK)

//============================================================================

// #define LCD_BL  PC0	//Backlight pin for funsies, I mean testing
// #define BL_OFF PORTC &= ~(1<<LCD_BL);
// #define BL_ON  PORTC |=  (1<<LCD_BL);

//extern void clearScreen(void);

//void delay(uint16_t t);
void pictureSlideShow();
void writeCommand(uint8_t command);
void writeData(uint8_t data);
void show_BMPs_in_root(void);
void SPI_send_pixels_565(uint8_t pixel_count, uint8_t *data_ptr);
void SPI_send_pixels_666(uint16_t pixel_count, uint8_t *data_ptr);

#endif /* __ATMEGA328_H__ */
