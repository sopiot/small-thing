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

class DistanceSensor : public Thing {
public:
  DistanceSensor(const std::string &broker_ip, int broker_port,
                 const std::string &class_name, int alive_cycle);
  ~DistanceSensor();

  //------------------------------------
  // Value adder
  //------------------------------------
  void addSenseDistance(DoubleValue pfunc);

  //------------------------------------
  // Value setter
  //------------------------------------
  void setDistance(double distance);

  //------------------------------------
  // Value getter
  //------------------------------------
  double getDistance();

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
  Value *value_distance_;

  double distance_;

  bool required = true;

  int RequireChecker();
  void SetupBroker(char *argv[]);
};