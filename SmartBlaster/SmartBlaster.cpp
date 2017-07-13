#include "Arduino.h"
#include "SmartBlaster.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Button.h>


SmartBlaster::SmartBlaster (uint8_t modes[], uint8_t IOPins[], uint8_t magSizes[]): 
_ammoCountingButton(IOPins[0], false, false, 25), _magInsertionDetectionButton(IOPins[1], false, false, 25),
_magSizeToggleButton(IOPins[2], false, false, 25), _selectFireToggleButton(IOPins[4], false, false, 25), _display(4) {
	_lastVoltageCheckTime = 0;
	_delayTime = 500;

	_chronoToPrint = "";
    _ammoToPrint = "";
    _voltageToPrint = "";
    _firingModeToPrint = "";

    _fireMode = 0;	//0 = safe = SF, 1 = semi-automatic = SA, 2 = 3 round burst = 3b, 3 = fully automatic = AM

    IR_MAP_TRIP_VAL = 95;
    DART_LEGNTH_FEET = 0.2362208333;

    R1 = 100000.0;
    R2 = 10000.0;

	initModes(modes).initIOPins(IOPins).initMagSizes(magSizes);

}

SmartBlaster& SmartBlaster::initModes (uint8_t modes[]) {
	_isIRGate = modes[0];
	_isChrono = modes[1];
	_isVoltmeter = modes[2];
	_isSelectFire = modes[3];

	return *this;
}

SmartBlaster& SmartBlaster::initIOPins (uint8_t pins[]) {
	_AMMO_COUNTING_INPUT_PIN = pins[0];
	
	_MAG_INSERTION_DETECTION_PIN = pins[1];
	_MAG_SIZE_TOGGLE_INPUT_PIN = pins[2];
	
	if (_isVoltmeter) {
		_VOLTMETER_INPUT_PIN = pins[3];
	}

	if (_isSelectFire) {
		_TOGGLE_SELECT_FIRE_INPUT_PIN = pins[4];
		_SELECT_FIRE_OUTPUT_PIN = pins[5];
	}

    return *this;	
}

SmartBlaster& SmartBlaster::initMagSizes(uint8_t magSizes[]) {
	for (int i = 0; i < ((sizeof(magSizes)/sizeof(magSizes[0])) - 1); i++) {
		_magSizeArr[i] = magSizes[i];
	}
    _currentMagSize = 0;
    _maxAmmo = _magSizeArr[_currentMagSize];
    _currentAmmo = _maxAmmo;

	return *this;
}

void SmartBlaster::displayValues (void) {
	//display ammo
	_display.clearDisplay(); //clear the display, so the stuff that was here before is no longer here
	_display.setTextSize(6);  //set the size of the text
	_display.setTextColor(WHITE);    //set the color of text text
	//tell the display where to draw the text
	_display.setCursor( (SCREEN_WIDTH/2) - ((_ammoToPrint.length()*2) * 9) , (SCREEN_HEIGHT/2) - 30 );  //center text
	_display.print(_ammoToPrint);    //print the text

	_display.setTextSize(1);

	//display chrono values
	if (_isChrono) {
		_display.setCursor(0, 50);  
		_display.print(_chronoToPrint);
	}

	//display voltage values
	if (_isVoltmeter) {
		_display.setCursor(60, 50);
		_display.print(_voltageToPrint);
	}

	//display fire mode
	if (_isSelectFire) {
		_display.setCursor(80, 50);  
		_display.print(_firingModeToPrint);
	}	
  
	_display.display(); //display the text
}

void SmartBlaster::initDisplayVoltage (double voltage) {
	_voltageToPrint = (String)voltage + " v";
	displayValues();
}

void SmartBlaster::initDisplayAmmo (void) {
	String textToDisplay = "00";    //create something to store what to print. This is empty now
    //if the ammo to print, current ammo, is less that 10, make it like '01' or '04'  
    if (_currentAmmo < 10) {
      textToDisplay = "0" + (String)_currentAmmo; //fill the thing we used to store what to print
    } else {    //if not, leave it as is
      textToDisplay = (String)_currentAmmo;   //fill the thing we used to store what to print
    }

    _ammoToPrint = textToDisplay;  //display the text, the ammo

	displayValues();
}

