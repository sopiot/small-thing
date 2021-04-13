#include "soilmoisturesensor.h"
#include "thing.h"

using namespace sopiot;
using namespace std;

SoilMoistureSensor::SoilMoistureSensor(const std::string& broker_ip,
                                       int broker_port,
                                       const std::string& class_name,
                                       int alive_cycle)
    : Thing(broker_ip, broker_port, class_name, alive_cycle) {
  value_soilmoisture_ = NULL;

  soilmoisture_ = 0.0;
}

SoilMoistureSensor::~SoilMoistureSensor() { delete value_soilmoisture_; }

// Value adder
void SoilMoistureSensor::addSenseSoilMoisture(DoubleValue pfunc) {
  value_soilmoisture_ = new Value("soilmoisture", pfunc, 0, 1000, 1000);
}

//------------------------------------
// Value setter
//------------------------------------
void SoilMoistureSensor::setSoilMoisture(double soilmoisture) {
  soilmoisture_ = soilmoisture;
}

//------------------------------------
// Value Getter
//------------------------------------
double SoilMoistureSensor::getSoilMoisture() { return soilmoisture_; }

//------------------------------------
// Function callback functions
//------------------------------------

//------------------------------------
// etc functions
//------------------------------------

void SoilMoistureSensor::SetupThing() {
  RequireChecker();

  // Setup Values
  Add(*value_soilmoisture_);

  // Setup Functions

  // Setup Thing
  Thing::Setup();
}

void SoilMoistureSensor::SetupBroker(char* argv[]) {
  std::string broker_ip = std::string(argv[1]);
  int broker_port = atoi(argv[2]);

  Thing::SetBroker(broker_ip, broker_port);
}

int SoilMoistureSensor::RequireChecker() {
  SOPLOG("RequireChecker Start\n");

  if (value_soilmoisture_ == NULL) {
    SOPLOG("Please fill out the callback function \"SenseTemp\".\n");
    required = false;
    exit(0);
  }

  if (required) {
    static Attribute attribute("SoilmoistureSensor", "SoilmoistureSensor",
                               STRING);
    AddPredefine(attribute);
  } else {
    SOPLOG("Please fill out all the callback functions\n");
    exit(0);
  }
}

void SoilMoistureSensor::Loop(int argc, char* argv[]) {
  if (argc == 3) SetupBroker(argv);

  while (true) {
    Publish();
    CAPTime_Sleep(100);
  }

  Disconnect();
}