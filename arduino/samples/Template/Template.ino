//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT Thing library
#include "thing.h"

// Module libraries
#include <Servo.h>

// Pins
static const int kServo1Pin = 8;

//----------------------------------------
// Modules
//----------------------------------------

// Modules
Servo servo1;

//----------------------------------------
// Thing
//----------------------------------------

// Thing declaration
// Thing(class_name, alive_cycle, serial);
// Thing(class_name, serial);
Thing template_thing((const char *)"Template", 60, SafeSerial);

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables
int switch_status_;

// Getter functions of each Value variable
int SenseSwitchStatus() { return switch_status_; }

// Value declarations
// Value(name, sense_function, min, max, period(ms));
Value switch_status((const char *)"switch_status", SenseSwitchStatus, 0, 2,
                    10000);

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

void ActuateSwitchOn(void *pData) {
  // int res;
  // res = GetIntArgumentByName(pData, "angle", &angle);
  // if (res == -1)
  //   return;

  // servo1.write(angle);
  servo1.write(45);
  delay(300);
  switch_status_ = 1;
}

// Function declarations
// Function(name, actuate_function, arguments_num, function_attributes_num);
Function switch_on((const char *)"switch_on", ActuateSwitchOn, 0, 0);

//----------------------------------------
// Setup
//----------------------------------------

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kServo1Pin, OUTPUT);

  // Attach modules
  servo1.attach(kServo1Pin);
}

void SetupThing() {
  // Setup Functions
  template_thing.Add(switch_on);

  // Setup Values
  template_thing.Add(switch_status);

  // Setup Thing
  template_thing.Setup();
}

//----------------------------------------
// Main
//----------------------------------------

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { template_thing.Loop(); }
