#include "lightsensor.h"
#include "thing.h"

using namespace sopiot;
using namespace std;

LightSensor::LightSensor(const std::string& broker_ip, int broker_port,
                         const std::string& class_name, int alive_cycle)
    : Thing(broker_ip, broker_port, class_name, alive_cycle) {
  value_brightness_ = NULL;
  brightness_ = 0;
}

LightSensor::~LightSensor() { delete value_brightness_; }

// Value adder
void LightSensor::addSenseBrightness(IntegerValue pfunc) {
  value_brightness_ = new Value("brightness", pfunc, 0, 1000, 1000);
}

//------------------------------------
// Value setter
//------------------------------------
void LightSensor::setBrightness(int brightness) { brightness_ = brightness; }

//------------------------------------
// Value Getter
//------------------------------------
int LightSensor::getBrightness() { return brightness_; }

//------------------------------------
// Function callback functions
//------------------------------------

//------------------------------------
// etc functions
//------------------------------------

void LightSensor::SetupThing() {
  RequireChecker();

  // Setup Values
  Add(*value_brightness_);

  // Setup Functions

  // Setup Thing
  Thing::Setup();
}

void LightSensor::SetupBroker(char* argv[]) {
  std::string broker_ip = std::string(argv[1]);
  int broker_port = atoi(argv[2]);

  Thing::SetBroker(broker_ip, broker_port);
}

int LightSensor::RequireChecker() {
  SOPLOG("RequireChecker Start\n");

  if (value_brightness_ == NULL) {
    SOPLOG("Please fill out the callback function \"SenseBrightness\".\n");
    required = false;
    exit(0);
  }

  if (required) {
    static Attribute attribute("LightSensor", "LightSensor", STRING);
    AddPredefine(attribute);
  } else {
    SOPLOG("Please fill out all the callback functions\n");
    exit(0);
  }
}

void LightSensor::Loop(int argc, char* argv[]) {
  if (argc == 3) SetupBroker(argv);

  while (true) {
    Publish();
    CAPTime_Sleep(100);
  }

  Disconnect();
}