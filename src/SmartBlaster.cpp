#include "Arduino.h"
#include "SmartBlaster.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Button.h>

#define SW_CNT_BTN_PIN 2
#define RELOAD_BTN_PIN 4
#define MAG_SZ_TOG_BTN_PIN 7
#define REV_TRIG_BTN_PIN 8
#define MAIN_TRIG_BTN_PIN 11
#define FIRE_MODE_TOG_BTN_PIN 13

#define PULLUP true
#define INVERT true
#define DEBOUNCE 20

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4

#define DART_LEGNTH_FEET 2.83465
#define IR_MAP_TRIP_VAL 90
#define PWM_MAPPED_MAX_OUTPUT_THRESHOLD 16

#define SAFETY 0
#define SINGLE_FIRE 1
#define BURST_FIRE 2
#define FULL_AUTO 3


SmartBlaster::SmartBlaster () :
  _display(OLED_RESET),
  _swCntBtn(SW_CNT_BTN_PIN, PULLUP, INVERT, DEBOUNCE),
  _reloadBtn(RELOAD_BTN_PIN, PULLUP, INVERT, DEBOUNCE),
  _magSzTogBtn(MAG_SZ_TOG_BTN_PIN, PULLUP, INVERT, DEBOUNCE),
  _revTrigBtn (REV_TRIG_BTN_PIN, PULLUP, INVERT, DEBOUNCE),
  _mainTrigBtn (MAIN_TRIG_BTN_PIN, PULLUP, INVERT, DEBOUNCE),
  _fireModeTogBtn (FIRE_MODE_TOG_BTN_PIN, PULLUP, INVERT, DEBOUNCE)  {

    resetChronoVals();
    _chronoVal = 0;

    _hasFlywheelsAccelerated = false;
    _flywheelAccelStartTime = 0;

    _hasPusherAccelerated = false;
    _pusherAccelStartTime = 0;

    _fireMode = 0;
    _dartsFiredForSelectFire = 0;
    _isCheckingForDartsFired = false;
}

void SmartBlaster::init(uint8_t modes[], uint8_t magSizes[], uint8_t pins[], uint32_t otherOptions[]) {
  initDisplay();
  initModes(modes);
  initMagSizes(magSizes);
  initPins(pins);
  initOtherOptions(otherOptions);

  initAmmoForDisplay();
  initChronoValForDisplay(false);
}

