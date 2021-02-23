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

class LightSensor : public Thing {
public:
  LightSensor(const std::string &broker_ip, int broker_port,
              const std::string &class_name, int alive_cycle);
  ~LightSensor();

  //------------------------------------
  // Value adder
  //------------------------------------
  void addSenseBrightness(IntegerValue pfunc);

  //------------------------------------
  // Value setter
  //------------------------------------
  void setBrightness(int brightness);

  //------------------------------------
  // Value getter
  //------------------------------------
  int getBrightness();

  //------------------------------------
  // Function adder
  //------------------------------------

  //------------------------------------
  // etc functions
  //------------------------------------

  //------------------------------------
  // Setup functions
  //------------------------------------
  void Loop(int argc, char *argv[]);
  void SetupThing();

private:
  Value *value_brightness_;

  int brightness_;

  bool required = true;

  int RequireChecker();
  void SetupBroker(char *argv[]);
};