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

class DustSensor : public Thing {
 public:
  DustSensor(const std::string& broker_ip, int broker_port,
             const std::string& class_name, int alive_cycle);
  ~DustSensor();

  //------------------------------------
  // Value adder
  //------------------------------------
  void addSenseDust(DoubleValue pfunc);

  //------------------------------------
  // Value setter
  //------------------------------------
  void setDust(double dust);

  //------------------------------------
  // Value getter
  //------------------------------------
  double getDust();

  //------------------------------------
  // Function adder
  //------------------------------------

  //------------------------------------
  // etc functions
  //------------------------------------

  //------------------------------------
  // Setup functions
  //------------------------------------
  void Loop(int argc, char* argv[]);
  void SetupThing();

 private:
  Value* value_dust_;

  double dust_;

  bool required = true;

  int RequireChecker();
  void SetupBroker(char* argv[]);
};