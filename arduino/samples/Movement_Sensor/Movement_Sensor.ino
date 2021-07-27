//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT Thing library
#include "thing.h"

// Module libraries

// Pins
static const int kmovement1Pin = 7;

//----------------------------------------
// Modules
//----------------------------------------

// Modules

//----------------------------------------
// Thing
//----------------------------------------

// Thing declaration
// Thing(class_name, alive_cycle, serial);
// Thing(class_name, serial);
Thing movement_thing((const char *)"Move", 60, SafeSerial);

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables

// Getter functions of each Value variable
int SenseMovementStatus() { return (int)digitalRead(kmovement1Pin); }

// Value declarations
// Value(name, sense_function, min, max, period(ms));
Value movement_value((const char *)"movement_value", SenseMovementStatus, 0, 2,
                     1000);
Tag movement_tag("Move");

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

// Function declarations
// Function(name, actuate_function, arguments_num, function_attributes_num);

//----------------------------------------
// Setup
//----------------------------------------

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kmovement1Pin, INPUT);

  // Attach modules
}

void SetupThing() {
  // Setup Functions

  // Setup Values
  movement_value.AddTag(movement_tag);
  movement_thing.Add(movement_value);

  // Setup Thing
  movement_thing.Setup();
}

//----------------------------------------
// Main
//----------------------------------------

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { movement_thing.Loop(); }
