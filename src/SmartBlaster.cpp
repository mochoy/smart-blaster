#include "Arduino.h"
#include "SmartBlaster.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Button.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

SmartBlaster::SmartBlaster (uint8_t magSizes[]) {
    _isChrono = true;
    _isVoltmeter = true;
    _isSelectFire = true;

    _ammoToPrint = "16";
    _chronoToPrint = "109.3";
    _voltageToPrint = "8.2";
    _fireModeToPrint = "SS";

        initMagSizes(magSizes);
}

SmartBlaster SmartBlaster::init(Adafruit_SSD1306 displayArg) {
    initDisplay(displayArg);

        return *this;
}

SmartBlaster SmartBlaster::smartMyBlaster(Adafruit_SSD1306 displayArg, Button triggerBtnArg, Button magInsDetArg, Button magSzTogBtnArg) {
    reload(magInsDetArg);
    printVals(displayArg);

        return *this;
}



SmartBlaster SmartBlaster::initMagSizes (uint8_t magSizes[]) {
        numOfMagSizes = (sizeof(magSizes)/sizeof(magSizes[0]));
        for (int i = 0; i < numOfMagSizes; i++) {
            _magSizes[i] = magSizes[i];
        }

        _currentMagSize = 0;
        _maxAmmo = _magSizes[_currentMagSize];
        _currentAmmo = _maxAmmo;

        return *this;
}

SmartBlaster SmartBlaster::initDisplay (Adafruit_SSD1306 displayArg) {
    displayArg.clearDisplay();

    return *this;
}

SmartBlaster SmartBlaster::printVals(Adafruit_SSD1306 displayArg) {
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

    return *this;
}

//method to deal with reloading
SmartBlaster SmartBlaster::reload (Button magInsDetArg) {
    magInsDetArg.read();    //read button, using Button library

    //if button pressed, reload
    if (magInsDetArg.wasPressed()) {

    }

        return *this;
}

