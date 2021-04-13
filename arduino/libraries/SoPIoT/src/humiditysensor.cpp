#include "humiditysensor.h"
#include "templete.h"
#include "thing.h"

using namespace sopiot;
using namespace std;

HumiditySensor::HumiditySensor(const std::string& broker_ip, int broker_port,
                               const std::string& class_name, int alive_cycle)
    : Thing(broker_ip, broker_port, class_name, alive_cycle) {
  value_humidity_ = NULL;

  humidity_ = 0.0;
}

HumiditySensor::~HumiditySensor() { delete value_humidity_; }

// Value adder
void HumiditySensor::addSenseHumidity(DoubleValue pfunc) {
  value_humidity_ = new Value("humidity", pfunc, 0, 1000, 1000);
}

//------------------------------------
// Value setter
//------------------------------------
void HumiditySensor::setHumidity(double humidity) { humidity_ = humidity; }

//------------------------------------
// Value Getter
//------------------------------------
double HumiditySensor::getHumidity() { return humidity_; }

//------------------------------------
// Function callback functions
//------------------------------------

//------------------------------------
// etc functions
//------------------------------------

void HumiditySensor::SetupThing() {
  RequireChecker();

  // Setup Values
  Add(*value_humidity_);

  // Setup Functions

  // Setup Thing
  Thing::Setup();
}

void HumiditySensor::SetupBroker(char* argv[]) {
  std::string broker_ip = std::string(argv[1]);
  int broker_port = atoi(argv[2]);

  Thing::SetBroker(broker_ip, broker_port);
}

int HumiditySensor::RequireChecker() {
  SOPLOG("RequireChecker Start\n");

  if (value_humidity_ == NULL) {
    SOPLOG("Please fill out the callback function \"SenseHumidity\".\n");
    required = false;
    exit(0);
  }

  if (required) {
    static Attribute attribute("HumiditySensor", "HumiditySensor", STRING);
    AddPredefine(attribute);
  } else {
    SOPLOG("Please fill out all the callback functions\n");
    exit(0);
  }
}

void HumiditySensor::Loop(int argc, char* argv[]) {
  if (argc == 3) SetupBroker(argv);

  while (true) {
    Publish();
    CAPTime_Sleep(100);
  }

  Disconnect();
}