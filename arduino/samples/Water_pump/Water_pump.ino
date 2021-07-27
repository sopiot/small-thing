//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT Thing library
#include "thing.h"

// Pins
static const int kRelayPin = 3;

//----------------------------------------
// Thing
//----------------------------------------
// Thing(class_name, alive_cycle, serial);
// Thing(class_name, serial);
// class name should not include '_'
Thing water_pump((const char *)"SmartPotPump", 60, SafeSerial);

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables
int pump_status_ = 0;

// Getter functions of each Value variable
int SensePumpStatus() { return pump_status_; }

Value pump_status((const char *)"pump_status", SensePumpStatus, 0, 2, 1000);

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

void ActuatePumpOn(void *pData) {
  digitalWrite(kRelayPin, HIGH);
  pump_status_ = 1;
}

void ActuatePumpOff(void *pData) {
  digitalWrite(kRelayPin, LOW);
  pump_status_ = 0;
}

Function pump_on((const char *)"pump_on", ActuatePumpOn, 0, 0);
Function pump_off((const char *)"pump_off", ActuatePumpOff, 0, 0);

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kRelayPin, OUTPUT);

  // Setup initial state
  digitalWrite(kRelayPin, LOW);
  pump_status_ = 0;
}

void SetupThing() {
  // Setup Functions
  water_pump.Add(pump_on);
  water_pump.Add(pump_off);

  // Setup Values
  water_pump.Add(pump_status);

  // Setup Thing
  water_pump.Setup();
}

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { water_pump.Loop(); }