void SmartBlaster::initDisplayChronoReadings (double fps) {
	if (fps == -1) {
	        _chronoToPrint = ("ERR");
	    } else if (fps == -2) {
	        _chronoToPrint = ("NO FPS");
	    } else {
	        _chronoToPrint = ( (String)(fps)  + " fps" );
	}

	displayValues();	
}

void SmartBlaster::initDisplayFireMode(void) {
	//print text based on mode: 0 == S == Safety, 1 == SS == Single Shot, 2 == 3B == 3 Round Burst, 3 == A == Automatic
	if (_fireMode == 0) {
	    _firingModeToPrint = "S";
	} else if (_fireMode == 1) {
	    _firingModeToPrint = "SS";
	} else if (_fireMode == 2) {
	    _firingModeToPrint = "3B";
	} else if (_fireMode == 3) {
	    _firingModeToPrint = "A";
	}	

	displayValues();
}

void SmartBlaster::resetChronoVals(void) {
	_tripTime = -10;
	_exitTime = -10;
}

double SmartBlaster::calculateChronoReadings(double firstTime, double secondTime) {
	if ( (_tripTime > -10) && (_exitTime > -10) ) {
        resetChronoVals();
        return (DART_LEGNTH_FEET) / ((secondTime-firstTime)/1000000.0);
	}
}

void SmartBlaster::chrono(void) {
    //when tripped and expecting first value
    if ((map(analogRead(_AMMO_COUNTING_INPUT_PIN), 0, 1023, 0, 100) > IR_MAP_TRIP_VAL) && (_tripTime == -10) ) { 
        _tripTime = micros();
    //when tripped and expecting second value
    } else if ( (_tripTime != -10) && (_exitTime == -10) && (map(analogRead(_AMMO_COUNTING_INPUT_PIN), 0, 1023, 0, 100) < IR_MAP_TRIP_VAL) )  {
        _exitTime = micros();
        initDisplayChronoReadings(calculateChronoReadings(_tripTime, _exitTime));

        countAmmo();  
        
    //when no second value within 1 second
    } else if ( (_tripTime != -10) && (_tripTime + 2000000) < micros() ) {
        resetChronoVals();
        initDisplayChronoReadings(-1);
    } 
}

void SmartBlaster::changeMags(void) {
	//make sure the magazine insertion detection button is pressed from not being pressed
    if (_magInsertionDetectionButton.isPressed()) {   //when the magazine is inserted
        _currentAmmo = _maxAmmo;  //set current ammo to the max amount of ammo
        initDisplayAmmo();  //display ammo
	}
}

void SmartBlaster::toggleMags(void) {
	//check if the magazine toggle button is pressed
  if (_magSizeToggleButton.isPressed()) {
      //make sure the value doesn't overflow:
      //if the we're trying to access the 10th element of the array, but there are only 9 elements, the program will break
        //must keep the value trying to access is within the amount of values there are. 
        if (_currentMagSize < ((sizeof(_magSizeArr)/sizeof(_magSizeArr[0])) - 1) ) {
            _currentMagSize ++;  //change current magazine size
        } else {  
            _currentMagSize = 0;
        }

        //there's a new max ammo, because there's a new magazine size
        _maxAmmo = _magSizeArr[_currentMagSize];
        _currentAmmo = _maxAmmo;

        initDisplayAmmo();    //display the maxAmmo

	} 
}

//detect if dart has been fired, based on IR gate or not
void SmartBlaster::ammoCounter (void) {
	//check to see if IR gate
	if (!_isIRGate) {
		//if not ir gate, then using switch, so it will be checking if button/switch is pressed
		if (_ammoCountingButton.isPressed()) {
			countAmmo();
		}
	} else {
		//if it is IR gate, then check to see if IR gate broken by dart
		if (map(analogRead(_AMMO_COUNTING_INPUT_PIN), 0, 1023, 0, 100) > IR_MAP_TRIP_VAL) {
			countAmmo();
		}
	}
}

