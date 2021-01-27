//----------------------------------------
// Libraries
//----------------------------------------

//SoPIoT thing library
#include <thing.h>

// Module libraries
#include <Servo.h>

// Pins
static const int kServo1Pin = 9;
static const int kMoisturePin = A1;
//#define kMoisturePin A1

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
Thing valve_with_soil_moisture((const char *)"SmartPotVSoil", 60, SafeSerial);

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables
// int valve1_status_ = 0;
// int pot1_moisture_ = 0;
double valve1_status_ = 0.0;
double pot1_moisture_ = 0.0;

// Getter functions of each Value variable
//int SenseValve1Status() { return valve1_status_; }
double SenseValve1Status() { return valve1_status_; }

//int SensePot1Moisture() {
double SensePot1Moisture() {
  double moisture_vol = 0.0;
  for (int i = 0; i < 10; i++) {
    moisture_vol = moisture_vol + analogRead(kMoisturePin)/1024.0 * 100.0;
    delay(1);
  }
  pot1_moisture_ = moisture_vol / 10.0;
  return pot1_moisture_;
}

Value valve1_status((const char *)"valve1_status", SenseValve1Status, 0.0, 2.0,
                    1000);
Value pot1_moisture((const char *)"pot1_moisture", SensePot1Moisture, 0.0, 2000.0,
                    3000);

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

void ActuateValve1Open(void *pData) {
  SOPLOGLN(F("[MOTOR DEBUG]: Actuate_Valve1_Open!!"));
  servo1.write(0);
  SOPLOGLN(F("[MOTOR DEBUG]: Actuate_Valve1_Open finished!!"));
  delay(300);
  valve1_status_ = 1.0;
}

void ActuateValve1Close(void *pData) {
  SOPLOGLN(F("[MOTOR DEBUG]: Actuate_Valve1_Close!!"));
  servo1.write(90);
  SOPLOGLN(F("[MOTOR DEBUG]: Actuate_Valve1_Close finished!!"));
  delay(300);
  valve1_status_ = 0.0;
}

Function valve1_open((const char *)"valve1_open", ActuateValve1Open, 0, 0);
Function valve1_close((const char *)"valve1_close", ActuateValve1Close, 0, 0);

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kServo1Pin, OUTPUT);
  pinMode(kMoisturePin, INPUT);
  // Attach modules
  servo1.attach(kServo1Pin);
}

void SetupThing() {
  //Setup Functions
  valve_with_soil_moisture.Add(valve1_open);
  valve_with_soil_moisture.Add(valve1_close);

  //Setup Values
  valve_with_soil_moisture.Add(valve1_status);
  valve_with_soil_moisture.Add(pot1_moisture);
  //Setup Thing
  valve_with_soil_moisture.Setup();
}

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { valve_with_soil_moisture.Loop(); }