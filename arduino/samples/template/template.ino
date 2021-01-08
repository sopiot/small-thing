//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT Thing library
#include <thing_client.h>

// Module libraries
#include <Servo.h>

//----------------------------------------
// Modules
//----------------------------------------

// Modules
Servo servo1;

// Pins
static const int kServo1Pin = 8;

//----------------------------------------
// Thing
//----------------------------------------

// Thing declaration
// ThingClient(class_name, alive_cycle, serial);
// ThingClient(class_name, serial);
ThingClient thing((const char *)"Template_001", 60, SafeSerial);

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

void ActuateSwitchOn(void *pData) {
  // int res;
  // res = GetIntArgumentByName(pData, "angle", &angle);
  // if (res == -1)
  //   return;

  // servo.write(angle);
  servo.write(45);
  delay(300);
  servoState = 1;
}

// Function declarations
// Function(name, actuate_function, arguments_num, function_attributes_num);
Function switch_on((const char *)"SwitchOn", ActuateSwitchOn, 0, 0);

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables
int switch_status_;

// Getter functions of each Value variable
int SenseSwitchStatus() const { return switch_status_; }

// Value declarations
// Value(name, sense_function, min, max, period(ms));
Value switch_status((const char *)"SwitchStatus", SenseSwitchStatus, 0, 2,
                    10000);

//----------------------------------------
// Main
//----------------------------------------

void SetupSerial() {
#if BOARD_SERIAL_IS_ONE
  Serial1.begin(115200);
#else
  Serial.begin(115200);
#endif
}

void SetupModules() {
  // Setup Pin mode
  pinMode(kServo1Pin, OUTPUT);

  // Attach modules
  servo1.attach(kServo1Pin);
}

void SetupThing() {
  // Setup Functions
  thing.Add(functionSwitchOn);

  // Setup Values
  thing.Add(SW_status);

  // Setup Thing
  thing.Setting();
}

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { thing.DoLoop(); }