//actually decrement or increment ammo
void SmartBlaster::countAmmo (void) {
	//count ammo stuff
    //make sure that the ammo is less than 99 so it doesnt overflow the display
    //make sure it's in increment mode
    if ( (_magSizeArr[_currentMagSize] == 0) && (_currentAmmo < 99) ) {
        _currentAmmo++;    //increment ammo
    
    //make sure that the ammo is more than 0 so no negative numbers are displayed
    //make sure it's in increment mode
    } else if ( (_currentAmmo > 0) && (_magSizeArr[_currentMagSize] != 0) ){
        _currentAmmo--;    //decrement ammo
    }

    initDisplayAmmo();    //display the ammo  
}

void SmartBlaster::voltmeter (void) {
	//make sure only prints every .5 sec
    if (millis() >= _lastVoltageCheckTime + _delayTime) {
        //calculate voltage
        float voltageIn = ((analogRead(_VOLTMETER_INPUT_PIN) * 5.0) / 1024.0) / (R2/ (R1 + R2));
    
        //make sure voltage is above 0.03, since it could be an error
        if (voltageIn < 0.5) {
            voltageIn = 0; 
        }

        initDisplayVoltage(voltageIn);

        _lastVoltageCheckTime = millis();
	}
}

void SmartBlaster::fireModeMotorControl(void) {
	boolean canShoot = false, wasDartFired = false;    //flags enabling shooting    
    //check trigger switch was pressed
    if (_selectFireToggleButton.isPressed()) {
        wasDartFired = true;
    }
    
    if ((_fireMode == 1 || _fireMode == 2) && wasDartFired == true) {    // if in burst mode or single shot mode, and if trigger pressed
        //based on the fire mode (single shot or burst), we will only fire a certain number if shots
        uint8_t modeAmmoIndex;
        if (_fireMode == 1) {
            modeAmmoIndex = 1;
        } else {
            modeAmmoIndex = 3;
        }

        //make sure haven't fired more than 1 or 3 shots, depending on the fire mode
        if ((_currentAmmo - modeAmmoIndex) < (_lastAmmo - 1)) {   
            //if haven't fired more than 1/3 shot, depending on fireMode, still can shoot more
            canShoot = true;
            _lastAmmo = _currentAmmo;
        } else {
            //if fired more than 1 or 3 shots, depending on mode, can't shoot anymore, and wait for next time trigger pressed
            canShoot = false;
            wasDartFired = false;
        }
    } else if (_fireMode == 3) { //if in full auto
        //make sure trigger switch pressed/blaster fired
        if (_isIRGate) {
        	if (map(analogRead(_AMMO_COUNTING_INPUT_PIN), 0, 1023, 0, 100) > IR_MAP_TRIP_VAL) {
				canShoot = true;
			}
		} else {
			if (_ammoCountingButton.isPressed()) {
            	canShoot = true;
        	}
		}
        
    } else {    //if not in fully auto, single shot, or burst
        //can't shoot
        canShoot = false;
    }

    //using the logic above, determine whether to shoot
    if (canShoot) {
        digitalWrite(_SELECT_FIRE_OUTPUT_PIN, HIGH);
    } else {
        digitalWrite(_SELECT_FIRE_OUTPUT_PIN, LOW);
	}
}

void SmartBlaster::toggleFireModeControl (void) {
	if (_selectFireToggleButton.isPressed()) {
		if (_fireMode < 3) {
			_fireMode++;
		} else {
			_fireMode = 0;
		}

		initDisplayFireMode();

	}
}

void SmartBlaster::smartMyBlaster (void) {
	toggleMags();
	changeMags();

	if (!_isChrono) {
		ammoCounter();
	} else {
		chrono();
	}

	if (_isVoltmeter) {
		voltmeter();
	}

	if (_isSelectFire) {
		fireModeMotorControl();
		toggleFireModeControl();

	}
}


