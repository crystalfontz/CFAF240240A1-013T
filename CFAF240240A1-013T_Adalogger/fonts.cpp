#include "fonts.h"
#include "st7789h2.h"
#include "atsamd21g18.h"

const FONT_HEADER *font; // = &f_08x08;
  
void LCD_Character(uint8_t x0, uint8_t y0, uint8_t c)
{

  color_t foreground = { 0xFF, 0xFF, 0xFF };
  color_t background = { 0x00, 0x00, 0x00 };
  bool transparent = false;
  bool proportional = false;

  //singly, "register" saves 32
  register uint8_t x;
  //singly, "register" saves 32
  register uint8_t y;
  //singly, "register" saves 16
  register const uint8_t *bitmap_data;
  //singly, "register" saves 8, but costs with above allocations
  uint32_t mask;
  //singly, "register" saves 8, but costs with above allocations
  uint32_t line_data;
  //singly, "register" saves 8, but costs with above allocations
  uint32_t top_rows_blank;
  //singly, "register" saves 8, but costs with above allocations
  uint32_t cols_wide;
  //singly, "register" saves 8, but costs with above allocations
  uint32_t left_cols_blank;

  //Make sure the character is in range of this font.
  //(hopefully the font has a '?' in it ;)
  if (
    (c < font->start_char) || 
    ((font->start_char + font->number_of_chars) < c)
  )
  {
    c = '?';
  }
  //Change the ASCII input to an index into the font
  c -= font->start_char;

  //Initialize pointer to this character's bitmap data
  bitmap_data = &font->fd[font->fc[c].data_index];

  //Bring in the deeply indexed pointers to simple ints
  top_rows_blank = font->fc[c].top_rows_blank;
  cols_wide = font->fc[c].cols_wide;
  //font->c[c].left_cols_blank is used to center the
  //character in the cell for fixed width strings. If we
  //are proportional, we will act like it is 0.
  left_cols_blank =
    proportional ? 0 : font->fc[c].left_cols_blank;



//top_rows_blank=0
//cols_wide=10
//left_cols_blank=1
//proportional=0
//transparent=0

  //Screen is upside down compared to the font
  y0 += font->cell_max_height;
  CLR_CS;
  CLR_RS; // Select the LCD's command register
  M_SPI_WRITE_WAIT(ST7789_RAMWR); //write data
  SET_RS; // Select the LCD's data register

  if (transparent)
  {
    //Transparent background, do not draw background pixels
    for (y = 0; y < font->cell_max_height; y++)
    {
      //Skip blank lines
      /*lint --e(737)*/
      if ((top_rows_blank <= y)
              && (y < (top_rows_blank + font->fc[c].rows_high)))
      {
        //Now get the bitmap data and shove it out.
        line_data = *bitmap_data++;
        mask = 0x80;
        //If it is a two-byte character, get the second byte
        //of bitmap data.
        if (8 < cols_wide)
        {
          line_data <<= 8;
          line_data |= *bitmap_data++;
          mask = 0x8000;
        }
        for (x = 0; x < cols_wide; x++, mask >>= 1)
          if (line_data & mask)
            Put_Pixel(x0 + x + left_cols_blank, y0 - y,
                    foreground);
      }
    }
  }
  else
  {
    //Not transparent, so draw background pixels. Since we draw
    //every pixel, we can set the address once per line and
    //then just push the new data to the LCD controller, rather
    //than setting the address for every pixel.
    for (y = 0; y < font->cell_max_height; y++)
    {
      //Point the LCD to the start of this line
      //Set_LCD_for_write_at_X_Y(x0, y0 - y);
      //set_raw_XY(x0, y0 - y);

      //Check for blank lines
      /*lint --e(737)*/
      if ((y < top_rows_blank)
              || ((top_rows_blank + font->fc[c].rows_high) <= y))
      {
        //This is an entirely blank Line
        // This covers every blank line 
        for (x = 0; x < font->cell_max_width; x++)
        {

          //Background
          //Write the single pixel's worth of data
          // M_SPI_WRITE_WAIT(background.r);
          // M_SPI_WRITE_WAIT(background.g);
          // M_SPI_WRITE_WAIT(background.b);
          Put_Pixel(x0 + x, y0 - y, background);
        }
      }
      else
      {
        //Not a blank line. Set or clear based on the bit data.
        //Push out the leading  blank columns
        for (x = 0; x < left_cols_blank; x++)
        {
          //Background
          //Write the single pixel's worth of data
          // M_SPI_WRITE_WAIT(background.r);
          // M_SPI_WRITE_WAIT(background.g);
          // M_SPI_WRITE_WAIT(background.b);
          Put_Pixel(x0 + x, y0 - y, background);
        }
        //Now get the bitmap data and shove it out.
        line_data = *bitmap_data++;
        mask = 0x80;
        //If it is a two-byte character, get the second byte
        //of bitmap data.
        if (8 < cols_wide)
        {
          line_data <<= 8;
          line_data |= *bitmap_data++;
          mask = 0x8000;
        }
        //Conditionally set or clear the pixels based on the bitmap
        for (x = left_cols_blank; x < (left_cols_blank + cols_wide); x++, mask >>= 1)
        {
          if (line_data & mask)
          {
            //Foreground
            //Write the single pixel's worth of data
            // M_SPI_WRITE_WAIT(foreground.r);
            // M_SPI_WRITE_WAIT(foreground.g);
            // M_SPI_WRITE_WAIT(foreground.b);
            Put_Pixel(x0 + x, y0 - y, foreground);
          }
          else
          {
            //Background
            //Write the single pixel's worth of data
            // M_SPI_WRITE_WAIT(background.r);
            // M_SPI_WRITE_WAIT(background.g);
            // M_SPI_WRITE_WAIT(background.b);
            Put_Pixel(x0 + x, y0 - y, background);
          }
        }
        //Push out the trailing blank columns
        for (x = (left_cols_blank + cols_wide); x < font->cell_max_width;
                x++)
        {
          //Background
          //Write the single pixel's worth of data
          // M_SPI_WRITE_WAIT(background.r);
          // M_SPI_WRITE_WAIT(background.g);
          // M_SPI_WRITE_WAIT(background.b);
          Put_Pixel(x0 + x, y0 - y, background);
        }

      }
 // Deselect the LCD controller
  SET_CS;

    }
  }
}
// ********************************************************
void LCD_String(uint8_t x0, uint8_t y0, const char *s)
{
  uint8_t c;
  while (0 != (c = (uint8_t) * s))
  {
    LCD_Character(x0, y0, c);
    //Move over, rover
    // if (proportional)
    //   /*lint --e(737)*/
    //   x0 += font->fc[c - font->start_char].cols_wide + font->prop_space;
    // else
      x0 += font->cell_max_width;
    //Move to the next character
    s++;
  }
}