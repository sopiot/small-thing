#include "distancesensor.h"

#include "../thing.h"

using namespace sopiot;
using namespace std;

DistanceSensor::DistanceSensor(const std::string& broker_ip, int broker_port,
                               const std::string& class_name, int alive_cycle)
    : Thing(broker_ip, broker_port, class_name, alive_cycle) {
  value_distance_ = NULL;
  distance_ = 0.0;
}

DistanceSensor::~DistanceSensor() { delete value_distance_; }

// Value adder
void DistanceSensor::addSenseDistance(DoubleValue pfunc) {
  value_distance_ = new Value("distance", pfunc, 0, 1000, 1000);
}

//------------------------------------
// Value setter
//------------------------------------
void DistanceSensor::setDistance(double distance) { distance_ = distance; }

//------------------------------------
// Value Getter
//------------------------------------
double DistanceSensor::getDistance() { return distance_; }

//------------------------------------
// Function callback functions
//------------------------------------

//------------------------------------
// etc functions
//------------------------------------

void DistanceSensor::SetupThing() {
  RequireChecker();

  // Setup Values
  Add(*value_distance_);

  // Setup Functions

  // Setup Thing
  Thing::Setup();
}

void DistanceSensor::SetupBroker(char* argv[]) {
  std::string broker_ip = std::string(argv[1]);
  int broker_port = atoi(argv[2]);

  Thing::SetBroker(broker_ip, broker_port);
}

int DistanceSensor::RequireChecker() {
  SOPLOG("RequireChecker Start\n");

  if (value_distance_ == NULL) {
    SOPLOG("Please fill out the callback function \"SenseDistance\".\n");
    required = false;
    exit(0);
  }

  if (required) {
    static Attribute attribute("DistanceSensor", "DistanceSensor", STRING);
    AddPredefine(attribute);
  } else {
    SOPLOG("Please fill out all the callback functions\n");
    exit(0);
  }
}

void DistanceSensor::Loop(int argc, char* argv[]) {
  if (argc == 3) SetupBroker(argv);

  while (true) {
    Publish();
    CAPTime_Sleep(100);
  }

  Disconnect();
}