void SmartBlaster::smartMyBlaster() {
  ammoCounter();
  chrono();
  PWM(0);
  PWM(1);
  selectFire();
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

uint16_t SmartBlaster::getFlywheelPWM () {
  if (_isFlywheelPWM) {
    return analogRead(_FLYWHEEL_PWM_POT_PIN);
  }
}

uint16_t SmartBlaster::getPusherPWM () {
  if (_isPusherPWM) {
    return analogRead(_PUSHER_PWM_POT_PIN);
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
  _isPusherPWM = modes[4];
  _isSelectFire = modes[5];
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
  _FLYWHEEL_OUT_PIN = pins[2];
  _PUSHER_PWM_POT_PIN = pins[3];
  _PUSHER_OUT_PIN = pins[4];

  pinMode(_FLYWHEEL_OUT_PIN, OUTPUT);
  pinMode(_PUSHER_OUT_PIN, OUTPUT);
}

void SmartBlaster::initOtherOptions (uint32_t otherOptions[]) {
  _FLYWHEEL_MOTOR_ACCEL_TIME = otherOptions[0];
  _PUSHER_MOTOR_ACCEL_TIME = otherOptions[1];
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
  } else if (_maxAmmo == 0 && _currentAmmo < 255) { //make sure ammo doesn't overflow uint8_t max val
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



void SmartBlaster::PWM (uint8_t toPWM) {    //0 = flywheels, 1 = pusher
  if (_isFlywheelPWM || _isPusherPWM) {
    uint8_t isFlywheel = (toPWM == 0 && _isFlywheelPWM);

    Button& trigBtn = isFlywheel ? _revTrigBtn : _mainTrigBtn;
    uint8_t& hasAccelerated = isFlywheel ? _hasFlywheelsAccelerated : _hasPusherAccelerated;
    uint32_t& accelStartTime = isFlywheel ? _flywheelAccelStartTime : _pusherAccelStartTime;
    uint8_t& lastPotReading = isFlywheel ? _lastFlywheelPWMPotReading : _lastPusherPWMPotReading;
    const uint8_t PWM_IN_PIN = isFlywheel ? _FLYWHEEL_PWM_POT_PIN : _PUSHER_PWM_POT_PIN;
    const uint8_t PWM_OUT_PIN = isFlywheel ? _FLYWHEEL_OUT_PIN : _PUSHER_OUT_PIN; 
    const uint32_t MOTOR_ACCEL_TIME = isFlywheel ? _FLYWHEEL_MOTOR_ACCEL_TIME : _PUSHER_MOTOR_ACCEL_TIME; 

    trigBtn.read();
    if(trigBtn.isPressed() && !hasAccelerated) {           //when trigger first pressed
      digitalWrite(PWM_OUT_PIN, HIGH);                         //motor at full power
      if (accelStartTime == 0) {
        accelStartTime = millis();
      }
    } else if (trigBtn.isPressed() && hasAccelerated) {    //if trigger pressed
      analogWrite(PWM_OUT_PIN, analogRead(PWM_IN_PIN)/4);    //write PWM depending on pot value
    } else if (trigBtn.wasReleased()) {                     //when trigger released
      digitalWrite(PWM_OUT_PIN, LOW);                          //turn motor off
      hasAccelerated = false;                                  //reset flag to check for acceleration
    }

    checkFinishAccel(toPWM, accelStartTime, hasAccelerated, MOTOR_ACCEL_TIME);
    initDisplayPWM(PWM_IN_PIN, lastPotReading);
  }
}

void SmartBlaster::checkFinishAccel (uint8_t toPWM, uint32_t& accelStartTime, uint8_t& hasAccelerated, uint32_t MOTOR_ACCEL_TIME) {
    if ( (accelStartTime > 0) && (millis() > accelStartTime + MOTOR_ACCEL_TIME) ) {       //passed accel time
      hasAccelerated = true;
      accelStartTime = 0;
    }
}




void SmartBlaster::selectFire () {
  if (_isSelectFire) {
    toggleFireModes();
    fireBurst();
  }
}

void SmartBlaster::toggleFireModes () {
  if (_isSelectFire) {
    _fireModeTogBtn.read();
    if (_fireModeTogBtn.wasPressed()) {
      _fireMode += (_fireMode == 3 ? -3 : 1);
      resetSelectFireVals();
      printVals();
    }
  }
}

void SmartBlaster::resetSelectFireVals () {
  digitalWrite(_PUSHER_OUT_PIN, LOW);
  _dartsFiredForSelectFire = 0;
  _isCheckingForDartsFired = false;
}

void SmartBlaster::fireBurst () {
  if (_isCheckingForDartsFired &&                                        
   (_fireMode == SINGLE_FIRE || _fireMode == BURST_FIRE)) { 
    byte dartsToFire = (_fireMode == SINGLE_FIRE ? 1 : 3); 
    if (_dartsFiredForSelectFire < dartsToFire) {
      digitalWrite(_PUSHER_OUT_PIN, HIGH); 
    } else if (_swCntBtn.isPressed() &&  
     _dartsFiredForSelectFire >= dartsToFire) {      
      resetSelectFireVals();             
    }
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

void SmartBlaster::initDisplayPWM (uint8_t POT_PIN, uint8_t& lastPotReading) {
  uint8_t mappedPWMReading = map(analogRead(POT_PIN), 0, 1010, 0, PWM_MAPPED_MAX_OUTPUT_THRESHOLD);
  if (mappedPWMReading != lastPotReading) {
    lastPotReading = mappedPWMReading;
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

  if (_isPusherPWM) {
    uint8_t lineLength = 64 - _lastPusherPWMPotReading * 4;
    _display.drawLine(126, 63, 126, lineLength, WHITE);
    _display.drawLine(127, 63, 127, lineLength, WHITE);
  } 

  if (_isSelectFire) {
    _display.setCursor(85, 55);
    if (_fireMode == SAFETY) {
      _display.print("SAFETY");
    } else if (_fireMode == SINGLE_FIRE) {
      _display.print("SINGLE");
    } else if (_fireMode == BURST_FIRE) {
      _display.print("BURST");
    } else if (_fireMode == FULL_AUTO) {
      _display.print("AUTO");
    }
  }
  

  _display.display(); //display the text

}
