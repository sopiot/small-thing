//----------------------------------------
// Libraries
//----------------------------------------

//SoPIoT thing library
#include <thing.h>

// Module libraries
#include <Servo.h>

//--------------------------------------------------
// Modules
// -------------------------------------------------

// Modules
Servo servo1;

// Pins
static const int kServo1Pin = 9;

enum valve_status {
    valve_close = 0,
    valve_open = 1
};

//----------------------------------------
// Thing
//----------------------------------------
// Thing(class_name, alive_cycle, serial);
// Thing(class_name, serial);
Thing valve_with_soil_moisture((const char *)"valve_with_soil_moisture", 60, SafeSerial);

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables
int valve1_status_;
int pot1_moisture_;

// Getter functions of each Value variable
int SenseValve1Status() { return valve1_status_; }
int SensePot1Moisture() { return pot1_moisture_; }

Value valve1_status((const char *)"valve1_status", SenseValve1Status, 0, 2, 
                    10000);
Value pot1_moisture((const char *)"pot1_status", SensePot1Status, 0, 2, 
                    10000);

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

void ActuateValve1Open(void *pData)
{
    servo1.write(0);
    delay(300);
    valve1_status = valve_open;
}

void ActuateValve1Close(void *pData)
{
    servo1.write(90);
    delay(300);
    valve1_status = valve_close;
}

Function valve1_open((const char *)"valve1_open", ActuateValve1Open, 0, 0);
Function valve1_close((const char *)"valve1_close", ActuateValve1Close, 0, 0);

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kServo1Pin, OUTPUT);

  // Attach modules
  servo1.attach(kServo1Pin);
}

void SetupThing() {
    //Setup Functions
    valve_with_soil_moisture.Add(ActuateValve1Open);
    valve_with_soil_moisture.Add(ActuateValve1Close);

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