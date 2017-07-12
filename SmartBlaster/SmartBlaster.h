#ifndef AmmoCounter_h
#define AmmoCounter_h

#include "Arduino.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Button.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

//stuff to help keep track of magazine stuff
//
//
//
//if you want to add/remove magazine sizes, do that here. Remember, they go in order when you toggle between them, from left to right in this.
//
//when you change the value, the "currentMagSize"(several lines below) has to be less than the number of different magazines sizes, 
//the number of different magazine size values in the "magSizeArr" you can change "currentMagSize" to whatever you want. 
//When it program first starts, when the microcontroller turns on, the 5th element of "magSizeArr" is the current magazine size,
//starting from 0. 
//Ex: byte array = {1(0), 2(1), 3(2), 4(3)} - the numbers without parenthesis are the values this array/list 
//stores, and the number between the parenthesis indicates which place they are, their "index", where they are in the list/array. 
//If I want to access the value 1, which is the first value of the array/list, which the computer sees as the 
//"zeroith" value, I would do array[0]. If I want to access the value 3, the third value of the array, I would do array[2]
class SmartBlaster {
    private:
        //usefule constant values needed to deal with specific functions
        static int IR_MAP_TRIP_VAL;
        static float DART_LEGNTH_FEET;
        static float R1;
        static float R2;

        //stuff to help keep track of magazine stuff
        //
        //
        //
        //if you want to add/remove magazine sizes, do that here. Remember, they go in order when you toggle between them, from left to right in this.
        //
        //when you change the value, the "currentMagSize"(several lines below) has to be less than the number of different magazines sizes, 
        //the number of different magazine size values in the "magSizeArr" you can change "currentMagSize" to whatever you want. 
        //When it program first starts, when the microcontroller turns on, the 5th element of "magSizeArr" is the current magazine size,
        //starting from 0. 
        //Ex: byte array = {1(0), 2(1), 3(2), 4(3)} - the numbers without parenthesis are the values this array/list 
        //stores, and the number between the parenthesis indicates which place they are, their "index", where they are in the list/array. 
        //If I want to access the value 1, which is the first value of the array/list, which the computer sees as the 
        //"zeroith" value, I would do array[0]. If I want to access the value 3, the third value of the array, I would do array[2]
        uint8_t _magSizeArr[];
        uint8_t _currentMagSize;
        uint8_t _maxAmmo;
        uint8_t _currentAmmo;

        //stuff for dealing with pins
        uint8_t _AMMO_COUNTING_INPUT_PIN;
        uint8_t _MAG_INSERTION_DETECTION_PIN;
        uint8_t _MAG_SIZE_TOGGLE_INPUT_PIN; 
        uint8_t _VOLTMETER_INPUT_PIN;
        uint8_t _TOGGLE_SELECT_FIRE_INPUT_PIN;

        uint8_t _SELECT_FIRE_OUTPUT_PIN; 

        //to keep keep track of the buttons
        Button _ammoCountingButton;
        Button _magInsertionDetectionButton;
        Button _magSizeToggleButton;
        Button _selectFireToggleButton;

        // Strings to know what to print
        String _chronoToPrint;
        String _ammoToPrint;
        String _voltageToPrint;
        String _firingModeToPrint;

        //stuff for dealing with modes: chrono, voltmeter, etc.
        uint8_t _isIRGate;  
        uint8_t _isChrono;
        uint8_t _isVoltmeter;
        uint8_t _isSelectFire;

        //stuff for dealing with chrono calculations
        double _tripTime;
        double _exitTime;

        //values for dealing with voltmeter stuff
        double _lastVoltageCheckTime;
        int _delayTime;

        //value for dealing with select-fire
        uint8_t _lastAmmo;
        uint8_t _fireMode;  //0 = safe = SF, 1 = semi-automatic = SA, 2 = 3 round burst = 3b, 3 = fully automatic = AM

        Adafruit_SSD1306 _display;

    //methods
    public:
        //constructor
        SmartBlaster(uint8_t[], uint8_t[], uint8_t[]);

        //init stuff
        SmartBlaster& initModes(uint8_t[]);
        SmartBlaster& initIOPins(uint8_t[]);
        SmartBlaster& initButtons();
        SmartBlaster& initMagSizes(uint8_t[]);

        //getter and setter for display
        Adafruit_SSD1306 setDisplay(Adafruit_SSD1306);
        Adafruit_SSD1306 getDisplay();

        //display stuff
        void displayValues();
        void initDisplayAmmo();
        void initDisplayChronoReadings(double);
        void initDisplayFireMode();
        void initDisplayVoltage(double);


        //chrono stuff
        void resetChronoVals();
        double calculateChronoReadings(double, double);
        void chrono();

        //mag stuff
        void changeMags();
        void toggleMags();

        void ammoCounter();
        void countAmmo();

        void voltmeter();

        void fireModeMotorControl();
        void toggleFireModeControl();
        void fireVelocityControl();

        void smartMyBlaster();
};

#endif