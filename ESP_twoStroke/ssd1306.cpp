#include "i2c.h"
#include "ssd1306.h"
#include "fonts.h"

#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

#define CHAR(c) (&font8x8[(c - 32) * 8])
#define CHARB(c) (&font11x16[(c - 32) * 22])

uint8_t db[1024];          
uint8_t i2c_address = 0x3C;
uint8_t sda, scl;

uint8_t initCommands[25] = {
   SSD1306_DISPLAYOFF ,                    // 0xAE --turn off oled panel
   SSD1306_SETDISPLAYCLOCKDIV ,            // 0xD5
   0x80 ,                                  // 0x80 (also default value) 
   SSD1306_SETMULTIPLEX ,                  // 0xA8
   0x3F ,                                  // 0x3f (also default value)
   SSD1306_SETDISPLAYOFFSET ,              // 0xD3
   0x0 ,                                   // no offset
   SSD1306_SETSTARTLINE | 0x0 ,            // line #0
   SSD1306_CHARGEPUMP ,                    // 0x8D
   0x14 , 
   SSD1306_MEMORYMODE ,                    // 0x20
   0x00 ,                                  // 0x00 set addressing mode
   SSD1306_SEGREMAP | 0x1 ,
   SSD1306_COMSCANDEC ,
   SSD1306_SETCOMPINS ,                    // 0xDA
   0x12 ,
   SSD1306_SETCONTRAST ,                   // 0x81
   0xCF ,
   SSD1306_SETPRECHARGE ,                  // 0xd9
   0xF1 ,
   SSD1306_SETVCOMDETECT ,                 // 0xDB
   0x40 ,
   SSD1306_DISPLAYALLON_RESUME ,           // 0xA4
   SSD1306_NORMALDISPLAY ,                 // 0xA6
   SSD1306_DISPLAYON                       // 0xAF --turn on oled panel
};


//####################################################//

void ssd1306_init(uint8_t sda, uint8_t scl, uint8_t address){
  i2c_address = address;  // default to 0x3C
  i2c_init(sda, scl);     // default to sda 0, scl 2
  i2c_write_start();
  i2c_write_byte(i2c_address << 1);
  i2c_write_byte(0x00);
  for (int i = 0;i < 25;i++) i2c_write_byte(initCommands[i]);
  i2c_write_stop();
}

//####################################################//

void ssd1306_command(uint8_t command){
  i2c_write_start();
  i2c_write_byte(i2c_address << 1);
  i2c_write_byte(0x00);
  i2c_write_byte(command);
  i2c_write_stop();
}

//####################################################//

void ssd1306_text8x8(uint8_t x, uint8_t y, char *text){
  int c = x +(y*128);
  while(*text){
    uint8_t *pByte = CHAR(*text);
    for (int i = 0; i < 8; i++) db[c++] = pgm_read_byte(pByte++);
    text++;
  }
} 

//####################################################//

void ssd1306_clearBuffer(){
  memset(db,0,1024);
}

//####################################################//

//sets/clears the pixel at x,y 
void ssd1306_pixel(uint8_t x, uint8_t y, bool on ){// x = 0..127 y = 0..63
  int index = (int)(y/8)*128 + x; 
  if (on) db[index] |= 1 << y%8;
  else db[index] &= ~(1 << y%8);
}

//####################################################//

//draw/erase the line from bx,by to ex,ey 
void ssd1306_drawLine(float bx, float by, float ex, float ey, bool draw){
//void ssd1306_drawLine(int bx, int by, int ex, int ey, int draw){
  float x, y, dx, dy, stp;
  int i;
  dx = (ex - bx);
  dy = (ey - by);
  if (abs(dx) >= abs(dy))
    stp = abs(dx);
  else
    stp = abs(dy);
  dx = dx / stp;
  dy = dy / stp;
  x = bx;
  y = by;
  i = 1;
  while(i <= stp) {
    ssd1306_pixel(x,y,draw);
    x = x + dx;
    y = y + dy;
    i = i + 1;
  }
} 

//####################################################//

// Writes the 1024 bytes buffer to the display
void ssd1306_displayBuffer(){
  ssd1306_command(0xB0);
  ssd1306_command(0x00);
  ssd1306_command(0x10);
  i2c_write_start();
  i2c_write_byte(i2c_address << 1);
  i2c_write_byte(0x40);
  for (int i = 0; i < 1024; i++)
    i2c_write_byte(db[i]);  
  i2c_write_stop();
} 
