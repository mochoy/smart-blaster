//display libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

//button library
#include <Button.h>

//smart blaster library - make your blaster smarter
#include <SmartBlaster.h>


void setup() {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void loop() {
    
}

