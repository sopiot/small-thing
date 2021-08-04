//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT thing library
#include "thing.h"

// Module libraries
#include <Servo.h>

// Pins
static const int kServo1Pin = 9;
static const int kMoisturePin = A0;

//--------------------------------------------------
// Modules
// -------------------------------------------------

// Modules
Servo servo1;

//----------------------------------------
// Thing
//----------------------------------------
// Thing(class_name, alive_cycle, serial);
// Thing(class_name, serial);
// class name should not include '_'
Thing valve_with_soil_moisture((const char *)"SmartPotVSoil", 60, SafeSerial);

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables
int valve_status_ = 0;
int pot_moisture_ = 0;

// Getter functions of each Value variable
int SenseValveStatus() { return valve_status_; }

int SensePotMoisture() {
  pot_moisture_ = (double)analogRead(kMoisturePin) / 1024 * 100;
  return pot_moisture_;
}

Value valve_status((const char *)"valve_status", SenseValveStatus, 0, 2, 1000);
Value pot_moisture((const char *)"pot_moisture", SensePotMoisture, 0, 2000,
                   3000);

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

void ActuateValveOpen(void *pData) {
  servo1.write(0);
  valve_status_ = 1;
}

void ActuateValveClose(void *pData) {
  servo1.write(90);
  valve_status_ = 0;
}

Function valve_open((const char *)"valve_open", ActuateValveOpen, 0, 0);
Function valve_close((const char *)"valve_close", ActuateValveClose, 0, 0);

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kServo1Pin, OUTPUT);
  pinMode(kMoisturePin, INPUT);

  // Attach modules
  servo1.attach(kServo1Pin);

  // Setup initial state
  servo1.write(90);
  valve_status_ = 0;
}

void SetupThing() {
  // Setup Functions
  valve_with_soil_moisture.Add(valve_open);
  valve_with_soil_moisture.Add(valve_close);

  // Setup Values
  valve_with_soil_moisture.Add(valve_status);
  valve_with_soil_moisture.Add(pot_moisture);

  // Setup Thing
  valve_with_soil_moisture.Setup();
}

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { valve_with_soil_moisture.Loop(); }