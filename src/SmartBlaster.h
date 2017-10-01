#ifndef SmartBlaster_h
#define SmartBlaster_h

#include "Arduino.h"

#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

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

    U8G2_SSD1306_128X64_NONAME_F_SW_I2C _u8g2;

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
