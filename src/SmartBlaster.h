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
    uint8_t _isFlywheelPWM;
    uint8_t _isPusherPWM;
    uint8_t _isSelectFire;

    //pins
    uint8_t _IR_GATE_PIN;
    uint8_t _FLYWHEEL_PWM_POT_PIN;
    uint8_t _FLYWHEEL_OUT_PIN;
    uint8_t _PUSHER_PWM_POT_PIN;
    uint8_t _PUSHER_OUT_PIN;

    //display to print stuff on
    Adafruit_SSD1306 _display;

    //keep track of buttons
    Button _swCntBtn;
    Button _reloadBtn;
    Button _magSzTogBtn;
    Button _revTrigBtn;
    Button _mainTrigBtn;
    Button _fireModeTogBtn;

    //stuff to keep track of what to print for each different value
    String _ammoToPrint;
    String _chronoToPrint;

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

    //PWM vals
    //for flywheel
    uint32_t _FLYWHEEL_MOTOR_ACCEL_TIME;
    uint8_t _hasFlywheelsAccelerated;
    uint32_t _flywheelAccelStartTime;
    uint8_t _lastFlywheelPWMPotReading;
    //for pusher
    uint32_t _PUSHER_MOTOR_ACCEL_TIME;
    uint8_t _hasPusherAccelerated;
    uint32_t _pusherAccelStartTime;
    uint8_t _lastPusherPWMPotReading;

    //select-fire vals
    uint8_t _fireMode;
    uint8_t _dartsFiredForSelectFire;
    uint8_t _isCheckingForDartsFired;

    //init methods upon instantiation
    void initDisplay();
    void initModes(uint8_t modes[]);
    void initMagSizes(uint8_t magSizes[]);
    void initPins(uint8_t pins[]);
    void initOtherOptions(uint32_t otherOptions[]);

    //smart blaster funcitonality methods
    void ammoCounter();
    void switchAmmoCounter();
    void IRGateAmmoCounter();
    void countAmmo();
    void reload();  
    void toggleMagSizes();

    void chrono();
    void calculateChronoReading();
    void resetChronoVals();

    void PWM(uint8_t toPWM);
    void checkFinishAccel(uint8_t toPWM, uint32_t& accelStartTime, uint8_t& hasAccelerated, uint32_t MOTOR_ACCEL_TIME);

    void selectFire();
    void toggleFireModes();

    void initAmmoForDisplay();
    void initChronoValForDisplay(uint8_t err);
    void initDisplayPWM(uint8_t POT_PIN, uint8_t& lastPotReading);
    void printVals();   //print all values at the same time, including ammo, chrono vals, etc.

  public:
    SmartBlaster();
    void init (uint8_t modes[], uint8_t magSizes[], uint8_t pins[], uint32_t otherOptions[]);
    void smartMyBlaster();

    //getters
    uint8_t getAmmo();
    uint32_t getChrono();
    uint16_t getFlywheelPWM();
    uint16_t getPusherPWM();


};

#endif
