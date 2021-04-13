#include "sensor.h"

#include "thing.h"

using namespace sopiot;
using namespace std;

Sensor::Sensor(const std::string& broker_ip, int broker_port,
               const std::string& class_name, int alive_cycle)
    : Thing(broker_ip, broker_port, class_name, alive_cycle) {
  value_sensor_status_ = NULL;
  sensor_status_ = 0;
}

Sensor::~Sensor() { delete value_sensor_status_; }

// Value adder
void Sensor::addSenseSensorStatus(IntegerValue pfunc) {
  value_sensor_status_ = new Value("sensor_status", pfunc, 0, 10, 1000);
}

//------------------------------------
// Value setter
//------------------------------------
void Sensor::setSensorStatus(int sensor_status) {
  sensor_status_ = sensor_status;
}

//------------------------------------
// Value Getter
//------------------------------------
int Sensor::getSensorStatus() { return sensor_status_; }

//------------------------------------
// Function callback functions
//------------------------------------

//------------------------------------
// etc functions
//------------------------------------

void Sensor::SetupThing() {
  RequireChecker();

  // Setup Values
  Add(*value_sensor_status_);

  // Setup Functions

  // Setup Thing
  Thing::Setup();
}

void Sensor::SetupBroker(char* argv[]) {
  std::string broker_ip = std::string(argv[1]);
  int broker_port = atoi(argv[2]);

  Thing::SetBroker(broker_ip, broker_port);
}

int Sensor::RequireChecker() {
  SOPLOG("RequireChecker Start\n");

  if (value_sensor_status_ == NULL) {
    SOPLOG("Please fill out the callback function \"SensoSensorStatus\".\n");
    required = false;
    exit(0);
  }

  if (required) {
    static Attribute attribute("Sensor", "Sensor", STRING);
    AddPredefine(attribute);
  } else {
    SOPLOG("Please fill out all the callback functions\n");
    exit(0);
  }
}

void Sensor::Loop(int argc, char* argv[]) {
  if (argc == 3) SetupBroker(argv);

  while (true) {
    Publish();
    CAPTime_Sleep(100);
  }

  Disconnect();
}