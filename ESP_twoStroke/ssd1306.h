#ifndef SSD1306_H
#define SSD1306_H

#define SSD1306_LCDWIDTH       128
#define SSD1306_LCDHEIGHT       64
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF    0xAE
#define SSD1306_DISPLAYON     0xAF


void ssd1306_init(uint8_t sda=4, uint8_t scl=5, uint8_t address = 0x3C);
void ssd1306_command(uint8_t command);
void ssd1306_pixel(uint8_t x, uint8_t y, bool on = true);// x = 0..127 y = 0..63
void ssd1306_drawLine(float bx, float by, float ex, float ey, bool draw=true);
void ssd1306_text8x8(uint8_t x, uint8_t y, char *text);
void ssd1306_displayBuffer();
void ssd1306_clearBuffer();

#endif
