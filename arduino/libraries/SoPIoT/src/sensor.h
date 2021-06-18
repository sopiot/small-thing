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

class Sensor : public Thing {
 public:
  Sensor(const std::string& broker_ip, int broker_port,
         const std::string& class_name, int alive_cycle);
  ~Sensor();

  //------------------------------------
  // Value adder
  //------------------------------------
  void addSenseSensorStatus(IntegerValue pfunc);

  //------------------------------------
  // Value setter
  //------------------------------------
  void setSensorStatus(int sensor_status);

  //------------------------------------
  // Value getter
  //------------------------------------
  int getSensorStatus();

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
  Value* value_sensor_status_;

  int sensor_status_;

  bool required = true;

  int RequireChecker();
  void SetupBroker(char* argv[]);
};