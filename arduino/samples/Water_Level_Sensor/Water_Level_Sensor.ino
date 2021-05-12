//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT Thing library
#include <thing.h>
// Module libraries

// Pins
static const int kWaterLevelPin = 13;

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
Thing water_level_sensor((const char *)"WaterLevelSensor", 60, SafeSerial);

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables
int water_level_;

// Getter functions of each Value variable
int SenseWaterLevel() {
  water_level_ = digitalRead(kWaterLevelPin);
  return water_level_;
}

// Value declarations
// Value(name, sense_function, min, max, period(ms));
Value water_level((const char *)"water_level", SenseWaterLevel, 0, 1000, 1000);

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

// Function declarations
// Function(name, actuate_function, arguments_num, function_attributes_num);

//----------------------------------------
// Setup
//----------------------------------------

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kWaterLevelPin, INPUT);

  // Attach modules
}

void SetupThing() {
  // Setup Functions

  // Setup Values
  water_level_sensor.Add(water_level);

  // Setup Thing
  water_level_sensor.Setup();
}

//----------------------------------------
// Main
//----------------------------------------

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { water_level_sensor.Loop(); }