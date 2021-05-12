#include "soundsensor.h"

SoundSensor::SoundSensor(const char* class_name, int alive_cycle,
                         Stream& serial)
    : Thing(class_name, alive_cycle, serial) {
  value_loudness_ = NULL;
}

SoundSensor::~SoundSensor() { delete value_loudness_; }

//------------------------------------
// Value adder
//------------------------------------
void SoundSensor::addSenseLoudness(IntegerValue pfunc) {
  value_loudness_ = new Value("Value_SoundSensor", pfunc, 0, 2500, 1000);
}

//------------------------------------
// Function adder
//------------------------------------

int SoundSensor::SetupThing() {
  if (RequireChecker() != 0) return -1;

  // Setup Values
  Add(*value_loudness_);

  // Setup Functions

  return 0;
}

// int SoundSensor::RequireChecker() {
//   required = true;
//   if (value_loudness_ == NULL) {
//     SOPLOG("Please fill out the callback function \"SenseValueStatus\".\n");
//     required = false;
//     return -1;
//   }

//   if (required) {
//     static Attribute attribute("SoundSensor", "SoundSensor", STRING);
//     AddPredefine(attribute);
//     return 0;
//   } else {
//     SOPLOG("Please fill out all the callback functions\n");
//     return -1;
//   }
// }