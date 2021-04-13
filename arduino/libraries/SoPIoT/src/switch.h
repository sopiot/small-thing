// System Library
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <iostream>
#include <vector>

// SoPIoT Thing Library
#include "../thing.h"
#include "../utils.h"

class Switch : public Thing {
 public:
  Switch(const std::string& broker_ip, int broker_port,
         const std::string& class_name, int alive_cycle);
  ~Switch();

  //------------------------------------
  // Value adder
  //------------------------------------
  void addSenseSwitchStatus(IntegerValue pfunc);

  //------------------------------------
  // Function adder
  //------------------------------------
  void addActuateSwitchOn(VoidFunction);
  void addActuateSwitchOff(VoidFunction);

  //------------------------------------
  // etc functions
  //------------------------------------

  //------------------------------------
  // Setup functions
  //------------------------------------
  void Loop();
  int SetupThing();

 private:
  Value* value_switch_status_;

  Function* function_switch_on_;
  Function* function_switch_off_;

  bool required = true;

  int RequireChecker();
};