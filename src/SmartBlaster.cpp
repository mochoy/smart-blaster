#include "Arduino.h"
#include "SmartBlaster.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Button.h>

#define SW_CNT_BTN_PIN 4
#define RELOAD_BTN_PIN 7
#define MAG_SZ_TOG_BTN_PIN 8
#define REV_TRIG_BTN_PIN 12
#define PULLUP true
#define INVERT true
#define DEBOUNCE 20

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
#define DART_LEGNTH_FEET 2.83465
#define IR_MAP_TRIP_VAL 90
#define PWM_MAPPED_MAX_OUTPUT_THRESHOLD 16


SmartBlaster::SmartBlaster () :
  _display(OLED_RESET),
  _swCntBtn(SW_CNT_BTN_PIN, PULLUP, INVERT, DEBOUNCE),
  _reloadBtn(RELOAD_BTN_PIN, PULLUP, INVERT, DEBOUNCE),
  _magSzTogBtn(MAG_SZ_TOG_BTN_PIN, PULLUP, INVERT, DEBOUNCE),
  _revTrigBtn (REV_TRIG_BTN_PIN, PULLUP, INVERT, DEBOUNCE)  {

    resetChronoVals();
    _chronoVal = 0;

    _hasFlywheelsAccelerated = false;
    _flywheelAccelStartTime = 0;
}

void SmartBlaster::init(uint8_t modes[], uint8_t magSizes[], uint8_t pins[], uint8_t otherOptions[]) {
  initModes(modes);
  initMagSizes(magSizes);
  initPins(pins);
  initDisplay();

  initAmmoForDisplay();
  initChronoValForDisplay(false);
}

void SmartBlaster::smartMyBlaster() {
  ammoCounter();
  chrono();
  PWM();
}

uint8_t SmartBlaster::getAmmo () {
  if (_isSwitchAmmoCounter || _isIRGateAmmoCounter) {
    return _currentAmmo;
  }
}

uint32_t SmartBlaster:: getChrono () {
  if (_isChrono) {
    return _chronoVal;
  }
}

uint8_t SmartBlaster::getFlywheelPWM () {
  if (_isFlywheelPWM) {
    return analogRead(_FLYWHEEL_PWM_OUT_PIN) * 4;
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
  _isFlywheelPWM = modes[3];
}

void SmartBlaster::initMagSizes (uint8_t magSizes[]) {
  _magSizes = magSizes;
  _currentMagSize = 0;
  _maxAmmo = _magSizes[_currentMagSize];
  _currentAmmo = _maxAmmo;
}

void SmartBlaster::initPins (uint8_t pins[]) {
  _IR_GATE_PIN = pins[0];
  _FLYWHEEL_PWM_POT_PIN = pins[1];
  _FLYWHEEL_PWM_OUT_PIN = pins[2];
}

void SmartBlaster::initOtherOptions (uint8_t otherOptions[]) {
  _FLYWHEEL_MOTOR_ACCEL_TIME = otherOptions[0];
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



void SmartBlaster::PWM () {
  if (_isFlywheelPWM) {
    Button& trigBtn = _isFlywheelPWM ? _revTrigBtn : _revTrigBtn;


    trigBtn.read();
    if(trigBtn.isPressed() && !_hasFlywheelsAccelerated) {           //when trigger first pressed
      digitalWrite(_FLYWHEEL_PWM_OUT_PIN, HIGH);                         //motor at full power
      if (_flywheelAccelStartTime == 0) {
        _flywheelAccelStartTime = millis();
      }
    } else if (trigBtn.isPressed() && _hasFlywheelsAccelerated) {    //if trigger pressed
      analogWrite(_FLYWHEEL_PWM_OUT_PIN, analogRead(_FLYWHEEL_PWM_POT_PIN)/4);    //write PWM depending on pot value
    } else if (trigBtn.wasReleased()) {                     //when trigger released
      digitalWrite(_FLYWHEEL_PWM_OUT_PIN, LOW);                          //turn motor off
      _hasFlywheelsAccelerated = false;                                  //reset flag to check for acceleration
    }

    checkFinishAccel();
    initDisplayPWM();
  }
}

void SmartBlaster::checkFinishAccel () {
    if ( (_flywheelAccelStartTime > 0) && (millis() > _flywheelAccelStartTime + _FLYWHEEL_MOTOR_ACCEL_TIME) ) {       //passed accel time
      _hasFlywheelsAccelerated = true;
      _flywheelAccelStartTime = 0;
    }
}



//helper function to display ammo. Initializes value to be passed displayed on display
void SmartBlaster::initAmmoForDisplay () {
  _ammoToPrint = (_currentAmmo < 10 ? "0" : "") + (String)_currentAmmo;    //determine whether to insert 0 at the beginning of ammo
   printVals();   //print vals based on whether to print them from this method
}

void SmartBlaster::initChronoValForDisplay (uint8_t err) {
  if (err) {
    _chronoToPrint = "ERR";
  } else if (_chronoVal == 0) {
    _chronoToPrint = "NO FPS";
  } else if (!err) {
    _chronoToPrint = _chronoVal + " fps";
  }

  printVals();
}

void SmartBlaster::initDisplayPWM () {
  uint8_t mappedPWMReading = map(analogRead(_FLYWHEEL_PWM_POT_PIN), 0, 1010, 0, PWM_MAPPED_MAX_OUTPUT_THRESHOLD);
  if (mappedPWMReading != _lastFlywheelPWMPotReading) {
    _lastFlywheelPWMPotReading = mappedPWMReading;
    printVals();
  }
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
    _display.setCursor(7, 55);
    _display.print(_chronoToPrint);
  }

  if (_isFlywheelPWM) {
    uint8_t lineLength = 64 - _lastFlywheelPWMPotReading * 4;
    _display.drawLine(0, 63, 0, lineLength, WHITE);
    _display.drawLine(1, 63, 1, lineLength, WHITE);
  }
  

  _display.display(); //display the text

}
