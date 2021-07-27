// solenoid valve + soil sensor

//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT Thing library
#include "thing.h"

// Module libraries

// Pins
static const int kRelayPin = 3;
static const int kMoisture1Pin = A1;

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
Thing smartpot_thing((const char *)"SmartPot", 60, SafeSerial);

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables
int pot_status_ = 0;    // == TODO:
int valve_status_ = 0;  // == valve status. 1: power on

// Getter functions of each Value variable
int SensePotStatus() { return pot_status_; }
int SensePotStatus() { return valve_status_; }

int SensePotMoisture() {
  return (double)analogRead(kMoisture1Pin) / 1024.0 * 100;
}

// Value declarations
// Value(name, sense_function, min, max, period(ms));
Value pot_status((const char *)"pot_status", SensePotStatus, 0, 3, 10000);
Value valvue_status((const char *)"pot_status", SensePotStatus, 0, 1, 10000);
Value soil_moisture((const char *)"soil_moisture", SensePotMoisture, 0, 1000,
                    3000);

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

void ActuateValveClose(void *pData) {
  digitalWrite(kRelayPin, HIGH);
  // delay(300);
  pot_status_ = 1;
}

void ActuateValveOpen(void *pData) {
  digitalWrite(kRelayPin, LOW);
  // delay(300);
  pot_status_ = 0;
}

// Function declarations
// Function(name, actuate_function, arguments_num, function_attributes_num);
Function valve_close((const char *)"valve_close", ActuateValveClose, 0, 0);
Function valve_open((const char *)"valve_open", ActuateValveOpen, 0, 0);

//----------------------------------------
// Setup
//----------------------------------------

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kRelayPin, OUTPUT);

  // Attach modules
}

void SetupThing() {
  // Setup Functions
  smartpot_thing.Add(valve_close);
  smartpot_thing.Add(valve_open);

  // Setup Values
  smartpot_thing.Add(pot_status);
  smartpot_thing.Add(soil_moisture);

  // Setup Thing
  smartpot_thing.Setup();
}

//----------------------------------------
// Main
//----------------------------------------

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { smartpot_thing.Loop(); }