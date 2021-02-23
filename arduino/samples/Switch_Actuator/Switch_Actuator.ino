//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT Thing library
#include "switch.h"
#include <thing.h>

// Module libraries
#include <Servo.h>

// Pins
static const int kServo1Pin = 8;
static const int kServo2Pin = 9;

//----------------------------------------
// Modules
//----------------------------------------

// Modules
Servo servo1;
Servo servo2;

// Global variables
int switch_status_ = 0;

//----------------------------------------
// Thing
//----------------------------------------
Switch *thing = new Switch("147.46.174.110", 11883, "Switch_Templete", 60);

//----------------------------------------
// Value callback functions
//----------------------------------------
int SenseSwitchStatus() { return switch_status_; }

//----------------------------------------
// Functions callback functions
//----------------------------------------
void ActuateSwitchOn() {
  servo2.attach(kServo2Pin);
  int power = 1, res;
  int mid = 90, turn = 45;

  servo2.write(mid - turn);
  delay(300);
  servo2.write(mid);
  delay(300);
  switch_status_ = 1;
  servo2.detach();
}
void ActuateSwitchOff() {
  servo1.attach(kServo1Pin);
  int power = 1, res;
  int mid = 90, turn = -45;

  servo1.write(mid - turn);
  delay(300);
  servo1.write(mid);
  delay(300);
  switch_status_ = 0;
  servo1.detach();
}

//----------------------------------------
// Setup
//----------------------------------------
void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kServo1Pin, OUTPUT);
  pinMode(kServo2Pin, OUTPUT);

  // Attach modules
  servo1.attach(kServo1Pin);
  servo1.attach(kServo2Pin);
}

void Setup() {
  SetupSerial();
  SetupModules();

  // Setup Values
  Actuate_thing->addSenseSwitchStatus(switch_status);

  // Setup Functions
  Actuate_thing->addActuateSwitchOn(switch_on);
  Actuate_thing->addActuateSwitchOff(switch_off);

  // Setup Thing
  Actuate_thing.Setup();

  return 0;
}

//----------------------------------------
// Main
//----------------------------------------
void loop() {
  if (Setup() == 0) {
    thing->Loop();
    return 0;
  } else
    return -1;
}
