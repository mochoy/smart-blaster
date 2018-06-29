#include <Button.h>
#include <SoftwareSerial.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// If using software SPI (the default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

 
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

#define NUM_OF_BURST_FIRE_SHOTS 3


//keeping track of fire stuff
byte fireMode = 0;  
byte lastFireMode = 0;

byte dartsFired = 0;       
bool isCheckingForDartsFired = false;  

//butons
Button trigger (TRIGGER_PIN, PULLUP, INVERT, DEBOUNCE_MS);     
Button cycleControlSwitch (CYCLE_CONTROL_PIN, PULLUP, INVERT, DEBOUNCE_MS); 


void setup() {
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.display();

  pinMode(HALF_BRIDGE_LOW_IN, OUTPUT);
  pinMode(HALF_BRIDGE_HIGH_IN, OUTPUT);

  pusherOff(false);

}

void loop () {
  readAllBtns();
  toggleFireModes();
  selectFire();
  fireExactShots();
}

//toCheckCycleControl only turns off motors when cycle control pressed
void pusherOff(bool toCheckCycleControl) {
  // if (toCheckCycleControl && cycleControlSwitch.isPressed()) {
    digitalWrite(HALF_BRIDGE_LOW_IN, HIGH);
    digitalWrite(HALF_BRIDGE_HIGH_IN, LOW);
  // } else if (!toCheckCycleControl) {
  //  digitalWrite(HALF_BRIDGE_LOW_IN, HIGH);
  //  digitalWrite(HALF_BRIDGE_HIGH_IN, LOW);
  // }
  
}

void pusherOn() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello, world!");
  display.display();

  delay(50);

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
  } else if (lastFireMode != SINGLE_FIRE && analogRead(JOYSTICK_X_PIN) < LOW_JOYSTICK_TRIP) {  //burst
    lastFireMode = fireMode = SINGLE_FIRE;
    hasStateChanged = true;

  } else if (lastFireMode != BURST_FIRE && analogRead(JOYSTICK_Y_PIN) > HIGH_JOYSTICK_TRIP) {  //single shot 
    lastFireMode = fireMode = BURST_FIRE;
    hasStateChanged = true;

  } else if (lastFireMode != FULL_AUTO && analogRead(JOYSTICK_Y_PIN) < LOW_JOYSTICK_TRIP) {  //full auto
    lastFireMode = fireMode = FULL_AUTO;
    hasStateChanged = true;
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

//function to handle firing in burst and single shots, when number of shots is exact. 
//This is imporntat because fireMode  logic in selectFire() only executes when the 
//trigger was pressed, not while the trigger is pressed.
void fireExactShots() {
  //make sure checking for darts to be fired first. This happens when the trigger is pressed 
  //and on burst/single shot.
  //blaster can only fire exact shots when:
  //  - trigger has been pressed while in correct mode
  //  - there haven't been enough darts fired yet, darts fired is less than the amount of 
  //    darts to be fired
  if (isCheckingForDartsFired) {
    byte dartsToFire = (fireMode == SINGLE_FIRE ? 1 : NUM_OF_BURST_FIRE_SHOTS);   
    if (dartsFired < dartsToFire) {   //there are still darts to fire
      pusherOn();
    } else {
      isCheckingForDartsFired = false;
      pusherOff(true);
    }
  }
}

void selectFire() {
  if (fireMode == SAFETY) {
    pusherOff(true);
  } else if (trigger.wasPressed()) {
    if (fireMode == SINGLE_FIRE || fireMode == BURST_FIRE) {
      isCheckingForDartsFired = true;
    } 
  } else if (trigger.isPressed()) {
    if (fireMode == FULL_AUTO) {
      pusherOff(true);
    }
  } else {
    if (fireMode == FULL_AUTO) {
      pusherOff(true);
    }
  }
}


