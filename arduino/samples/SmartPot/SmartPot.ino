// solenoid valve + soil sensor

//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT Thing library
#include <thing.h>

// Module libraries

// Pins
static const int kRelayPin = 3;
static const int kMoisture1Pin = A1;
static const int kMoisture2Pin = A2;

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
int pot_status_ = 0;  // == valve status. 0: open, 1: closed

// Getter functions of each Value variable
int SensePotStatus() { return pot_status_; }

int SensePot1Moisture() {
  return (double)analogRead(kMoisture1Pin) / 1024.0 * 100;
}

int SensePot2Moisture() {
  return (double)analogRead(kMoisture2Pin) / 1024.0 * 100;
}
// Value declarations
// Value(name, sense_function, min, max, period(ms));
Value pot_status((const char *)"pot_status", SensePotStatus, 0, 2, 10000);

Value soil_moisture((const char *)"soil_moisture", SensePot1Moisture, 0, 2000,
                    3000);

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

void ActuateValveLock(void *pData) {
  digitalWrite(kRelayPin, HIGH);
  // delay(300);
  pot_status_ = 0;
}

void ActuateValveUnlock(void *pData) {
  digitalWrite(kRelayPin, LOW);
  // delay(300);
  pot_status_ = 1;
}

// Function declarations
// Function(name, actuate_function, arguments_num, function_attributes_num);
Function valve_lock((const char *)"valve_lock", ActuateValveLock, 0, 0);
Function valve_unlock((const char *)"valve_unlock", ActuateValveUnlock, 0, 0);

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
  smartpot_thing.Add(valve_lock);
  smartpot_thing.Add(valve_unlock);

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