#include "dustsensor.h"
#include "thing.h"

using namespace sopiot;
using namespace std;

DustSensor::DustSensor(const std::string& broker_ip, int broker_port,
                       const std::string& class_name, int alive_cycle)
    : Thing(broker_ip, broker_port, class_name, alive_cycle) {
  value_dust_ = NULL;
  dust_ = 0.0;
}

DustSensor::~DustSensor() { delete value_dust_; }

// Value adder
void DustSensor::addSenseDust(DoubleValue pfunc) {
  value_dust_ = new Value("dust", pfunc, 0, 1000, 1000);
}

//------------------------------------
// Value setter
//------------------------------------
void DustSensor::setDust(double dust) { dust_ = dust; }

//------------------------------------
// Value Getter
//------------------------------------
double DustSensor::getDust() { return dust_; }

//------------------------------------
// Function callback functions
//------------------------------------

//------------------------------------
// etc functions
//------------------------------------

void DustSensor::SetupThing() {
  RequireChecker();

  // Setup Values
  Add(*value_dust_);

  // Setup Functions

  // Setup Thing
  Thing::Setup();
}

void DustSensor::SetupBroker(char* argv[]) {
  std::string broker_ip = std::string(argv[1]);
  int broker_port = atoi(argv[2]);

  Thing::SetBroker(broker_ip, broker_port);
}

int DustSensor::RequireChecker() {
  SOPLOG("RequireChecker Start\n");

  if (value_dust_ == NULL) {
    SOPLOG("Please fill out the callback function \"SenseDust\".\n");
    required = false;
    exit(0);
  }

  if (required) {
    static Attribute attribute("DustSensor", "DustSensor", STRING);
    AddPredefine(attribute);
  } else {
    SOPLOG("Please fill out all the callback functions\n");
    exit(0);
  }
}

void DustSensor::Loop(int argc, char* argv[]) {
  if (argc == 3) SetupBroker(argv);

  while (true) {
    Publish();
    CAPTime_Sleep(100);
  }

  Disconnect();
}