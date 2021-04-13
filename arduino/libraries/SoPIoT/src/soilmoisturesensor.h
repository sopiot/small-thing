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

class SoilMoistureSensor : public Thing {
 public:
  SoilMoistureSensor(const std::string& broker_ip, int broker_port,
                     const std::string& class_name, int alive_cycle);
  ~SoilMoistureSensor();

  //------------------------------------
  // Value adder
  //------------------------------------
  void addSenseSoilMoisture(DoubleValue pfunc);

  //------------------------------------
  // Value setter
  //------------------------------------
  void setSoilMoisture(double soilmoisture);

  //------------------------------------
  // Value getter
  //------------------------------------
  double getSoilMoisture();

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
  Value* value_soilmoisture_;

  double soilmoisture_;

  bool required = true;

  int RequireChecker();
  void SetupBroker(char* argv[]);
};