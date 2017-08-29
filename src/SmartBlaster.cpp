#include "Arduino.h"
#include "SmartBlaster.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Button.h>

#include <StandardCplusplus.h>
#include <vector>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4

#define PULLUP false
#define INVERT false
#define DEBOUNCE 20

SmartBlaster::SmartBlaster (int magSizes[], int numOfMagSizes) :
  _display(OLED_RESET),
  _triggerBtnArg(4, PULLUP, INVERT, DEBOUNCE),
  _magInsDetBtn(7, PULLUP, INVERT, DEBOUNCE),
  _magSzTogBtn(8, PULLUP, INVERT, DEBOUNCE)  {

    _isChrono = false;
    _isVoltmeter = false;
    _isSelectFire = false;

    initMagSizes(magSizes, numOfMagSizes);

    // _ammoToPrint = "7";
    // _chronoToPrint = "109.3";
    // _voltageToPrint = "8.2";
    // _fireModeToPrint = "SS";
}

SmartBlaster SmartBlaster::init() {
    _display.clearDisplay();
    initAmmoForDisplay(false);
    // printVals();

    return *this;
}

SmartBlaster SmartBlaster::smartMyBlaster() {
    reload().toggleMagSizes();

    return *this;
}



SmartBlaster SmartBlaster::initMagSizes (int magSizes[], int numOfMagSizes) {
      _numOfMagSizes = numOfMagSizes;
      for (int i = 0; i < _numOfMagSizes; i++) {
          _magSizes.push_back(magSizes[i]);
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
SmartBlaster SmartBlaster::initAmmoForDisplay (bool toPrint) {
    // Serial.println("initing ammo!");   //for debugging

    _ammoToPrint = "00";    //default value

    if (_currentAmmo < 10) {
        _ammoToPrint = "0" + (String)_currentAmmo;     //put leading '0' if ammo less than 10
    } else {
        _ammoToPrint = (String)_currentAmmo;       //convert int to string
    }

    Serial.println(_ammoToPrint);

    if (toPrint) {
      printVals();
    }

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
    // if (_isChrono) {
    //     _display.setCursor(0, 50);
    //     _display.print(_chronoToPrint);
    // }
    //
    // //display voltage values
    // if (_isVoltmeter) {
    //     _display.setCursor(60, 50);
    //     _display.print(_voltageToPrint);
    // }
    //
    // //display fire mode
    // if (_isSelectFire) {
    //     _display.setCursor(100, 50);
    //     _display.print(_fireModeToPrint);
    // }

    _display.display(); //display the text

    return *this;
}

//method to deal with reloading
SmartBlaster SmartBlaster::reload () {
    _magInsDetBtn.read();    //read button, using Button library

    //if button pressed, reload
    if (_magInsDetBtn.wasPressed()) {
        Serial.println("Reloading!");
        _currentAmmo = _maxAmmo;
        initAmmoForDisplay(true);      //display new ammo
    }

    return *this;
}

//toggle between magazine sizes
SmartBlaster SmartBlaster::toggleMagSizes () {
    _magSzTogBtn.read(); //read button, using Button library

    //if button was pressed, toggle size
    if (_magSzTogBtn.wasPressed()) {
      //make sure mag sizes dont overflow
      //if it does, it goes back to the first mag size
      //if it doesn't, then goes to next mag size
      //if it doesn't, then goes to next mag size
      _currentMagSize = (_currentMagSize == _numOfMagSizes - 1) ? 0 : _currentMagSize + 1;
      _maxAmmo = _magSizes[_currentMagSize];
      _currentAmmo = _maxAmmo;
      initAmmoForDisplay(true);

    }

    return *this;
}
