# Smart Blaster v0.8:
## Completely outclass your opponents at your next NERF war.

Arduino library to make ammo counters and more for highly modified NERF blasters. All sketches require corresponding hardware.

___

## Contents:
- [How it all Works](#how-it-all-works)
- [Features Included](#features-included)
- [Parts and Tools Needed](#parts-and-tools-needed)
- [Getting Started](#getting-started)
- [Examples](#examples)
- [Version History](#version-history)
- [How to Contribute](#how-to-contribute)

___

## How it all Works:
An Arduino microcontroller is placed in the blaster. Through a series of inputs, the microcontroller can detect when a dart is fired, when the magazine is removed and re-inserted (reloading), and much more. These inputs are made visible to the user through a small, 0.96" 128 x 64 OLED display, which displays the ammo and more, depending on the mode. Both the code and electronics are requried to make everything work. This README includes instructions on installing the code, as well as schematics for each of the components for you to make your very own.

___

## Features Included:
To include any of these features, the corresponding hardware must be included.

1. **Ammo Counting:** Count your ammo. Whenever you fire a dart, it's detected using one of these methods:
	1. **Switch**: Detect shots every time the trigger is pressed. Orient a tactile switch to be pressed when the blaster's trigger is pressed, and wire it up to the microcontroller. May not be ideal for blasters where 1 trigger pull will fire more than one dart, including fully automatic blasters. 
	2. **Infrared (IR) Gate**: Using both an IR transmitter diode and IR receiver diode to detect dart passage rather than physical trigger pulls. The transmitter and receiver should be positioned so the transmitter shines directly on the receiver along the diameter of the barrel, forming a "gate". When a dart is fired, the gate is broken, which is detected with the microcontroller.
2. **Chronograph**: Using an IR gate, a chronograph can be implemented by recording the time it took for the dart to completely break through the gate, divided by the known length of the dart. Units are in feet per second (fps).
3. **Voltmeter**: Using a voltage divider, the Arduino can read the voltage of the battery used. Ideal for electronic-powered blasters, so you know when to switch out the batteries. Can detect the voltage of the battery powering the blaster, or of the battery powering the microcontroller.
4. **Select-Fire**:  Want to conserve ammo in your fully-automatic blaster? Toggle between safety, single-shot. three-round-burst, or fully-automatic with this feature. Requires a relay or MOSFET to control the pusher motor. IR gate required.
6. **Fire Velocity Control**: Using a potentiometer, the amount of power going into the motors can be controlled, so the speed of the motor can be controlled. The more power going into the motor, the faster the motor will spin, and the faster and further the dart will fire. Only works on motorized flywheel blasters. Requires a MOSFET to pwm the motors. 
6. **Magazine Insertion Detection**:  Check if magazine is inserted in blaster, and when magazine is swapped out, the ammo will reset.
7. **Different Magazine Sizes**: Toggle between various magazines sizes with the press of a button. All NERF-compatible magazines can be configured: 5, 6, 10, 12, 15, 18, 20, 21, 25, and 36.
8. **Count-Up Mode**: Instead of counting down, it can count up every time a shot is fired.

___

## Parts and Tools Needed:
All builds require some sort of Arduino-compatible microcontroller. I recommend the [Adafruit Pro Trinket](https://www.adafruit.com/product/2000). Also make sure to have the [Arduino IDE](https://www.arduino.cc/en/Main/Software) installed.

### Parts:
|               Features vs Parts Required                   | Ammo Counter - Switch | Ammo Counter - IR Gate| Chronograph | Voltmeter| Select - Fire |                                                                           Usages                                                                             |                                                                                                              Notes                                                                                               |
|:-----------------------------------------------------------|:---------------------:|:---------------------:|:-----------:| :-------:|:-------------:|:------------------------------------------------------------------------------------------------------------------------------------------------------------:|:----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|
|                   Microcontroller                          |           X           |           X           |      X      |     X    |       X       | Brains of everything.                                                                                                                                        |                                                                                          Microcontroller recommendation above                                                                                    |
| [Slider Switch](https://goo.gl/cvBtNh)                     |           X           |           X           |      X      |     X    |       X       | Used to toggle power/on of microcontroller                                                                                                                   | Can be replaced by rocker switch                                                                                                                                                                                 |
| [I2C OLED Display](https://goo.gl/BAZtMd)                  |           X           |           X           |      X      |     X    |       X       | Used for displaying ammo, and if applicable, mode, voltage, and chrono readings                                                                              |                                                                                                                                                                                                                  |
| [Lever Switch](https://goo.gl/oKqK1h)                      |         X (2)         |           X           |      X      |     X    |       X       | Used for magazine insertion detection. If switch varient of ammo counter is selected, then another switch will be used for ammo counting.                    |                                                                                                                                                                                                                  |
| [Momentary Push Button](https://goo.gl/MkMK2u)             |           X           |           X           |      X      |     X    |     X (2)     | Used for toggling between the various magazines sizes. If Select-Fire mode setup, then another button will be used to toggle through the various fire modes. |                                                                                                                                                                                                                  |
| [IR Transmitter and Receiver Diode](https://goo.gl/tKLQK2) |                       |           X           |      X      |          |       X       | Used for IR gate                                                                                                                                             |                                                                                                                                                                                                                  |
| Resistors (100K and 1000k)                                 |                       |                       |             |     X    |               | Used for voltage divider to drop the voltage to a safe level for the microcontroller.                                                                        | Resistors can be different values, just make sure the battery being checked has its voltage dropped to a voltage safe for the microcontroller.                                                                   |
| [Relay](https://goo.gl/cXLg6e)                             |                       |                       |             |          |       X       | Used to toggle power of the blaster's motors throught the microcontroller                                                                                    | Make sure the relay is rated for the power (voltage and curret) for your blaster. Will only work to toggle power, so only controls on/off. Will not work for an analog value, such as for fire velocity vontrol. |
| [MOSFET](https://goo.gl/vGVjyK)                            |                       |                       |             |     X    |       X       | Used to toggle power of blaster's motors, and control fire velocity.                                                                                         | Can replace relay for select fire, but required for fire velocity control                                                                                                                                        |

### Tools:
 1. Phillips Head Screwdriver
2. Drill
3. Soldering Iron and Solder + soldering materials/tools (wire, flux, shrink wrap, helping hands, etc.)
3. Dremel or rotary tool (recommended, not required)
4. Hot glue and/or Epoxy putty (recommended, not required)
5. Sandpaper and/or files (recommened, not required)
6. Nuts and bolts to mount display and/or microcontroller

**Extra Notes on Parts and Tools**: 
 1. Most of the parts can be bought cheaper from EBay or Digikey or other electronics stores.
2. Microcontroller will need a battery to power it. A 9v will work fine. If the blaster is an electronic blaster, the microcontroller can be powered off the same batttery as the blaster, at the cost of a slight decrease in the blaster's performance.

___

## Getting Started
A good foundation in basic electronics and a bit of physics as well a programming will help dramatically when building your blaster. The microcontroller needs to be programmed before it can be put into the blaster: 

### 1) Installation
Go to folder 'libraries' to add some libraries which can be accessed through the Arduino IDE:

```sh
cd ../Arduino/libraries
```

Install library to Smart Your Blaster:

```sh
git clone https://github.com/etnom/nerf-ammo-counter.git
```

Install Adafruit's library to work with display:

```sh
git clone https://github.com/adafruit/Adafruit_SSD1306.git
```

Install library to work with graphics on the dosplay:

```sh
git clone https://github.com/adafruit/Adafruit-GFX-Library.git
```

Install library to deal with buttons and debouncing:
 
`git clone https://github.com/JChristensen/Button.git`

Alternatively, the libraries can be installed the easy way:
1. Go to https://github.com/etnom/nerf-ammo-counter, click the **Download ZIP** button and save the ZIP file to a convenient location on your PC. 
2. Uncompress the downloaded file.  This will result in a folder containing all the files for the library, that has a name that includes the branch name, usually **nerf-ammo-counter-master**.
3.  Rename the folder to just **nerf-ammo-counter**.
4.  Copy the renamed folder to the Arduino/libraries folder.
5.  Open the folder **Libraries** in **nerf-ammo-counter**. Copy the three folders out to the Arduino/libraries folder.

### 2) Importation
In an Arduino sketch, include the required libraries:

```c++
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Button.h>
#include <SmartBlaster.h>
```
### 3) Usage

Also set up some stuff for the display:

```c++
#define OLED_RESET 4


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
```

Set up modes:

```c++
byte modes[] = {isIRAmmoCounter, isChronograph, isVoltmeter, isSelectFire};
```
- All items in array are boolean values.
- First item specifies if ammo counting will be detected through a switch or IR gate. `false` if switch, `true` if IR gate. Can't be both switch and IR gate.
- Second item specifies whether or not the chronograph feature will be implemented, third for voltmeter, and fourth for select-fire.

Setup IO Pins:

```c++
int pins[] = {ammoCountingInputPin, magazineInsertionDetectionPin, magazineSizeTogglePin, voltmeterPin, firingModeTogglePin, firingModeOutputPin};
```

- All items in array are integer values.
- If the corresponding mode for the pin was not set up, put `255` for the pin value.
- First item will be pin which the ammo counting mechanism (switch or IR gate) is connected to. 
- Make sure if the ammo counting mechanism is switch, it's connecting to a digital pin. If the ammo counting mechanism is the IR gate, make sure it's connected to an analog pin. 
- Second item is digital pin which the magazine insertion detection switch is connected to.
- Third item is digital pin which the magazine size toggle button is connected to. 
- Fourth item is the analog pin which the voltage divider for the voltmeter is connected to. 
- Fifth item is the digital pin which the fire mode toggle button is connected to. 
- Sixth item is the digital pin which the relay for connecting the automatic blaster's motor is connected to. 

Setup magazines sizes:

```c++
byte magSizes[] = {5, 6, 8, 10, 12, 15, 18, 19, 20, 22, 25, 36, 0};
```

- All items in array are integer values.
- Put all desired magazine sizes to use in the array. Above are all of the magazines sizes of every NERF compatable magazine in existence.
- The countup mode is specified by `0` is the magazine size.
- When the magazine size is toggled, it goes from left to right in the array. When the last item is reached, it starts back at the beginning.

Setup SmartBlaster:

```c++
SmartBlaster smartBlaster(modes, pins, magSizes);
```

Now lets Smart the Blaster! In `void loop()`:

```c++
smartBlaster.smartMyBlaster();
```

Once everything is done, upload the sketch to the microcontroller. 

### 4) Blaster/Shell Modifications
After the code has been uploaded the the microcontroller, it's time to modify a bit of the shell to integrate all the features to best Smart your Blaster. 
- Remove all the screws to open up the blaster. 
- For the magazine insertion detection switch, it should not be accessible from the exterior of the blaster. Orient the switch so it closes the circuit when the magazine is inserted, and opens the circuit when the magazine is removed. The best spot to place the magazine insertion detection switch is usually in the magazine well.
- If the switch variant of the ammo counter has been specified, place the switch in a position where the switch closes the circuit when the trigger is pressed, and opens the circuit when the trigger is not pressed. The best spot is usually directly behind the physical trigger of the blaster.
- Drill some holes in the shell to fit the rest the buttons and switches. Make sure the buttons and switches are accessible when the blasters is closed. 
- Attach these switches and buttons with epoxy putty or hot glue. 

#### Schematics
Based on your included features, wire up the microcontroller and the required electronic components to match the schematics:
- Magazine Insertion Detection Switch (same as Switch component of Ammo Counter - Switch variant) - Individual component
- Magazine size toggle button (same as toggle fire mode button component of Select-Fire) - Individual Component
- Ammo Counter - IR Gate variant, same as Chronograph - Individual Component
- Voltmeter - Individual Component
- Select fire + firemode toggle button
- Complete Ammo Counter: Switch
- Complete Chrono-Ammo Counter
- Complete Select-Fire-Ammo 
- Complete Smartest Blaster

___

## Examples
The following examples are included with the **Smart Blaster** library in the **Examples** folder:
1. **Ammo Counter**: A simple ammo counter for NERF blasters, using the switch mechanism. Includes ammo counting, magazine insertion detecion, and magazines size toggling all displayed on a 128 x 64 OLED display.
2. **Chrono-Ammo Counter**: A simple ammo counter, but also includes the chronograph feature. Uses the IR gate mechanism, with all data displayed on a 128 x 64 OLED display.
3. **Voltmeter-Ammo Counter**: A simple ammo counter using the switch mechanism, including all of the features in the Ammo Counter example sketch, with the voltmeter feature implemented. 
4. **Select-fire-Ammo Counter**: A simple IR gate ammo counter, including all of the features in the Ammo Counter example sketch, with select-fire implemented.
5. **Smartest Blaster**: Includes all the features: IR gate ammo counter, chronograph, voltmeter, and select-fire.

___

## Version History
- **v1.9**: Added a ton more features, including voltmeter, chronograph, and fire modes.
- **v1.0**: Just a simple ammo counter.

___

## How to Contribute
Want to add more features, or missing something? You can add it! Made a blaster with Smart Blaster? We'd love to hear from you! 

