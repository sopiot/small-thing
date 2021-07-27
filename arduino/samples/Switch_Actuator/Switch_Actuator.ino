//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT Thing library
#include "thing.h"

// Module libraries
#include <Servo.h>

// Pins
static const int kServo1Pin = 8;
static const int kServo2Pin = 9;

//----------------------------------------
// Modules
//----------------------------------------

// Modules
Servo servo1;
Servo servo2;

//----------------------------------------
// Thing
//----------------------------------------

// Thing declaration
// Thing(class_name, alive_cycle, serial);
// Thing(class_name, serial);
Thing Actuate_thing((const char *)"Actuate", 60, SafeSerial);

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
                    1000);

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

void ActuateSwitchOn(void *pData) {
  servo2.attach(kServo2Pin);
  int power = 1, res;
  int mid = 90, turn = 45;

  // res = GetIntArgumentByName(pData, ARG_SERVO_SWITCH, &power);
  // if (res == -1)
  //     return;

  // TODO:(thsvkd) - tuning servo1.write Value to meetting room switch!!!
  servo2.write(mid - turn);
  delay(300);
  servo2.write(mid);
  delay(300);
  switch_status_ = 1;
  servo2.detach();
}

void ActuateSwitchOff(void *pData) {
  servo1.attach(kServo1Pin);
  int power = 1, res;
  int mid = 90, turn = -45;

  // res = GetIntArgumentByName(pData, ARG_SERVO_SWITCH, &power);
  // if (res == -1)
  //     return;

  // TODO:(thsvkd) - tuning servo1.write Value to meetting room switch!!!
  servo1.write(mid - turn);
  delay(300);
  servo1.write(mid);
  delay(300);
  switch_status_ = 0;
  servo1.detach();
}

// Function declarations
// Function(name, actuate_function, arguments_num, function_attributes_num);
Function switch_on((const char *)"switch_on", ActuateSwitchOn, 0, 0);
Function switch_off((const char *)"switch_off", ActuateSwitchOff, 0, 0);

//----------------------------------------
// Setup
//----------------------------------------

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kServo1Pin, OUTPUT);
  pinMode(kServo2Pin, OUTPUT);

  // Attach modules
  servo1.attach(kServo1Pin);
  servo1.attach(kServo2Pin);
}

void SetupThing() {
  // Setup Functions
  Actuate_thing.Add(switch_on);
  Actuate_thing.Add(switch_off);

  // Setup Values
  Actuate_thing.Add(switch_status);

  // Setup Thing
  Actuate_thing.Setup();
}

//----------------------------------------
// Main
//----------------------------------------

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { Actuate_thing.Loop(); }
