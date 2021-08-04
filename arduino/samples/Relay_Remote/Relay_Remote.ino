//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT Thing library
#include "thing.h"

// Module libraries

// Pins
static const int kRelayPin = 5;

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
Thing relayremote_thing((const char *)"RelayRemote", 60, SafeSerial);

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables
int switch_status_ = LOW;

// Getter functions of each Value variable
int SenseSwitchStatus() { return switch_status_; }

// Value declarations
// Value(name, sense_function, min, max, period(ms));
Value switch_status((const char *)"switch_status", SenseSwitchStatus, 0, 2,
                    3000);

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

void RelayFunction(void *pData) {
  Serial.print("Switch = ");
  Serial.println(switch_status_);
  switch_status_ = !switch_status_;
  digitalWrite(kRelayPin, switch_status_);
}

// Function declarations
// Function(name, actuate_function, arguments_num, function_attributes_num);
Function relay_onoff((const char *)"relay_onoff", RelayFunction, 0, 0);

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
  relayremote_thing.Add(relay_onoff);

  // Setup Values
  relayremote_thing.Add(switch_status);

  // Setup Thing
  relayremote_thing.Setup();
}

//----------------------------------------
// Main
//----------------------------------------

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { relayremote_thing.Loop(); }
