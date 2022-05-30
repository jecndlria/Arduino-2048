
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#define LCD_SCK 3
#define LCD_SDA 4
#define LCD_DCA0 5
#define LCD_RESET 6
#define LCD_CS 7

#define COLOR_2048_2 238,228,218
#define COLOR_2048_4 237,224,200
#define COLOR_2048_8 242,177,121
#define COLOR_2048_16 245,149,99
#define COLOR_2048_32 246,124,96
#define COLOR_2048_64 246,94,59
#define COLOR_2048_128 237,207,115
#define COLOR_2048_256 247,204,98
#define COLOR_2048_512 247,200,80
#define COLOR_2048_1024 237,197,63
#define COLOR_2048_2048 237,194,45

// Precomputed values are faster than ones calculated at runtime.
Adafruit_ST7735 tft = Adafruit_ST7735(LCD_CS, LCD_DCA0, LCD_SDA, LCD_SCK, LCD_RESET);
void initializeScreen()
{
  tft.initR(INITR_GREENTAB);   // initialize a ST7735S chip, black tab  //<----------------------Must Change Examples to this! 
}
short colorHelper(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) // pass in color constants!
{
  r = map(r, 0, 255, 0, 31);
  g = map(g, 0, 255, 0, 63);
  b = map(b, 0, 255, 0, 31);
  return (r << 11) | (g << 5) | b; // this is how the TFT library stores color: 5 bits red, 6 bits green, 5 bits blue.
}

// Precomputed values are faster than ones calculated at runtime.
short colorLookupTable[11] = 
{
  colorHelper(COLOR_2048_2),
  colorHelper(COLOR_2048_4),
  colorHelper(COLOR_2048_8),
  colorHelper(COLOR_2048_16),
  colorHelper(COLOR_2048_32),
  colorHelper(COLOR_2048_64),
  colorHelper(COLOR_2048_128),
  colorHelper(COLOR_2048_256),
  colorHelper(COLOR_2048_512),
  colorHelper(COLOR_2048_1024),
  colorHelper(COLOR_2048_2048)
}; 
void testScreen()
{
    tft.fillScreen(ST7735_BLACK);
    tft.fillRect(0, 0, 32, 32, colorLookupTable[0]);
    tft.fillRect(32, 0, 32, 32, colorLookupTable[1]);
    tft.fillRect(64, 0, 32, 32, colorLookupTable[2]);
    tft.fillRect(96, 0, 32, 32, colorLookupTable[3]);
    tft.setCursor(4,16);
    tft.setTextColor(ST7735_BLACK);
    tft.setTextSize(1);
    tft.print("2");
    tft.setCursor(4+32, 16);
    tft.print("4");
    tft.setCursor(4+64, 16);
    tft.print("8");
    tft.setCursor(4+96, 16);
    tft.print("16");
    tft.fillRect(0, 32, 32, 32, colorLookupTable[4]);
    tft.fillRect(32, 32, 32, 32, colorLookupTable[5]);
    tft.fillRect(64,32,32,32,colorLookupTable[6]);
    tft.fillRect(96,32,32,32,colorLookupTable[7]);
    tft.setCursor(4, 16+32);
    tft.print("32");
    tft.setCursor(4+32, 16+32);
    tft.print("64");
    tft.setCursor(4+64, 16+32);
    tft.print("128");
    tft.setCursor(4+96, 16+32);
    tft.print("256");
    tft.fillRect(0, 64, 32, 32, colorLookupTable[8]);
    tft.fillRect(32,64, 32, 32, colorLookupTable[9]);
    tft.fillRect(64,64,32,32, colorLookupTable[10]);
    tft.setCursor(4,16+64);
    tft.print("512");
    tft.setCursor(4+32,16+64);
    tft.print("1024");
    tft.setCursor(4+64,16+64);
    tft.print("2048");
}