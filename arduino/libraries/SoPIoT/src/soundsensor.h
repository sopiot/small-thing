// SoPIoT Thing Library
#include "../thing.h"
#include "../utils.h"

class SoundSensor : public Thing {
 public:
  SoundSensor(const char* class_name, int alive_cycle, Stream& serial);
  ~SoundSensor();

  //------------------------------------
  // Value adder
  //------------------------------------
  void addSenseLoudness(IntegerValue pfunc);

  //------------------------------------
  // Function adder
  //------------------------------------

  //------------------------------------
  // Setup
  //------------------------------------
  int SetupThing();

 protected:
  Value* value_loudness_;

  // int RequireChecker();

 private:
  bool required = true;
};