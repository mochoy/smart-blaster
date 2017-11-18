#ifndef SmartBlaster_h
#define SmartBlaster_h

#include "Arduino.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Button.h>


class SmartBlaster {
  private:
    //flags to know which modes are enabled
    uint8_t _isAmmoCounter;
    uint8_t _isIRGate;
    uint8_t _isChrono;
    uint8_t _isVoltmeter;
    uint8_t _isSelectFire;

    uint8_t _IR_RECEIVER_PIN;

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

    //keep track of buttons
    Button _swCntBtn;
    Button _reloadBtn;
    Button _magSzTogBtn;

    //keep track of chrono vals
    uint32_t _chronoVal;
    uint32_t _firstTripTime;
    uint32_t _secondTripTime;

    //init methods
    void initMagSizes(uint8_t magSizes[]);

    void countAmmo();

    uint32_t calculateChronoReading();
    void resetChronoVals();

    //display methods
    void initDisplay();     //initialize display right when it's created
    void initAmmoForDisplay(bool toPrint);
    void printVals();   //print all values at the same time, including ammo, chrono vals, etc.

  public:
    SmartBlaster(uint8_t magSizes[]);
    void init (bool isAmmoCounter, bool isIRGate, bool isChrono, bool isVoltMeter, bool isSelectFire);
    void smartMyBlaster();

    //smart blaster funcitonality methods
    uint8_t ammoCounter ();
    uint8_t reload ();  //reload
    uint8_t toggleMagSizes ();
    uint32_t chrono ();


};

#endif
