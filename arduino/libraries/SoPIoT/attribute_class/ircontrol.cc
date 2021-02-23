#include "ircontrol.h"
#include "thing.h"

using namespace sopiot;
using namespace std;

IRControl::IRControl(const std::string &broker_ip, int broker_port,
                     const std::string &class_name, int alive_cycle)
    : Thing(broker_ip, broker_port, class_name, alive_cycle) {
  value_ir_status_ = NULL;
  ir_status_ = 0;
}

IRControl::~IRControl() { delete value_ir_status_; }

// Value adder
void IRControl::addSenseIRStatus(IntegerValue pfunc) {
  value_ir_status_ = new Value("ir_status", pfunc, 0, 10, 1000);
}

//------------------------------------
// Value setter
//------------------------------------
void IRControl::setIRStatus(int ir_status) { ir_status_ = ir_status; }

//------------------------------------
// Value Getter
//------------------------------------
int IRControl::getIRStatus() { return ir_status_; }

//------------------------------------
// Function callback functions
//------------------------------------
void IRControl::SendSignal(VoidFunction pfunc) {
  send_signal_ = new Function("send_signal", pfunc);
  static Argument signal_array("arg_signal_array", 0, 1000, STRING);
  static Argument signal_length("arg_signal_length", 0, 100, INTEGER);
  send_signal_->Add_argument(signal_array);
  send_signal_->Add_argument(signal_length);
}

//------------------------------------
// etc functions
//------------------------------------

void IRControl::SetupThing() {
  RequireChecker();

  // Setup Values
  Add(*value_ir_status_);

  // Setup Functions
  Add(*send_signal_);

  // Setup Thing
  Thing::Setup();
}

void IRControl::SetupBroker(char *argv[]) {
  std::string broker_ip = std::string(argv[1]);
  int broker_port = atoi(argv[2]);

  Thing::SetBroker(broker_ip, broker_port);
}

int IRControl::RequireChecker() {
  SOPLOG("RequireChecker Start\n");

  if (value_ir_status_ == NULL) {
    SOPLOG("Please fill out the callback function \"SenseIRStatus\".\n");
    required = false;
    exit(0);
  }

  if (send_signal_ == NULL) {
    SOPLOG("Please fill out the callback function \"SensSignal\".\n");
    required = false;
    exit(0);
  }

  if (required) {
    static Attribute attribute("IRControl", "IRControl", STRING);
    AddPredefine(attribute);
  } else {
    SOPLOG("Please fill out all the callback functions\n");
    exit(0);
  }
}

void IRControl::Loop(int argc, char *argv[]) {
  if (argc == 3)
    SetupBroker(argv);

  while (true) {
    Publish();
    CAPTime_Sleep(100);
  }

  Disconnect();
}