#include <SPI.h>
#include "st7789h2.h"
#include "atmega328.h"

// **************************************************
void show_BMPs_in_root(void)
{
  Serial.println("Showing BMPs from uSD Card");

  File root_dir;
  root_dir = SD.open("/");
  if(0 == root_dir)
  {
    Serial.println("show_BMPs_in_root: Can't open \"root\"");
  }
  else
  {
    File bmp_file;
    
    do
    {

      bmp_file = root_dir.openNextFile();
      if (0 == bmp_file)
      {
        // no more files, break out of while()
        // root_dir will be closed below.
        break;
      }
      //Skip directories (what about volume name?)
      else if(0 == bmp_file.isDirectory())
      {
        //The file name must include ".BMP"
        if(0 != strstr(bmp_file.name(),".BMP"))
        {
          //The BMP must be exactly 172854 long
          //(this is correct for 240x240, 24-bit + 54)
          if(172854 == bmp_file.size())
          {
            Serial.print("Displaying ");
            Serial.println(bmp_file.name());
            //Jump over BMP header. BMP must be 240x240 24-bit
            bmp_file.seek(54);
            //Bitmap file happens to match the LCD 1:1. Nice.
            
            setDisplayWindow(0,0,239,239);

            //Since we are limited in memory, break the line up from
            // 240*3 = 720 bytes into three chunks of 240 pixels
            // each 240*3 = 720 bytes.
            //Making this static speeds it up slightly (10ms)
            //Reduces flash size uses less bytes.
            static uint8_t uSDLine[240];
            for(uint8_t line=0; line<(240); line++)
            {
              for(uint8_t line_section=0; line_section<3; line_section++)
              {
                //Get a third of the row worth of pixels
                bmp_file.read(uSDLine,240);
                //Now write this third to the TFT
                SPI_send_pixels_666(240,uSDLine);
              }
            }
          }
          else
          {
            Serial.print("This BMP is ");
            Serial.print(bmp_file.size());
            Serial.println(" bytes");
          }
        }
      }
      //Release the BMP file handle
      bmp_file.close();
      delay(2000);
    } while(0 == bmp_file);
  }
  //Release the root directory file handle
  root_dir.close();
}
// **************************************************
#if (0)
void pictureSlideShow()
{
	uint8_t i;              //loop variable
	uint16_t j;             //loop variable
	uint32_t p;             //cluster
	uint16_t *buffer;       //buffer
	uint16_t pics = 1;
	uint16_t totalFiles = 0;
	uint16_t slideshowFlag = 1;
	uint8_t sector;
	uint32_t pixels;
	unsigned char * PATH = "\\batch";

	Search(PATH, &PictureInfo, &totalFiles);

	if (totalFiles == 0)
	{
		return;
	}

	if(!(buffer = malloc(512)))
	{
		return;
	}
	
	do{
		clearScreen(); // BLACK
		//find the file
		Search(PATH, &PictureInfo, &pics);
		
		//the first cluster of the file
		p = PictureInfo.deStartCluster + (((uint32_t)PictureInfo.deHighClust) << 16);
		
		sector = 0;

		//read a sector
		FAT_LoadPartCluster(p, sector, buffer);

		// total # of pixels to fill
		pixels = (uint32_t) 240 * 240;

		// byte count
		j = 0; 
	
		while(pixels > 0)
		{
			writeColor(buffer[j]);  // write 16 bits
			pixels--;               // which is one pixel
			
			j++;                    // increment word count
			if (j == 256)           // time for a new sector
			{
				sector++;
				if (sector == SectorsPerClust)
				{
					p = FAT_NextCluster(p);	// read next cluster
					sector = 0;
				}

				FAT_LoadPartCluster(p, sector, buffer);	// read a sector
				j = 0;
			}
		}

		if(slideshowFlag)
		{
			for (i = 0; i < 3; i++)	// delay for a while
			{
				delay(0xFFFF);
			}

			pics++;					// increment picture number
			if (pics > totalFiles)	// if last
			{
				pics = 1;			// wrap around
			}

		}
	} while(slideshowFlag);
	
	free(buffer);
}
#endif //(SD_ENABLED)
// **************************************************
//============================================================================
// This function transfers data, in one stream. Slightly
// optimized to do index operations during SPI transfers.
void SPI_send_pixels_666(uint8_t byte_count, uint8_t *data_ptr)
{
  uint8_t subpixel;
    
  // Select the LCD's data register
  SET_RS;
  // Select the LCD controller
  CLR_CS;

  //Load the first byte
  subpixel=*data_ptr;
  
  while(byte_count)
  {
    //Send the byte out.
    SPDR = subpixel;
    //do something that can happen while transmitting
    data_ptr++; //point to next byte
    //Load the next byte
    subpixel=*data_ptr;
    //count this byte
    byte_count--;
    //Now that we have done all we can do, wait for the transfer to finish.
    while (!(SPSR & _BV(SPIF))) ;
  }

  //Needs some non-trivial delay here. Not sure why.
  delayMicroseconds(2);    
  // Deselect the LCD controller
  SET_CS;
}

void SPI_send_pixels_565(uint8_t pixel_count, uint8_t *data_ptr)
  {
  uint8_t
    r;
  uint8_t
    g;
  uint8_t
    b;
  uint8_t
    first_half;
  uint8_t
    second_half;

  // Select the OLED's data register
  SET_RS;
  // Select the OLED controller
  CLR_CS;

  //Load the first pixel. BMPs BGR format
  b=*data_ptr;
  data_ptr++;
  g=*data_ptr;
  data_ptr++;
  r=*data_ptr;
  data_ptr++;

  //The display takes two bytes (565) RRRRR GGGGGG BBBBB 
  //to show one pixel.
  first_half=(r&0xF8) | (g >> 5);
  second_half=((g << 3)&0xE0) | (b >> 3);

  while(pixel_count)
    {
    //Send the first half of this pixel out
    SPDR = first_half;
    //Load the next pixel while that is transmitting
    b=*data_ptr;
    data_ptr++;
    g=*data_ptr;
    data_ptr++;
    r=*data_ptr;
    data_ptr++;
    //Calculate the next first half while that is transmitting
    // ~1.9368us -0.1256 us = 1.8112uS
    first_half=(r&0xF8) | (g >> 5);
    //At 8MHz SPI clock, the transfer is done by now, so we
    //do not need this:
    // while (!(SPSR & _BV(SPIF))) ;
    //Send the second half of the this pixel out
    SPDR = second_half;
    //Calculate the next second half
    second_half=((g << 3)&0xE0) | (b >> 3);
    //Done with this pixel
    pixel_count--;
    //At 8MHz SPI clock, the transfer is done by now, so we
    //do not need this:
    // while (!(SPSR & _BV(SPIF))) ;
    }
  //Wait for the final transfer to complete before we bang on CS.
  while (!(SPSR & _BV(SPIF))) ;
  // Deselect the OLED controller
  SET_CS;    
  }
//================================================================================
void reset_display(void)
{
	CLR_RESET;
	delay(10);
	SET_RESET;
	delay(10);
}
//================================================================================
void writeCommand(uint8_t command)
{
	// Select the LCD's command register
	CLR_RS;
	// Select the LCD controller
	CLR_CS;

	//Send the command via SPI:
	SPI.transfer(command);
	//deselect the controller
	SET_CS;
}
//================================================================================
void writeData(uint8_t data)
{
	//Select the LCD's data register
	SET_RS;
	//Select the LCD controller
	CLR_CS;
	//Send the command via SPI:
	SPI.transfer(data);

	// Deselect the LCD controller
	SET_CS;
}
