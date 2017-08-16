#include "Arduino.h"
#include "SmartBlaster.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Button.h>

SmartBlaster::SmartBlaster () {

}

Adafruit_SSD1306 SmartBlaster::_initDisplay (Adafruit_SSD1306 displayArg) {
    displayArg.clearDisplay();
    displayArg.setTextSize(2);
    displayArg.setTextColor(WHITE);
    displayArg.setCursor(0, 0);
    displayArg.print("Hello, world!");
    displayArg.display();
}

