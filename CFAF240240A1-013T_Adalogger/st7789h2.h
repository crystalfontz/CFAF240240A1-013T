#ifndef __ST7789H2_H__
#define __ST7789H2_H__
/*
 * ST7789H2.h
 *
 * Created: 2015-08-18 9:16:09 AM
 *  Author: max
 */ 

#include <Arduino.h>
#include <SPI.h>
#include "atsamd21g18.h"

// color definitions
const color_t BLACK   = {0x00, 0x00, 0x00}; //#define	BLACK   0x000000
const color_t BLUE    = {0x00, 0x00, 0xFF}; //#define	BLUE    0x0000FF
const color_t GREEN   = {0x00, 0xFF, 0x00}; //#define	GREEN   0x00FF00
const color_t CYAN    = {0x00, 0xFF, 0xFF}; //#define CYAN    0x00FFFF
const color_t RED     = {0xFF, 0x00, 0x00}; //#define	RED     0xFF0000
const color_t MAGENTA = {0xFF, 0x00, 0xFF}; //#define MAGENTA 0xFF00FF
const color_t YELLOW  = {0xFF, 0xFF, 0x00}; //#define YELLOW  0xFFFF00
const color_t WHITE   = {0xFF, 0xFF, 0xFF}; //#define WHITE   0xFFFFFF

// ********************************************************
// Defines for the ST7789 registers.
// ref: https://www.crystalfontz.com/products/document/3277/ST7735_V2.1_20100505.pdf
// ********************************************************
#define ST7789_SLPIN      (0x10)  //Sleep in
#define ST7789_SLPOUT     (0x11)  //Sleep Out
#define ST7789_INVOFF     (0x20)  //Display Inversion Off
#define ST7789_INVON      (0x21)  //Display Inversion On
#define ST7789_DISPOFF    (0x28)  //Display Off
#define ST7789_DISPON     (0x29)  //Display On
#define ST7789_CASET      (0x2A)  //Column Address Set
#define ST7789_RASET      (0x2B)  //Row Address Set
#define ST7789_RAMWR      (0x2C)  //Memory Write
#define ST7789_TEOFF      (0x34)  //Tearing Effect Line OFF
#define ST7789_TEON       (0x35)  //Tearing Effect Line On
#define ST7789_MADCTL     (0x36)  //Memory Data Access Control
#define ST7789_COLMOD     (0x3A)  //Interface Pixel Format
#define ST7789_WRCACE     (0x55)  //Write Content Adaptive Brightness Control and Color Enhancement
#define ST7789_WRCABCMB   (0x5E)  //Write CABC minimum brightness
#define ST7789_PORCTRL    (0xB2)  //Porch control
#define ST7789_GCTRL      (0xB7)  //Gate Control
#define ST7789_VCOMS      (0xBB)  //Gate on timing adjustment
#define ST7789_LCMCTRL    (0xC0)  //LCM Control
#define ST7789_VDVVRHEN   (0xC2)  //VDV and VRH Command Enable
#define ST7789_VRHS       (0xC3)  //VRH Set
#define ST7789_VDVSET     (0xC4)  //VDV Set
#define ST7789_FRCTR2     (0xC6)  //Frame Rate Control in Normal Mode
#define ST7789_CABCCTRL   (0xC7)  //CABC Control
#define ST7789_PWCTRL1    (0xD0)  //Power Control 1
#define ST7789_PVGAMCTRL  (0xE0)  //Positive Voltage Gamma Control
#define ST7789_NVGAMCTRL  (0xE1)  //Negative Voltage Gamma Control
// ********************************************************

void clearScreen(void);
void displayHome(void);
void displayInit(void);
void enterSleep (void);
void exitSleep (void);
void Fast_Horizontal_Line(uint16_t x0, uint16_t y, uint16_t x1, color_t color);
void Fill_OLED_Gamma_Gradient(uint8_t height, uint8_t width);
void fillScreen(color_t color);
void LCD_Circle(uint8_t x0, uint8_t y0, uint8_t radius, color_t color);
void OLED_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t color);
void Put_Pixel(uint8_t x, uint8_t y, color_t color);
void Put_Pixel_Raw(uint8_t x, uint8_t y, uint8_t R, color_t color);
//void Set_LCD_for_write_at_X_Y(uint8_t x, uint8_t y);
void setDisplayWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void setInterface(void);
//void writeColor(uint32_t color);
void writeColor(color_t color);
void writeColorBars(uint8_t height, uint8_t width);
void SPI_sendCommand(uint8_t command);
void SPI_sendData(uint8_t data);
void set_raw_XY(uint8_t x,uint8_t y);

//
// Customer code after this line
//

#endif /* __ST7789H2_H__ */
