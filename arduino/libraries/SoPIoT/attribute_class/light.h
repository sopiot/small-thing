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

class Light : public Thing {
public:
  Light(const std::string &broker_ip, int broker_port,
        const std::string &class_name, int alive_cycle);
  ~Light();

  //------------------------------------
  // Value adder
  //------------------------------------
  void addSenseOn(IntegerValue pfunc);
  void addSenseBrightness(IntegerValue pfunc);
  void addSenseColor(IntegerValue pfunc);

  //------------------------------------
  // Value setter
  //------------------------------------
  void setOn(int on);
  void setBrightness(int brightness);
  void setColor(int color);

  //------------------------------------
  // Value getter
  //------------------------------------
  int getOn();
  int getBrightness();
  int getColor();

  //------------------------------------
  // Function adder
  //------------------------------------
  void addOnOff(VoidFunction pfunc);
  void addSetBrightness(VoidPointerFunction pfunc);
  void addSetColor(VoidPointerFunction pfunc);

  //------------------------------------
  // etc functions
  //------------------------------------

  //------------------------------------
  // Setup functions
  //------------------------------------
  void Loop(int argc, char *argv[]);
  void SetupThing();

private:
  Value *value_on_;
  Value *value_brightness_;
  Value *value_color_;

  Function *on_off_;
  Function *set_brightness_;
  Function *set_color_;

  int on_;
  int brightness_;
  int color_;

  bool required = true;

  int RequireChecker();
  void SetupBroker(char *argv[]);
};