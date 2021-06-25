//----------------------------------------
// Libraries
//----------------------------------------
// SoPIoT Thing library
#include <thing.h>
// Module libraries
// Pins
static const int kLight1Pin = A0;
//----------------------------------------
// Modules
//----------------------------------------
// Modules
//----------------------------------------
// Thing
//----------------------------------------
// Thing declaration
// Thing(class_name, alive_cycle, serial)
;
// Thing(class_name, serial);
Thing light_sensor((const char *)"Light", 60, SafeSerial);
//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------
// Value variables
int brightness_;
// Getter functions of each Value variable
int SenseBrightness() { return analogRead(kLight1Pin); }
int Actuatefunction(void* args) { 
  int int_arg;
  SOPLOGLN("in Actuatefunction");
  GetIntArgumentByName(args, "args_test", &int_arg);
  SOPLOGLN("int_arg: %d", int_arg);
  return int_arg;
 }
// Value declarations
// Value(name, sense_function, min, max, period(ms));
Value brightness((const char *)"brightness", SenseBrightness, 0, 2048, 1000);
Function func("dummy_function", Actuatefunction, 1);
Argument arg("args_test", 0, 100, INTEGER);

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
  // Attach modules
}
void SetupThing() {
  // Setup Functions
  // Setup Values
  delay(2000);
  brightness.AddTag("light");
  brightness.AddTag("livingroom");
  func.AddTag("livingroom2");
  func.AddTag("livingroom3");
  light_sensor.Add(brightness);
  func.AddArgument(arg);
  light_sensor.Add(func);
  // Setup Thing

 

  light_sensor.Setup();
}
//----------------------------------------
// Main
//----------------------------------------
void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}
void loop() { light_sensor.Loop(); }