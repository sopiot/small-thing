//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT Thing library
#include <thing.h>

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
double pump_status_ = 0.0;

// Getter functions of each Value variable
double SensePumpStatus() { return pump_status_; }

Value pump_status((const char *)"pump_status", SensePumpStatus, 0.0, 2.0, 3000);

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

void ActuatePumpOn(void *pData) {
  SOPLOGLN(F("[MOTOR DEBUG]: Actuate_Pump_Open!!"));
  digitalWrite(kRelayPin,HIGH); 
  delay(500);
  pump_status_ = 1.0;
}

void ActuatePumpOff(void *pData) {
  SOPLOGLN(F("[MOTOR DEBUG]: Actuate_Pump_Open!!"));
  digitalWrite(kRelayPin,LOW); 
  delay(500);
  pump_status_ = 0.0;
}

Function pump_on((const char *)"pump_on", ActuatePumpOn, 0, 0);
Function pump_off((const char *)"pump_off", ActuatePumpOff, 0, 0);

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kRelayPin, OUTPUT);
}

void SetupThing() {
  //Setup Functions
  water_pump.Add(pump_on);
  water_pump.Add(pump_off);

  //Setup Values
  water_pump.Add(pump_status);

  //Setup Thing
  water_pump.Setup();
}

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { water_pump.Loop(); }