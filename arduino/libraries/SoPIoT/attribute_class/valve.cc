#include "valve.h"
#include "thing.h"

using namespace sopiot;
using namespace std;

Valve::Valve(const std::string &broker_ip, int broker_port,
             const std::string &class_name, int alive_cycle)
    : Thing(broker_ip, broker_port, class_name, alive_cycle) {
  vavlue_valve_status_ = NULL;
  valve_status_ = 0;
}

Valve::~Valve() { delete vavlue_valve_status_; }

// Value adder
void Valve::addSenseValveStatus(IntegerValue pfunc) {
  vavlue_valve_status_ = new Value("valve", pfunc, 0, 10, 1000);
}

//------------------------------------
// Value setter
//------------------------------------
void Valve::setValveStatus(int valve_status) { valve_status_ = valve_status; }

//------------------------------------
// Value Getter
//------------------------------------
int Valve::getValveStatus() { return valve_status_; }

//------------------------------------
// Function callback functions
//------------------------------------
void Valve::ValveOpen(VoidFunction pfunc) {
  valve_open_ = new Function("valve_open", pfunc);
}

void Valve::ValveClose(VoidFunction pfunc) {
  valve_close_ = new Function("valve_close", pfunc);
}

//------------------------------------
// etc functions
//------------------------------------

void Valve::SetupThing() {
  RequireChecker();

  // Setup Values
  Add(*vavlue_valve_status_);

  // Setup Functions
  Add(*valve_open_);
  Add(*valve_close_);

  // Setup Thing
  Thing::Setup();
}

void Valve::SetupBroker(char *argv[]) {
  std::string broker_ip = std::string(argv[1]);
  int broker_port = atoi(argv[2]);

  Thing::SetBroker(broker_ip, broker_port);
}

int Valve::RequireChecker() {
  SOPLOG("RequireChecker Start\n");

  if (vavlue_valve_status_ == NULL) {
    SOPLOG("Please fill out the callback function \"SenseValveStatus\".\n");
    required = false;
    exit(0);
  }

  if (valve_open_ == NULL) {
    SOPLOG("Please fill out the callback function \"SenseValveStatus\".\n");
    required = false;
    exit(0);
  }

  if (valve_close_ == NULL) {
    SOPLOG("Please fill out the callback function \"SenseValveStatus\".\n");
    required = false;
    exit(0);
  }

  if (required) {
    static Attribute attribute("Valve", "Valve", STRING);
    AddPredefine(attribute);
  } else {
    SOPLOG("Please fill out all the callback functions\n");
    exit(0);
  }
}

void Valve::Loop(int argc, char *argv[]) {
  if (argc == 3)
    SetupBroker(argv);

  while (true) {
    Publish();
    CAPTime_Sleep(100);
  }

  Disconnect();
}