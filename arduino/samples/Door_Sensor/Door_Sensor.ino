//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT Thing library
#include "thing.h"

// Module libraries

// Pins
static const int kDoorPin = 2;

//----------------------------------------
// Modules
//----------------------------------------

// Modules

// Global variables

int openclose_ = 0;

//----------------------------------------
// Thing
//----------------------------------------
Thing thing((const char *)"DoorSensor", 60, SafeSerial);

//----------------------------------------
// Value callback functions
//----------------------------------------
int SenseOpenClose() { return digitalRead(kDoorPin); }

Value openclose_value((const char *)"openclose_value", SenseOpenClose, 0, 1,
                      1000);

//----------------------------------------
// Functions callback functions
//----------------------------------------

//----------------------------------------
// Setup
//----------------------------------------
void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kDoorPin, INPUT);

  // Attach modules
}

void SetupThing() {
  // Setup Functions

  // Setup Values
  thing.Add(openclose_value);

  // Setup Thing
  thing.Setup();
}

void setup() {
  SetupSerial();
  SetupModules();

  SetupThing();
}

//----------------------------------------
// Main
//----------------------------------------
void loop() { thing.Loop(); }
