#include "switch.h"

#include <Arduino.h>

using namespace sopiot;
using namespace std;

Switch::Switch(const std::string& broker_ip, int broker_port,
               const std::string& class_name, int alive_cycle)
    : Thing(broker_ip, broker_port, class_name, alive_cycle) {
  value_switch_status_ = NULL;
}

Switch::~Switch() { delete value_switch_status_; }

//------------------------------------
// Value adder
//------------------------------------
void Switch::addSenseSwitchStatus(IntegerValue pfunc) {
  value_switch_status_ = new Value("switch_status", pfunc, 0, 10, 1000);
}

//------------------------------------
// Function adder
//------------------------------------
void Switch::addActuateSwitchOn(VoidFunction pfunc) {
  function_switch_on_ = new Function("switch_on", pfunc);
}

void Switch::addActuateSwitchOff(VoidFunction pfunc) {
  function_switch_off_ = new Function("switch_off", pfunc);
}

//------------------------------------
// etc functions
//------------------------------------

int Switch::SetupThing() {
  if (RequireChecker() != 0) return -1;

  // Setup Values
  Add(*value_switch_status_);

  // Setup Functions
  Add(*function_switch_on_);
  Add(*function_switch_off_);

  // Setup Thing
  Thing::Setup();

  return 0;
}

int Switch::RequireChecker() {
  required = true;
  if (value_status_ == NULL) {
    SOPLOG("Please fill out the callback function \"SenseSwitchStatus\".\n");
    required = false;
    return -1;
  }

  required = true;
  if (function_switch_on_ == NULL) {
    SOPLOG("Please fill out the callback function \"SenseSwitchOn\".\n");
    required = false;
    return -1;
  }
  required = true;
  if (function_switch_off_ == NULL) {
    SOPLOG("Please fill out the callback function \"SenseSwitchOff\".\n");
    required = false;
    return -1;
  }

  if (required) {
    static Attribute attribute("Switch", "Switch", STRING);
    AddPredefine(attribute);
    return 0;
  } else {
    SOPLOG("Please fill out all the callback functions\n");
    return -1;
  }
}

void Switch::Loop() {
  while (true) {
    Publish();
    CAPTime_Sleep(100);
  }

  Disconnect();
}