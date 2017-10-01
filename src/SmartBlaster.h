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
    char _ammoToPrint[3];
    char _chronoToPrint[7];
    char _voltageToPrint[5];
    char _fireModeToPrint[3];

    //keep track of ammo
    uint8_t *_magSizes;
    uint8_t _numOfMagSizes;
    uint8_t _currentMagSize;
    uint8_t _maxAmmo;
    uint8_t _currentAmmo;

    Adafruit_SSD1306 _display;

    Button _triggerBtnArg;
    Button _magInsDetBtn;
    Button _magSzTogBtn;

    //init methods
    void initMagSizes(uint8_t magSizes[]);

    //display methods
    void initDisplay();     //initialize display right when it's created
    void initAmmoForDisplay(bool toPrint);
    void printVals();   //print all values at the same time, including ammo, chrono vals, etc.

    //smart blaster funcitonality methods
    SmartBlaster reload ();  //reload
    SmartBlaster toggleMagSizes ();

  public:
    SmartBlaster(uint8_t magSizes[]);
    SmartBlaster init ();
    SmartBlaster smartMyBlaster();

};

#endif
