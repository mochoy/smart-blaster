#ifndef SmartBlaster_h
#define SmartBlaster_h

#include "Arduino.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Button.h>
#include <Vector.h>

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
    // Vector<int> _magSizes;
    uint8_t _magSizes[];
    uint8_t _numOfMagSizes;
    uint8_t _currentMagSize;
    int _maxAmmo;
    int _currentAmmo;

    Adafruit_SSD1306 _display;

    Button _triggerBtnArg;
    Button _magInsDetBtn;
    Button _magSzTogBtn;

    //init methods
    SmartBlaster initMagSizes(int magSizes[]);

    //display methods
    SmartBlaster initDisplay();     //initialize display right when it's created
    SmartBlaster initAmmoForDisplay(bool toPrint);
    SmartBlaster printVals();   //print all values at the same time, including ammo, chrono vals, etc.

    //smart blaster funcitonality methods
    SmartBlaster reload ();  //reload
    SmartBlaster toggleMagSizes ();

  public:
    SmartBlaster(int magSizes[]);
    SmartBlaster init ();
    SmartBlaster smartMyBlaster();

};

#endif
