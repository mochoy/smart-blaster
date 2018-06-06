#include <Button.h>
#include <SoftwareSerial.h>

//pins
#define TRIGGER_PIN 1
#define CYCLE_CONTROL_PIN 2

#define HALF_BRIDGE_LOW_IN 0
#define HALF_BRIDGE_HIGH_IN 3

#define JOYSTICK_X_PIN 3
#define JOYSTICK_Y_PIN 4

//joy stick settings
#define HIGH_JOYSTICK_TRIP 900 
#define LOW_JOYSTICK_TRIP 100

//btn settings
#define PULLUP true 
#define INVERT true   
#define DEBOUNCE_MS 20

//fire modes
#define SAFETY 0 
#define SINGLE_FIRE 1
#define BURST_FIRE 2
#define FULL_AUTO 3


//keeping track of fire stuff
byte fireMode = 0;  
byte lastFireMode = 0;

byte dartsFired = 0;       
bool isCheckingForDartsFired = false;  

//butons
Button trigger (TRIGGER_PIN, PULLUP, INVERT, DEBOUNCE_MS);     
Button cycleControlSwitch (CYCLE_CONTROL_PIN, PULLUP, INVERT, DEBOUNCE_MS); 


void setup() {
	Serial.begin(9600);

	pinMode(HALF_BRIDGE_LOW_IN, OUTPUT);
	pinMode(HALF_BRIDGE_HIGH_IN, OUTPUT);

	pusherOff(false);

}

void loop () {
	readAllBtns();
	toggleFireModes();

}

//toCheckCycleControl only turns off motors when cycle control pressed
void pusherOff(bool toCheckCycleControl) {
	if (toCheckCycleControl && cycleControlSwitch.isPressed()) {
		digitalWrite(HALF_BRIDGE_LOW_IN, HIGH);
		digitalWrite(HALF_BRIDGE_HIGH_IN, LOW);
	} else if (!toCheckCycleControl) {
		digitalWrite(HALF_BRIDGE_LOW_IN, HIGH);
		digitalWrite(HALF_BRIDGE_HIGH_IN, LOW);
	}
	
}

void pusherOn() {
	digitalWrite(HALF_BRIDGE_LOW_IN, LOW);
	digitalWrite(HALF_BRIDGE_HIGH_IN, HIGH);
}

//executed at beginning of loop only to keep readings reliable
void readAllBtns() {
	trigger.read();
	cycleControlSwitch.read();
}

void toggleFireModes() {
	bool hasStateChanged = false;
 
  if (lastFireMode != SAFETY && analogRead(JOYSTICK_X_PIN) > HIGH_JOYSTICK_TRIP) {   //safety
    lastFireMode = fireMode = SAFETY;
    hasStateChanged = true;
    Serial.println("S");
  } else if (lastFireMode != SINGLE_FIRE && analogRead(JOYSTICK_X_PIN) < LOW_JOYSTICK_TRIP) {  //burst
    lastFireMode = fireMode = SINGLE_FIRE;
    hasStateChanged = true;
        Serial.println("SF");

  } else if (lastFireMode != BURST_FIRE && analogRead(JOYSTICK_Y_PIN) > HIGH_JOYSTICK_TRIP) {  //single shot 
    lastFireMode = fireMode = BURST_FIRE;
    hasStateChanged = true;
        Serial.println("BF");

  } else if (lastFireMode != FULL_AUTO && analogRead(JOYSTICK_Y_PIN) < LOW_JOYSTICK_TRIP) {  //full auto
    lastFireMode = fireMode = FULL_AUTO;
    hasStateChanged = true;

        Serial.println("FA");

  }

  if (hasStateChanged) {
    // resetDartsFired();                                                    //reset darts fired stuff so it doesn't get messed up later
  }
}

//function to see if darts are fired using cycle control switch
void checkForDartsFired() {
	if (cycleControlSwitch.wasPressed()) {
		dartsFired++;
	}
}

void selectFire() {
	if (fireMode == SAFETY) {
		pusherOff(true);
	} else if (trigger.wasPressed()) {
		if (fireMode == SINGLE_FIRE || fireMode == BURST_FIRE) {
			isCheckingForDartsFired = true;
		} 
	} 
}

