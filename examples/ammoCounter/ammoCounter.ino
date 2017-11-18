#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Button.h>
#include <SmartBlaster.h>

#define OLED_RESET 4

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

byte modes[] = {false, false, false, false};
byte pins[] = {1, 2, 3, 255, 255, 255};
byte magSizes[] = {5, 6, 8, 10, 12, 15, 18, 19, 20, 22, 25, 36, 0};

SmartBlaster smartBlaster(magSizes);


void setup() {

}

void loop() {
  // put your main code here, to run repeatedly:
  // smartBlaster.smartMyBlaster();

}
