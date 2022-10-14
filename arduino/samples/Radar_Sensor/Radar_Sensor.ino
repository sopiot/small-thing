#include <thing.h>

#define WINDOW_SIZE 10

// Module libraries

// Pins
static const int kRadarPin = 2;

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
Thing radar_thing((const char *)"Radar", 60, SafeSerial);

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables

// Getter functions of each Value variable
int SenseRadarStatus() {
  static int time_window[WINDOW_SIZE] = {0};
  static int index = 0;
  int window_sum = 0;

  if (index > WINDOW_SIZE - 1) {
    index = 0;
  }

  if (digitalRead(kRadarPin) == 0) {
    time_window[index] = 1;
  } else {
    time_window[index] = 0;
  }

  for (int i = 0; i < WINDOW_SIZE; i++) {
    window_sum += time_window[i];
  }

  if (window_sum > 0) {
    return 1;
  } else {
    return 0;
  }
}

// Value declarations
// Value(name, sense_function, min, max, period(ms));
Value radar_value((const char *)"radar_value", SenseRadarStatus, 0, 2, 200);

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
  pinMode(kRadarPin, INPUT);

  // Attach modules
}

void SetupThing() {
  // Setup Functions

  // Setup Values
  radar_thing.Add(radar_value);

  // Setup Thing
  radar_thing.Setup();
}

//----------------------------------------
// Main
//----------------------------------------

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { radar_thing.Loop(); }
