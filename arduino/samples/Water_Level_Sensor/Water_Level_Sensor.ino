//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT Thing library
#include "thing.h"
#define WATER_LEVEL_SENSOR_NUM 2

// Module libraries
// Pins
const int kWaterLevelPin[WATER_LEVEL_SENSOR_NUM] = {2, 3};

//----------------------------------------
// Modules
//----------------------------------------

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
int water_level_[2];
double water_percentage_ = 0.0;
double unit = 100.0 / WATER_LEVEL_SENSOR_NUM;

// Getter functions of each Value variable
int SenseWaterLevel() {
  double sum = 0;
  for (int i = 0; i < WATER_LEVEL_SENSOR_NUM; i++) {
    water_level_[i] = digitalRead(kWaterLevelPin[i]);
    sum += (water_level_[i] == 0) ? unit : 0.0;
  }
  return (int)sum;
}

// Value declarations
// Value(name, sense_function, min, max, period(ms));
Value water_level((const char *)"water_level", SenseWaterLevel, 0, 100, 1000);

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
  pinMode(kWaterLevelPin[0], INPUT);
  pinMode(kWaterLevelPin[1], INPUT);
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
