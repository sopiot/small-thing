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

class Valve : public Thing {
 public:
  Valve(const std::string& broker_ip, int broker_port,
        const std::string& class_name, int alive_cycle);
  ~Valve();

  //------------------------------------
  // Value adder
  //------------------------------------
  void addSenseValveStatus(IntegerValue pfunc);

  //------------------------------------
  // Value setter
  //------------------------------------
  void setValveStatus(int valve_status);

  //------------------------------------
  // Value getter
  //------------------------------------
  int getValveStatus();

  //------------------------------------
  // Function adder
  //------------------------------------
  void ValveOpen(VoidFunction pfunc);
  void ValveClose(VoidFunction pfunc);

  //------------------------------------
  // etc functions
  //------------------------------------

  //------------------------------------
  // Setup functions
  //------------------------------------
  void Loop(int argc, char* argv[]);
  void SetupThing();

 private:
  Value* vavlue_valve_status_;

  Function* valve_open_;
  Function* valve_close_;

  int valve_status_;

  bool required = true;

  int RequireChecker();
  void SetupBroker(char* argv[]);
};