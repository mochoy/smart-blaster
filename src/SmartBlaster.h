#ifndef SmartBlaster_h
#define SmartBlaster_h

#include "Arduino.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Button.h>


class SmartBlaster {
  private:
    //flags to know which modes are enabled
    uint8_t _isSwitchAmmoCounter;
    uint8_t _isIRGateAmmoCounter;
    uint8_t _isChrono;

    //pins
    uint8_t _IR_GATE_PIN;

    //display to print stuff on
    Adafruit_SSD1306 _display;

    //keep track of buttons
    Button _swCntBtn;
    Button _reloadBtn;
    Button _magSzTogBtn;

    //stuff to keep track of what to print for each different value
    String _ammoToPrint;

    //keep track of ammo
    uint8_t *_magSizes;
    uint8_t _numOfMagSizes;
    uint8_t _currentMagSize;
    uint8_t _maxAmmo;
    uint8_t _currentAmmo;

    //keep track of chrono vals
    uint32_t _chronoVal;
    int32_t _firstTripTime;
    int32_t _secondTripTime;

    //init methods upon instantiation
    void initDisplay();
    void initModes(uint8_t modes[]);
    void initMagSizes(uint8_t magSizes[]);
    void initPins(uint8_t pins[]);

    //smart blaster funcitonality methods
    void ammoCounter();
    void switchAmmoCounter();
    void IRGateAmmoCounter();
    void countAmmo();
    void reload();  
    void toggleMagSizes();
    void chrono();
    void resetChronoVals();

    void initAmmoForDisplay();
    void printVals();   //print all values at the same time, including ammo, chrono vals, etc.

  public:
    SmartBlaster();
    void init (uint8_t modes[], uint8_t magSizes[], uint8_t pins[]);
    void smartMyBlaster();

    //getters
    uint8_t getAmmo();


};

#endif
