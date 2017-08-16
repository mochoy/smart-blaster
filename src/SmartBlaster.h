#ifndef SmartBlaster_h
#define SmartBlaster_h

#include "Arduino.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Button.h>

class SmartBlaster {

  public:
        //constructor
    SmartBlaster();

    Adafruit_SSD1306 _initDisplay(Adafruit_SSD1306 displayArg);


};

#endif

