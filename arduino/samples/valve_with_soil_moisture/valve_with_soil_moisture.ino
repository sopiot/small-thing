//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT thing library
#include <thing.h>

// Module libraries
#include <Servo.h>

// Pins
static const int kServo1Pin = 9;
static const int kMoisturePin = A1;

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
Thing valve_with_soil_moisture((const char *) "SmartPotVSoil", 60, SafeSerial);

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables
int valve1_status_ = 0;
int pot1_moisture_ = 0;

// Getter functions of each Value variable
int SenseValve1Status() { return valve1_status_; }

int SensePot1Moisture() {
  return (double)analogRead(kMoisturePin) / 1024 * 100;
}

Value valve1_status((const char *) "valve1_status", SenseValve1Status, 0, 2,
                    1000);
Value pot1_moisture((const char *) "pot1_moisture", SensePot1Moisture, 0, 2000,
                    3000);

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

void ActuateValve1Open(void *pData) {
  servo1.write(0);
  valve1_status_ = 1;
}

void ActuateValve1Close(void *pData) {
  servo1.write(90);
  valve1_status_ = 0;
}

Function valve1_open((const char *) "valve1_open", ActuateValve1Open, 0, 0);
Function valve1_close((const char *) "valve1_close", ActuateValve1Close, 0, 0);

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kServo1Pin, OUTPUT);
  pinMode(kMoisturePin, INPUT);

  // Attach modules
  servo1.attach(kServo1Pin);
}

void SetupThing() {
  // Setup Functions
  valve_with_soil_moisture.Add(valve1_open);
  valve_with_soil_moisture.Add(valve1_close);

  // Setup Values
  valve_with_soil_moisture.Add(valve1_status);
  valve_with_soil_moisture.Add(pot1_moisture);

  // Setup Thing
  valve_with_soil_moisture.Setup();
}

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { valve_with_soil_moisture.Loop(); }