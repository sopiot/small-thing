//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT Thing library
#include <thing.h>

// Module libraries

// Pins
static const int kPWNPin = 7;
static const int kAnalogOutputPin = A2;

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
Thing CO2Sensor_thing((const char *)"CO2Sensor", 60, SafeSerial);

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables
int analog_ppm_;
int pwm_ppm_;

// Getter functions of each Value variable

int SenseAnalogOutputPPM() {
  return analogRead(kAnalogOutputPin);
}

int SensePWMOutputPPM() {
  
  return analog_ppm_; 
}

// Value declarations
// Value(name, sense_function, min, max, period(ms));
Value analogPPM((const char *)"co2_level", SenseAnalogOutputPPM, 0, 2, 5000);
// Value pwmPPM((const char *)"pwmPPM", SensePWMOutputPPM, 0, 2, 5000);

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
  pinMode(kAnalogOutputPin, INPUT);
//   pinMode(kPWNPin, INPUT);

  // Attach modules
}

void SetupThing() {
  // Setup Functions

  // Setup Values
  CO2Sensor_thing.Add(analogPPM);
  // CO2Sensor_thing.Add(pwmPPM);

  // Setup Thing
  CO2Sensor_thing.Setup();
}

//----------------------------------------
// Main
//----------------------------------------

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { CO2Sensor_thing.Loop(); }