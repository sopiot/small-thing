// System Library
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <vector>

// SoPIoT Thing Library
#include "thing.h"
#include "utils.h"

// CAP Common Libary
#include "CAPTime.h"

using namespace sopiot;
using namespace std;

class Templete : public Thing {
public:
  Templete(const std::string &broker_ip, int broker_port,
           const std::string &class_name, int alive_cycle);
  ~Templete();

  //------------------------------------
  // Value adder
  //------------------------------------
  void addSenseStatus(IntegerValue pfunc);

  //------------------------------------
  // Function adder
  //------------------------------------
  void addActuateAction(VoidFunction pfunc);

  //------------------------------------
  // Setup
  //------------------------------------
  void Loop();
  int SetupThing();

private:
  Value *value_status_;

  Function *function_action_;

  bool required = true;

  int RequireChecker();
};