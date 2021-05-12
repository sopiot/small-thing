include <thing.h>

// Module libraries

// Pins
static const int kradarPin = 2;

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
int SenseRadarStatus() { return (int)digitalRead(kradarPin); }

// Value declarations
// Value(name, sense_function, min, max, period(ms));
Value radar_value((const char *)"radar_value", SenseRadarStatus, 0, 2,
                     1000);

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
  pinMode(kradarPin, INPUT);

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

