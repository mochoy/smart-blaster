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

    //stuff to keep track of what to print for each different value
    String _ammoToPrint;

    //keep track of ammo
    uint8_t *_magSizes;
    uint8_t _numOfMagSizes;
    uint8_t _currentMagSize;
    uint8_t _maxAmmo;
    uint8_t _currentAmmo;

    //display to print stuff on
    Adafruit_SSD1306 _display;

    //keep track of buttons
    Button _swCntBtn;
    Button _reloadBtn;
    Button _magSzTogBtn;

    //init methods upon instantiation
    void initDisplay();

    //smart blaster funcitonality methods
    uint8_t ammoCounter ();
    uint8_t reload ();  //reload
    uint8_t toggleMagSizes ();

    void countAmmo();

    void initAmmoForDisplay();
    void printVals();   //print all values at the same time, including ammo, chrono vals, etc.

  public:
    SmartBlaster();
    void init (bool isAmmoCounter);
    void initMagSizes(uint8_t magSizes[]);
    void smartMyBlaster();


};

#endif
