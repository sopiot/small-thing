// System Library
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <iostream>
#include <vector>

// SoPIoT Thing Library
#include "thing.h"
#include "utils.h"

// CAP Common Libary
#include "CAPTime.h"

using namespace sopiot;
using namespace std;

class IRControl : public Thing {
 public:
  IRControl(const std::string& broker_ip, int broker_port,
            const std::string& class_name, int alive_cycle);
  ~IRControl();

  //------------------------------------
  // Value adder
  //------------------------------------
  void addSenseIRStatus(IntegerValue pfunc);

  //------------------------------------
  // Value setter
  //------------------------------------
  void setIRStatus(int ir_status);

  //------------------------------------
  // Value getter
  //------------------------------------
  int getIRStatus();

  //------------------------------------
  // Function adder
  //------------------------------------
  void SendSignal(VoidFunction pfunc);

  //------------------------------------
  // etc functions
  //------------------------------------

  //------------------------------------
  // Setup functions
  //------------------------------------
  void Loop(int argc, char* argv[]);
  void SetupThing();

 private:
  Value* value_ir_status_;

  Function* send_signal_;

  int ir_status_;

  bool required = true;

  int RequireChecker();
  void SetupBroker(char* argv[]);
};