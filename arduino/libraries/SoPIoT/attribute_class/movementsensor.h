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

class MovementSensor : public Thing {
public:
  MovementSensor(const std::string &broker_ip, int broker_port,
                 const std::string &class_name, int alive_cycle);
  ~MovementSensor();

  //------------------------------------
  // Value adder
  //------------------------------------
  void addSenseMovement(IntegerValue pfunc);

  //------------------------------------
  // Value setter
  //------------------------------------
  void setMovement(int movement);

  //------------------------------------
  // Value getter
  //------------------------------------
  int getMovement();

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
  Value *value_movement_;

  int movement_;

  bool required = true;

  int RequireChecker();
  void SetupBroker(char *argv[]);
};