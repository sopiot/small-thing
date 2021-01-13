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
Servo servo2; 

// Pins
static const int kServo1Pin = 9;
static const int kServo2Pin = 10;

enum valve_status {
    valve_close = 0,
    valve_open = 1
};

//----------------------------------------
// Thing
//----------------------------------------
// Thing(class_name, alive_cycle, serial);
// Thing(class_name, serial);
Thing valve_act1_thing((const char *)"SmartPotValve", 60, SafeSerial);

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables
int valve1_status_;
int valve2_status_;

// Getter functions of each Value variable
int SenseValve1Status() { return valve1_status_; }
int SenseValve2Status() { return valve2_status_; }

Value valve1_status((const char *)"valve1_status", SenseValve1Status, 0, 2, 
                    10000);
Value valve2_status((const char *)"valve2_status", SenseValve2Status, 0, 2, 
                    10000);

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

void ActuateValve1Open(void *pData) {
    servo1.write(0);
    delay(300);
    valve1_status_ = valve_open;
}

void ActuateValve1Close(void *pData) {
    servo1.write(90);
    delay(300);
    valve1_status_ = valve_close;
}

void ActuateValve2Open(void *pData) {
    servo2.write(0);
    delay(300);
    valve2_status_ = valve_open;
}

void ActuateValve2Close(void *pData) {
    servo2.write(90);
    delay(300);
    valve2_status_ = valve_close;
}

// Function declarations
// Function(name, actuate_function, arguments_num, function_attributes_num);
Function valve1_open((const char *)"valve1_open", ActuateValve1Open, 0, 0);
Function valve1_close((const char *)"valve1_close", ActuateValve1Close, 0, 0);
Function valve2_open((const char *)"valve2_open", ActuateValve2Open, 0, 0);
Function valve2_close((const char *)"valve2_close", ActuateValve2Close, 0, 0);

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kServo1Pin, OUTPUT);
  pinMode(kServo2Pin, OUTPUT);

  // Attach modules
  servo1.attach(kServo1Pin);
  servo2.attach(kServo2Pin);
}

void SetupThing() {
    //Setup Functions
    valve_act1_thing.Add(valve1_open);
    valve_act1_thing.Add(valve1_close);
    valve_act1_thing.Add(valve2_open);
    valve_act1_thing.Add(valve2_close);

    //Setup Values
    valve_act1_thing.Add(valve1_status);
    valve_act1_thing.Add(valve2_status);

    //Setup Thing
    valve_act1_thing.Setup();
}

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { valve_act1_thing.Loop(); }