#include "Arduino.h"
#include "SmartBlaster.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Button.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4

#define PULLUP false
#define INVERT false
#define DEBOUNCE 20

SmartBlaster::SmartBlaster (uint8_t magSizes[]) :
  _display(OLED_RESET),
  _triggerBtnArg(4, PULLUP, INVERT, DEBOUNCE),
  _magInsDetBtn(7, PULLUP, INVERT, DEBOUNCE),
  _magSzTogBtn(8, PULLUP, INVERT, DEBOUNCE)  {
    _isChrono = true;
    _isVoltmeter = true;
    _isSelectFire = true;

    _ammoToPrint = "16";
    _chronoToPrint = "109.3";
    _voltageToPrint = "8.2";
    _fireModeToPrint = "SS";

    initMagSizes(magSizes);
}

SmartBlaster SmartBlaster::init() {
    initDisplay();
    printVals();

    digitalWrite(12, HIGH);

    return *this;
}

SmartBlaster SmartBlaster::smartMyBlaster(
    Button triggerBtnArg,
    Button magInsDetBtn,
    Button magSzTogBtn) {

    reload(magInsDetBtn, _display).toggleMagSizes(magSzTogBtn, _display);

    return *this;
}



SmartBlaster SmartBlaster::initMagSizes (uint8_t magSizes[]) {
        _numOfMagSizes = (sizeof(magSizes)/sizeof(magSizes[0]));
        for (int i = 0; i < _numOfMagSizes; i++) {
            _magSizes[i] = magSizes[i];
        }

        _currentMagSize = 0;
        _maxAmmo = _magSizes[_currentMagSize];
        _currentAmmo = _maxAmmo;

        return *this;
}

SmartBlaster SmartBlaster::initDisplay () {
    _display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    _display.clearDisplay();

    return *this;
}


//helper function to display ammo. Initializes value to be passed displayed on display
SmartBlaster SmartBlaster::initAmmoForDisplay (Adafruit_SSD1306 displayArg) {
        String textToDisplay = "00";

        if (_currentAmmo < 10) {
                textToDisplay = "0" + (String)_currentAmmo;     //put leading '0' if ammo less than 10
        } else {
                textToDisplay = (String)_currentAmmo;       //convert int to string
        }

        _ammoToPrint = textToDisplay;       //so the display knows what to display

        printVals();

        return *this;
}

SmartBlaster SmartBlaster::printVals() {
    _display.clearDisplay(); //clear the display, so the stuff that was here before is no longer here
    _display.setTextSize(6);  //set the size of the text
    _display.setTextColor(WHITE);    //set the color of text text
    //tell the display where to draw the text
    _display.setCursor( (SCREEN_WIDTH/2) - ((_ammoToPrint.length()*2) * 9) , (SCREEN_HEIGHT/2) - 30 );  //center text
    _display.print(_ammoToPrint);    //print the text

    _display.setTextSize(1);

    //display chrono values
    if (_isChrono) {
        _display.setCursor(0, 50);
        _display.print(_chronoToPrint);
    }

    //display voltage values
    if (_isVoltmeter) {
        _display.setCursor(60, 50);
        _display.print(_voltageToPrint);
    }

    //display fire mode
    if (_isSelectFire) {
        _display.setCursor(100, 50);
        _display.print(_fireModeToPrint);
    }

    _display.display(); //display the text

    return *this;
}

//method to deal with reloading
SmartBlaster SmartBlaster::reload (Button magInsDetBtn, Adafruit_SSD1306 displayArg) {
    magInsDetBtn.read();    //read button, using Button library

    //if button pressed, reload
    if (magInsDetBtn.wasPressed()) {
            _currentAmmo = _maxAmmo;
            initAmmoForDisplay(displayArg);      //display new ammo
    }

    return *this;
}

//toggle between magazine sizes
SmartBlaster SmartBlaster::toggleMagSizes (Button magSzTogBtn, Adafruit_SSD1306 displayArg) {
    magSzTogBtn.read(); //read button, using Button library

    //if button was pressed, toggle size
    if (magSzTogBtn.wasPressed()) {
    //make sure mag sizes dont overflow
    //if it does, it goes back to the first mag size
    //if it doesn't, then goes to next mag size
    //if it doesn't, then goes to next mag size
    _currentMagSize = (_currentMagSize == _numOfMagSizes - 1) ? 0 : _currentMagSize + 1;
    _maxAmmo = _magSizes[_currentMagSize];

    }

    return *this;
}
