#include "temperaturesensor.h"
#include "thing.h"

using namespace sopiot;
using namespace std;

TemperatureSensor::TemperatureSensor(const std::string &broker_ip,
                                     int broker_port,
                                     const std::string &class_name,
                                     int alive_cycle)
    : Thing(broker_ip, broker_port, class_name, alive_cycle) {
  value_temp_ = NULL;

  temp_ = 0.0;
}

TemperatureSensor::~TemperatureSensor() { delete value_temp_; }

// Value adder
void TemperatureSensor::addSenseTemp(DoubleValue pfunc) {
  value_temp_ = new Value("temp", pfunc, 0, 1000, 1000);
}

//------------------------------------
// Value setter
//------------------------------------
void TemperatureSensor::setTemp(double temp) { temp_ = temp; }

//------------------------------------
// Value Getter
//------------------------------------
double TemperatureSensor::getTemp() { return temp_; }

//------------------------------------
// Function callback functions
//------------------------------------

//------------------------------------
// etc functions
//------------------------------------

void TemperatureSensor::SetupThing() {
  RequireChecker();

  // Setup Values
  Add(*value_temp_);

  // Setup Functions

  // Setup Thing
  Thing::Setup();
}

void TemperatureSensor::SetupBroker(char *argv[]) {
  std::string broker_ip = std::string(argv[1]);
  int broker_port = atoi(argv[2]);

  Thing::SetBroker(broker_ip, broker_port);
}

int TemperatureSensor::RequireChecker() {
  SOPLOG("RequireChecker Start\n");

  if (value_temp_ == NULL) {
    SOPLOG("Please fill out the callback function \"SenseTemp\".\n");
    required = false;
    exit(0);
  }

  if (required) {
    static Attribute attribute("TemperatureSensor", "TemperatureSensor",
                               STRING);
    AddPredefine(attribute);
  } else {
    SOPLOG("Please fill out all the callback functions\n");
    exit(0);
  }
}

void TemperatureSensor::Loop(int argc, char *argv[]) {
  if (argc == 3) SetupBroker(argv);

  while (true) {
    Publish();
    CAPTime_Sleep(100);
  }

  Disconnect();
}