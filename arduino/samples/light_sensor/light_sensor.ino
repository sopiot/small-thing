//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT Thing library
#include <sopiot.h>
// Module libraries

//----------------------------------------
// Modules
//----------------------------------------

// Modules

// Pins
static const int kLight1Pin = A0;

//----------------------------------------
// Thing
//----------------------------------------

// Thing declaration
// Thing(class_name, alive_cycle, serial);
// Thing(class_name, serial);
Thing light_sensor((const char *)"Light", 60, SafeSerial);

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables
int brightness_;

// Getter functions of each Value variable
int SenseBrightness() { return analogRead(kLight1Pin); }

// Value declarations
// Value(name, sense_function, min, max, period(ms));
Value brightness((const char *)"brightness", SenseBrightness, 0, 2048, 1000);

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

// Function declarations
// Function(name, actuate_function, arguments_num, function_attributes_num);

//----------------------------------------
// Setup
//----------------------------------------

void SetupSerial() { SafeSerial.begin(115200); }

void SetupModules() {
  // Setup Pin mode

  // Attach modules
}

void SetupThing() {
  // Setup Functions

  // Setup Values
  light_sensor.Add(brightness);

  // Setup Thing
  light_sensor.Setup();
}

//----------------------------------------
// Main
//----------------------------------------

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { light_sensor.Loop(); }