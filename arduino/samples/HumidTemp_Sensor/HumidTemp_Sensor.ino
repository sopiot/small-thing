//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT Thing library
#include "thing.h"

// Module libraries
#include <DHT.h>

// Pins
static const int khumidPin = 2;

//----------------------------------------
// Modules
//----------------------------------------

// Modules
#define HUMID_VALUE "humid"
#define TEMP_VALUE "temp"
#define DHT_TYPE DHT11
//#define DHT_TYPE DHT22
DHT dht(khumidPin, DHT_TYPE);

//----------------------------------------
// Thing
//----------------------------------------

// Thing declaration
// Thing(class_name, alive_cycle, serial);
// Thing(class_name, serial);
Thing humid_thing((const char *)"Humid", 60, SafeSerial);

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables
int humid_status_;
int temp_status_;

// Getter functions of each Value variable
int SenseHumidStatus() { return humid_status_ = (int)dht.readHumidity(); }
int SenseTempStatus() { return temp_status_ = (int)dht.readTemperature(); }

// Value declarations
// Value(name, sense_function, min, max, period(ms));
Value humid_status((const char *)"humid_status", SenseHumidStatus, 0, 2, 3000);
Value temp_status((const char *)"temp_status", SenseTempStatus, 0, 2, 3000);

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
  pinMode(khumidPin, INPUT);

  // Attach modules
  dht.begin();
}

void SetupThing() {
  // Setup Functions

  // Setup Values
  humid_thing.Add(humid_status);
  humid_thing.Add(temp_status);

  // Setup Thing
  humid_thing.Setup();
}

//----------------------------------------
// Main
//----------------------------------------

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { humid_thing.Loop(); }
