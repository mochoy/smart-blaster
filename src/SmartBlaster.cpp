#include "Arduino.h"
#include "SmartBlaster.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Button.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

SmartBlaster::SmartBlaster () {
    _isChrono = true;
    _isVoltmeter = true;
    _isSelectFire = true;
    
    _ammoToPrint = "16";
    _chronoToPrint = "109.3";
    _voltageToPrint = "8.2";
    _fireModeToPrint = "SS";
}

SmartBlaster SmartBlaster::smartMyBlaster(Adafruit_SSD1306 displayArg) {
    _printVals(displayArg);
}



SmartBlaster SmartBlaster::_initDisplay (Adafruit_SSD1306 displayArg) {
    displayArg.clearDisplay();
}

SmartBlaster SmartBlaster::_printVals(Adafruit_SSD1306 displayArg) {
    displayArg.clearDisplay(); //clear the display, so the stuff that was here before is no longer here
    displayArg.setTextSize(6);  //set the size of the text
    displayArg.setTextColor(WHITE);    //set the color of text text
    //tell the display where to draw the text
    displayArg.setCursor( (SCREEN_WIDTH/2) - ((_ammoToPrint.length()*2) * 9) , (SCREEN_HEIGHT/2) - 30 );  //center text
    displayArg.print(_ammoToPrint);    //print the text

    displayArg.setTextSize(1);

    //display chrono values
    if (_isChrono) {
        displayArg.setCursor(0, 50);  
        displayArg.print(_chronoToPrint);
    }

    //display voltage values
    if (_isVoltmeter) {
        displayArg.setCursor(60, 50);
        displayArg.print(_voltageToPrint);
    }

    //display fire mode
    if (_isSelectFire) {
        displayArg.setCursor(100, 50);  
        displayArg.print(_fireModeToPrint);
    }   
  
    displayArg.display(); //display the text
}


