#include "../thing.h"
#include "templete.h"

Templete::Templete(const std::string& broker_ip, int broker_port,
                   const std::string& class_name, int alive_cycle)
    : Thing(broker_ip, broker_port, class_name, alive_cycle) {
  value_status_ = NULL;
}

Templete::~Templete() { delete value_status_; }

//------------------------------------
// Value adder
//------------------------------------
void Templete::addSenseStatus(IntegerValue pfunc) {
  value_status_ = new Value("Value_templete", pfunc, 0, 2500, 1000);
}

//------------------------------------
// Function adder
//------------------------------------
void Templete::addActuateAction(VoidFunction pfunc) {
  function_action_ = new Function("Function_templete", pfunc);
}

int Templete::SetupThing() {
  if (RequireChecker() != 0) return -1;

  // Setup Values
  Add(*value_status_);

  // Setup Functions

  // Setup Thing
  Thing::Setup();

  return 0;
}

int Templete::RequireChecker() {
  required = true;
  if (value_status_ == NULL) {
    SOPLOG("Please fill out the callback function \"SenseValueStatus\".\n");
    required = false;
    return -1;
  }

  if (required) {
    static Attribute attribute("templete", "templete", STRING);
    AddPredefine(attribute);
    return 0;
  } else {
    SOPLOG("Please fill out all the callback functions\n");
    return -1;
  }
}

void Templete::Loop() {
  while (true) {
    Publish();
    CAPTime_Sleep(100);
  }

  Disconnect();
}