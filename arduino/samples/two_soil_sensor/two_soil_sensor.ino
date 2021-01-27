//----------------------------------------
// Libraries
//----------------------------------------

//SoPIoT thing library
#include <thing.h>

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
double pot1_moisture_ = 0.0;
double pot2_moisture_ = 0.0;

// Getter functions of each Value variable
double SensePot1Moisture() {
  double moisture_vol = 0.0;
  for (int i = 0; i < 10; i++) {
    moisture_vol = moisture_vol + analogRead(kMoisture1Pin)/1024.0 * 100.0;
    delay(1);
  }
  pot1_moisture_ = moisture_vol / 10.0;
  return pot1_moisture_;
}

double SensePot2Moisture() {
  double moisture_vol = 0.0;
  for (int i = 0; i < 10; i++) {
    moisture_vol = moisture_vol + analogRead(kMoisture2Pin)/1024.0 * 100.0;
    delay(1);
  }
  pot2_moisture_ = moisture_vol / 10.0;
  return pot2_moisture_;
}

Value pot1_moisture((const char *)"pot1_moisture", SensePot1Moisture, 0.0, 2000.0, 3000);
Value pot2_moisture((const char *)"pot2_moisture", SensePot2Moisture, 0.0, 2000.0, 3000);

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  pinMode(kMoisture1Pin, INPUT);
  pinMode(kMoisture2Pin, INPUT);
}

void SetupThing() {

  //Setup Values
  two_soil_sensor.Add(pot1_moisture);
  two_soil_sensor.Add(pot2_moisture);
  //Setup Thing
  two_soil_sensor.Setup();
}

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { two_soil_sensor.Loop(); }