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

class HumiditySensor : public Thing {
 public:
  HumiditySensor(const std::string& broker_ip, int broker_port,
                 const std::string& class_name, int alive_cycle);
  ~HumiditySensor();

  //------------------------------------
  // Value adder
  //------------------------------------
  void addSenseHumidity(DoubleValue pfunc);

  //------------------------------------
  // Value setter
  //------------------------------------
  void setHumidity(double distance);

  //------------------------------------
  // Value getter
  //------------------------------------
  double getHumidity();

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
  Value* value_humidity_;

  double humidity_;

  bool required = true;

  int RequireChecker();
  void SetupBroker(char* argv[]);
};