//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT Thing library
#include "thing.h"

// Module libraries

// Pins
static const int kTriggerPin = 3;
static const int kReceivePin = 4;

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
Thing distance_thing((const char *)"Distance", 60, SafeSerial);

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables
int distance_status_;

// Getter functions of each Value variable
int SenseDistanceStatus() {
  long Duration = 0;
  digitalWrite(kTriggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(kTriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(kTriggerPin, LOW);

  Duration = pulseIn(kReceivePin, HIGH);
  int Distance_mm = ((Duration / 2.9) / 2);

  return Distance_mm;
}

// Value declarations
// Value(name, sense_function, min, max, period(ms));
Value distance_status((const char *)"distance_status", SenseDistanceStatus, 0,
                      2, 3000);

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
  pinMode(kTriggerPin, OUTPUT);
  pinMode(kReceivePin, INPUT);

  // Attach modules
}

void SetupThing() {
  // Setup Functions

  // Setup Values
  distance_thing.Add(distance_status);

  // Setup Thing
  distance_thing.Setup();
}

//----------------------------------------
// Main
//----------------------------------------

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { distance_thing.Loop(); }
