#ifndef SmartBlaster_h
#define SmartBlaster_h

#include "Arduino.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Button.h>

class SmartBlaster {
  private:
    //flags to know which modes are enabled
    uint8_t _isChrono;
    uint8_t _isVoltmeter;
    uint8_t _isSelectFire;

    //stuff to keep track of what to print for each different value
    String _ammoToPrint;
    String _chronoToPrint;
    String _voltageToPrint;
    String _fireModeToPrint;

    //keep track of ammo
    uint8_t _magSizes[];
    uint8_t _numOfMagSizes;
    uint8_t _currentMagSize;
    uint8_t _maxAmmo;
    uint8_t _currentAmmo;

    Adafruit_SSD1306 _display;

    Button _triggerBtnArg;
    Button _magInsDetBtn;
    Button _magSzTogBtn;

    //init methods
    SmartBlaster initMagSizes(uint8_t magSizes[]);

    //display methods
    SmartBlaster initDisplay();     //initialize display right when it's created
    SmartBlaster initAmmoForDisplay(Adafruit_SSD1306 displayArg);
    SmartBlaster printVals();   //print all values at the same time, including ammo, chrono vals, etc.

    //smart blaster funcitonality methods
    SmartBlaster reload (Button magInsDetBtn, Adafruit_SSD1306 displayArg);  //reload
    SmartBlaster toggleMagSizes (Button magSzTogBtn, Adafruit_SSD1306 displayArg);

  public:
    SmartBlaster(uint8_t magSizes[]);
    SmartBlaster init ();
    SmartBlaster smartMyBlaster(Button triggerBtnArg, Button magInsDetArg, Button magSzTogBtnArg);

};

#endif
