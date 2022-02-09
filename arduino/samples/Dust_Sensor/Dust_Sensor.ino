//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT Thing library
#include "thing.h"

// Module libraries
#include <pm2008_i2c.h>

// Pins
static const int kTriggerPin = 3;
static const int kReceivePin = 4;

//----------------------------------------
// Modules
//----------------------------------------

// Modules
PM2008_I2C pm2008_i2c;
/************************
red    : 5V
black  : GND0
white  : A4 (SDA)
green  : A5 (SCL)
yellow : jumper (connect to GND -> i2c, connect to VCC or floating -> uart)
************************/

//----------------------------------------
// Thing
//----------------------------------------

// Thing declaration
// Thing(class_name, alive_cycle, serial);
// Thing(class_name, serial);
Thing dust_sensor_thing((const char*)"Dust2", 60, SafeSerial);

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables
int dust_status_;

// Getter functions of each Value variable
int SenseDustStatus() {
  uint8_t ret = pm2008_i2c.read();
  if (ret == 0) {
    // Serial.print("PM 1.0 (GRIMM) : ");
    // Serial.println(pm2008_i2c.pm1p0_grimm);
    // Serial.print("PM 2.5 (GRIMM) : : ");
    // Serial.println(pm2008_i2c.pm2p5_grimm);
    // Serial.print("PM 10 (GRIMM) : : ");
    // Serial.println(pm2008_i2c.pm10_grimm);
    // Serial.print("PM 1.0 (TSI) : ");
    // Serial.println(pm2008_i2c.pm1p0_tsi);
    // Serial.print("PM 2.5 (TSI) : : ");
    // Serial.println(pm2008_i2c.pm2p5_tsi);
    // Serial.print("PM 10 (TSI) : : ");
    // Serial.println(pm2008_i2c.pm10_tsi);
    // Serial.print("Number of 0.3 um : ");
    // Serial.println(pm2008_i2c.number_of_0p3_um);
    // Serial.print("Number of 0.5 um : ");
    // Serial.println(pm2008_i2c.number_of_0p5_um);
    // Serial.print("Number of 1 um : ");
    // Serial.println(pm2008_i2c.number_of_1_um);
    // Serial.print("Number of 2.5 um : ");
    // Serial.println(pm2008_i2c.number_of_2p5_um);
    // Serial.print("Number of 5 um : ");
    // Serial.println(pm2008_i2c.number_of_5_um);
    // Serial.print("Number of 10 um : ");
    // Serial.println(pm2008_i2c.number_of_10_um);

    return (int)(pm2008_i2c.number_of_1_um);
  }

  return -1;
}

// Value declarations
// Value(name, sense_function, min, max, period(ms));
Value dust_status((const char*)"dust_status", SenseDustStatus, 0, 2, 3000);

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

// Function declarations
// Function(name, actuate_function, arguments_num, function_attributes_num);

//----------------------------------------
// Setup
//----------------------------------------

void SetupSerial() { SafeSerial.begin(115200); }

void SetupModules() {
  // Setup Pin mode

  // Attach modules
  pm2008_i2c.begin();
  pm2008_i2c.command();
}

void SetupThing() {
  // Setup Functions

  // Setup Values
  dust_sensor_thing.Add(dust_status);

  // Setup Thing
  dust_sensor_thing.Setup();
}

//----------------------------------------
// Main
//----------------------------------------

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { dust_sensor_thing.Loop(); }
