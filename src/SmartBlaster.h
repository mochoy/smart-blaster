#ifndef SmartBlaster_h
#define SmartBlaster_h

#include "Arduino.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Button.h>

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
    //methods
    public:
        //constructor
        SmartBlaster();

        
};

#endif
