#ifndef SmartBlaster_h
#define SmartBlaster_h

#include "Arduino.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Button.h>

class SmartBlaster {
  private:
    bool _isChrono;
    bool _isVoltmeter;
    bool _isSelectFire;
  
    String _ammoToPrint;
    String _chronoToPrint;
    String _voltageToPrint;
    String _fireModeToPrint;
    
  public:
        //constructor
    SmartBlaster();

    

    SmartBlaster _initDisplay(Adafruit_SSD1306 displayArg);
    SmartBlaster _printVals(Adafruit_SSD1306 displayArg);

};

#endif

