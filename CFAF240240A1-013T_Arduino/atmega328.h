#ifndef __ATMEGA328_H__
#define __ATMEGA328_H__

#include <Arduino.h>
#include <SD.h>
#include <avr/io.h>

extern uint16_t SectorsPerClust;
extern uint16_t FirstDataSector;
extern uint8_t FAT32_Enable;



//============================================================================
//
// LCD SPI & control lines
//   ARD   | Port | LCD                      | Wire
// --------+------+--------------------------+------------
//  #4/D5  |  PD5 | TE (Input)               |
//  #4/D6  |  PD6 | BL_EN                    |
//  #7/D7  |  PD7 | SD_CS                    | Grey   
// --------+------+--------------------------+------------
//  #8/D8  |  PB0 | LCD_RS                   | Yellow
//  #9/D9  |  PB1 | LCD_RESET                | Purple
// #10/D10 |  PB2 | LCD_CS_NOT (or SPI SS)   | Grey
// #11/D11 |  PB3 | LCD_MOSI (hardware SPI)  | Green 
// #12/D12 |  PB4 | not used (would be MISO) | Blue
// #13/D13 |  PB5 | LCD_SCK (hardware SPI)   | White
// ----+--------+-----
//#define LCD_EN    0x08
//#define LCD_TE    0x20  
#define LCD_IM3   0x40  
#define uSD_CS    0x80 // Pin 7
// ----+--------+-----
#define LCD_RS    0x01
#define	LCD_RESET	0x02
#define	LCD_CS	  0x04
#define LCD_MOSI  0x08
#define LCD_MISO  0x10
#define LCD_SCK   0x20
// ----+--------+-----
// #define CLR_EN    (PORTD &= ~(LCD_EN))
// #define SET_EN    (PORTD |=  (LCD_EN))
// #define CLR_TE    (PORTD &= ~(LCD_TE))
// #define SET_TE    (PORTD |=  (LCD_TE))
#define CLR_IM3   (PORTD &= ~(LCD_IM3))
#define SET_IM3   (PORTD |=  (LCD_IM3))
#define CLR_uCS   (PORTD &= ~(uSD_CS))
#define SET_uCS   (PORTD |=  (uSD_CS))
// ----+--------+-----
#define CLR_RS    (PORTB &= ~(LCD_RS))
#define SET_RS    (PORTB |=  (LCD_RS))
#define CLR_RESET (PORTB &= ~(LCD_RESET))
#define SET_RESET (PORTB |=  (LCD_RESET))
#define CLR_CS    (PORTB &= ~(LCD_CS))
#define SET_CS    (PORTB |=  (LCD_CS))
#define CLR_MOSI  (PORTB &= ~(LCD_MOSI))
#define SET_MOSI  (PORTB |=  (LCD_MOSI))
#define CLR_MISO  (PORTB &= ~(LCD_MISO))
#define SET_MISO  (PORTB |=  (LCD_MISO))
#define CLR_SCK   (PORTB &= ~(LCD_SCK))
#define SET_SCK   (PORTB |=  (LCD_SCK))

//============================================================================

// #define LCD_BL  PC0	//Backlight pin for testing
// #define BL_OFF PORTC &= ~(1<<LCD_BL);
// #define BL_ON  PORTC |=  (1<<LCD_BL);

//extern void clearScreen(void);

void hostInit(void);
void pictureSlideShow();
void writeCommand(uint8_t command);
void writeData(uint8_t data);
void show_BMPs_in_root(void);
void SPI_send_pixels_565(uint8_t pixel_count, uint8_t *data_ptr);
void SPI_send_pixels_666(uint8_t byte_count, uint8_t *data_ptr);

#endif /* __ATMEGA328_H__ */
