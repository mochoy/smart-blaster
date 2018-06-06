#include <Button.h>

//pins
#define TRIGGER_PIN 1
#define CYCLE_CONTROL_PIN 2

#define HALF_BRIDGE_LOW_IN 0
#define HALF_BRIDGE_HIGH_IN 3

#define JOYSTICK_X_PIN 2
#define JOYSTICK_Y_PIN 3

//joy stick settings
#define HIGH_JOYSTICK_TRIP 490 
#define LOW_JOYSTICK_TRIP 360

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
Button dartCountingSwitch (DART_COUNTER_SWITCH_PIN, PULLUP, INVERT, DEBOUNCE_MS); 


void setup() {
	
}

void loop () {

}