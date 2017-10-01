#include "Arduino.h"
#include "SmartBlaster.h"

#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <Button.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4

#define PULLUP false
#define INVERT false
#define DEBOUNCE 20

SmartBlaster::SmartBlaster (uint8_t magSizes[]) :
  _u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE),
  _triggerBtnArg(4, PULLUP, INVERT, DEBOUNCE),
  _magInsDetBtn(7, PULLUP, INVERT, DEBOUNCE),
  _magSzTogBtn(8, PULLUP, INVERT, DEBOUNCE)  {

    _isChrono = false;
    _isVoltmeter = false;
    _isSelectFire = false;

    initMagSizes(magSizes);
}

SmartBlaster SmartBlaster::init() {
	Serial.println("initing!");

	initDisplay();
	initAmmoForDisplay(true);

    return *this;
}

SmartBlaster SmartBlaster::smartMyBlaster() {
    // reload().toggleMagSizes();

    return *this;
}



void SmartBlaster::initMagSizes (uint8_t magSizes[]) {
    _magSizes = magSizes;

	_currentMagSize = 0;
	_maxAmmo = _magSizes[_currentMagSize];
	_currentAmmo = _maxAmmo;
}

void SmartBlaster::initDisplay () {
    _u8g2.begin();
    _u8g2.clearDisplay();
}

//helper function to display ammo. Initializes value to be passed displayed on display
void SmartBlaster::initAmmoForDisplay (bool toPrint) {
    String ammoToPrintBuffer = (_currentAmmo < 10 ? "0" : "") + (String)_currentAmmo;    //determine whether to insert 0 at the beginning of ammo

    Serial.print("ammo is ");
    Serial.println(ammoToPrintBuffer);

    ammoToPrintBuffer.toCharArray(_ammoToPrint, 3);	 //string to char arr

    if (toPrint) printVals();   //print vals based on whether to print them from this method 
}

void SmartBlaster::printVals() {
    _u8g2.firstPage();   //keep track of pages
    do {
        _u8g2.setFont(u8g2_font_ncenB10_tr);   //select font
        _u8g2.drawUTF8(0, 24, _ammoToPrint);    //draw text at certain coordiantes
    } while (_u8g2.nextPage()); //keep track of pages

    // _display.clearDisplay(); //clear the display, so the stuff that was here before is no longer here
    // _display.setTextSize(6);  //set the size of the text
    // _display.setTextColor(WHITE);    //set the color of text text
    // //tell the display where to draw the text
    // _display.setCursor( (SCREEN_WIDTH/2) - ((_ammoToPrint.length()*2) * 9) , (SCREEN_HEIGHT/2) - 30 );  //center text
    // _display.print(_ammoToPrint);    //print the text


    // _display.setTextSize(1);

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

    // _display.display(); //display the text
}

//method to deal with reloading
SmartBlaster SmartBlaster::reload () {
    _magInsDetBtn.read();    //read button, using Button library

    //if button pressed, reload
    if (_magInsDetBtn.wasPressed()) {
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
