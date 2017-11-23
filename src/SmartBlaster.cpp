#include "Arduino.h"
#include "SmartBlaster.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Button.h>

#define SW_CNT_BTN_PIN 4
#define RELOAD_BTN_PIN 7
#define MAG_SZ_TOG_BTN_PIN 8
#define PULLUP true
#define INVERT true
#define DEBOUNCE 20

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
#define DART_LEGNTH_FEET 2.83465
#define IR_MAP_TRIP_VAL 90


SmartBlaster::SmartBlaster () :
  _display(OLED_RESET),
  _swCntBtn(SW_CNT_BTN_PIN, PULLUP, INVERT, DEBOUNCE),
  _reloadBtn(RELOAD_BTN_PIN, PULLUP, INVERT, DEBOUNCE),
  _magSzTogBtn(MAG_SZ_TOG_BTN_PIN, PULLUP, INVERT, DEBOUNCE)  {

    resetChronoVals();
    _chronoVal = 0;
}

void SmartBlaster::init(uint8_t modes[], uint8_t magSizes[], uint8_t pins[]) {
  initModes(modes);
  initMagSizes(magSizes);
  initDisplay();

  initAmmoForDisplay();
  initChronoValForDisplay(false);
}

void SmartBlaster::smartMyBlaster() {
  ammoCounter();
}

uint8_t SmartBlaster::getAmmo () {
  if (_isSwitchAmmoCounter || _isIRGateAmmoCounter) {
    return _currentAmmo;
  }
}





void SmartBlaster::initDisplay () {
  _display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  _display.clearDisplay();
}

void SmartBlaster::initModes (uint8_t modes[]) {
  _isSwitchAmmoCounter = modes[0];
  _isIRGateAmmoCounter = modes[1];
  _isChrono = modes[2];
}

void SmartBlaster::initMagSizes (uint8_t magSizes[]) {
  _magSizes = magSizes;
  _currentMagSize = 0;
  _maxAmmo = _magSizes[_currentMagSize];
  _currentAmmo = _maxAmmo;
}

void SmartBlaster::initPins (uint8_t pins[]) {
  _IR_GATE_PIN = pins[0];
}


//toggle between magazine sizes
void SmartBlaster::ammoCounter () {
  if (_isSwitchAmmoCounter) {
    switchAmmoCounter();
  } else if (_isIRGateAmmoCounter && !_isChrono) {
    IRGateAmmoCounter();
  } 
  reload();
  toggleMagSizes();
}

void SmartBlaster::switchAmmoCounter () {
  _swCntBtn.read();
  if (_swCntBtn.wasPressed()) {
    countAmmo();
  }
}

void SmartBlaster::IRGateAmmoCounter () {
  if (map(analogRead(_IR_GATE_PIN), 0, 1023, 0, 100) > IR_MAP_TRIP_VAL) {
    countAmmo();
  }
}

void SmartBlaster::countAmmo () {
  if (_maxAmmo != 0 && _currentAmmo < 99 && _currentAmmo > 0) {  //make sure that the ammo is less than 99 so it doesnt overflow the display and not in count-up mode
    _currentAmmo--;    //increment ammo
  } else if (_maxAmmo == 0 && _currentAmmo < 255) { //make sure ammo doesn't overflow uint_8 max val
    _currentAmmo++;    //decrement ammo
  }

  initAmmoForDisplay();
}

void SmartBlaster::reload () {
  _reloadBtn.read();    //read button, using Button library
  //if button pressed, reload
  if (_reloadBtn.wasPressed()) {
    // Serial.println("reloading!");
    _currentAmmo = _maxAmmo;
    initAmmoForDisplay();      //display new ammo
  }
}

//toggle between magazine sizes
void SmartBlaster::toggleMagSizes () {
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
    initAmmoForDisplay();

  }
}



void SmartBlaster::chrono () {
  if (_isChrono) {
    if (map(analogRead(_IR_GATE_PIN), 0, 1023, 0, 100) > IR_MAP_TRIP_VAL) {
      if (_firstTripTime == -10 && _secondTripTime == -10) {
        _firstTripTime = micros();
      } else if (_firstTripTime != -10 && _secondTripTime == -10) {
        _secondTripTime = micros();
        resetChronoVals();
        if (_isIRGateAmmoCounter) {
          countAmmo();
        }
        calculateChronoReading();
        initChronoValForDisplay(false);
      }
    } else if ( (micros() > _firstTripTime + 1000000 && _secondTripTime != -10) || (_firstTripTime > _secondTripTime)  ) {
      resetChronoVals();
      initChronoValForDisplay(true);
    }
  }
}

void SmartBlaster::calculateChronoReading () {
  _chronoVal = (DART_LEGNTH_FEET) / ((_secondTripTime-_firstTripTime)/1000000.0);
  resetChronoVals();
}

void SmartBlaster::resetChronoVals () {
  _firstTripTime = -10;
  _secondTripTime = -10;
}




//helper function to display ammo. Initializes value to be passed displayed on display
void SmartBlaster::initAmmoForDisplay () {
  _ammoToPrint = (_currentAmmo < 10 ? "0" : "") + (String)_currentAmmo;    //determine whether to insert 0 at the beginning of ammo
   printVals();   //print vals based on whether to print them from this method
}

void SmartBlaster::initChronoValForDisplay (uint8_t err) {
  if (!err) {
    _chronoToPrint = _chronoVal + " fps";
  } else if (err) {
    _chronoToPrint = "ERR";
  } else if (_chronoVal == 0) {
    _chronoToPrint = "NO FPS";
  }
  printVals();
}

void SmartBlaster::printVals() {
  _display.clearDisplay(); //clear the display, so the stuff that was here before is no longer here
  _display.setTextColor(WHITE);    //set the color of text

  if (_isSwitchAmmoCounter || _isIRGateAmmoCounter) {
    _display.setTextSize(7);  //set the size of the text
    _display.setCursor(30, 0);  //center text
    _display.print(_ammoToPrint);    //print the text
  }

  _display.setTextSize(1);

  if (_isChrono) {
    _display.setCursor(0, 50);
    _display.print(_chronoToPrint);
  }
  

  _display.display(); //display the text

}
