//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT thing library
#include "thing.h"

// Pins
static const int kMoisture1Pin = A1;
static const int kMoisture2Pin = A2;

//----------------------------------------
// Thing
//----------------------------------------
// Thing(class_name, alive_cycle, serial);
// Thing(class_name, serial);
// class name should not include '_'
Thing two_soil_sensor((const char *)"SmartPotTwoSoil", 60, SafeSerial);

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables

// Getter functions of each Value variable

int SensePot1Moisture() {
  return (double)analogRead(kMoisture1Pin) / 1024.0 * 100;
}

int SensePot2Moisture() {
  return (double)analogRead(kMoisture2Pin) / 1024.0 * 100;
}

Value pot1_moisture((const char *)"pot1_moisture", SensePot1Moisture, 0, 2000,
                    3000);
Value pot2_moisture((const char *)"pot2_moisture", SensePot2Moisture, 0, 2000,
                    3000);

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  pinMode(kMoisture1Pin, INPUT);
  pinMode(kMoisture2Pin, INPUT);
}

void SetupThing() {
  // Setup Values
  two_soil_sensor.Add(pot1_moisture);
  two_soil_sensor.Add(pot2_moisture);
  // Setup Thing
  two_soil_sensor.Setup();
}

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { two_soil_sensor.Loop(); }