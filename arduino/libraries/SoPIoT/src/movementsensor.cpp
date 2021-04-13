#include "movementsensor.h"
#include "thing.h"

using namespace sopiot;
using namespace std;

MovementSensor::MovementSensor(const std::string& broker_ip, int broker_port,
                               const std::string& class_name, int alive_cycle)
    : Thing(broker_ip, broker_port, class_name, alive_cycle) {
  value_movement_ = NULL;
  movement_ = 0;
}

MovementSensor::~MovementSensor() { delete value_movement_; }

// Value adder
void MovementSensor::addSenseMovement(IntegerValue pfunc) {
  value_movement_ = new Value("movement", pfunc, 0, 1000, 1000);
}

//------------------------------------
// Value setter
//------------------------------------
void MovementSensor::setMovement(int movement) { movement_ = movement; }

//------------------------------------
// Value Getter
//------------------------------------
int MovementSensor::getMovement() { return movement_; }

//------------------------------------
// Function callback functions
//------------------------------------

//------------------------------------
// etc functions
//------------------------------------

void MovementSensor::SetupThing() {
  RequireChecker();

  // Setup Values
  Add(*value_movement_);

  // Setup Functions

  // Setup Thing
  Thing::Setup();
}

void MovementSensor::SetupBroker(char* argv[]) {
  std::string broker_ip = std::string(argv[1]);
  int broker_port = atoi(argv[2]);

  Thing::SetBroker(broker_ip, broker_port);
}

int MovementSensor::RequireChecker() {
  SOPLOG("RequireChecker Start\n");

  if (value_movement_ == NULL) {
    SOPLOG("Please fill out the callback function \"tmp_value\".\n");
    required = false;
    exit(0);
  }

  if (required) {
    static Attribute attribute("MovementSensor", "MovementSensor", STRING);
    AddPredefine(attribute);
  } else {
    SOPLOG("Please fill out all the callback functions\n");
    exit(0);
  }
}

void MovementSensor::Loop(int argc, char* argv[]) {
  if (argc == 3) SetupBroker(argv);

  while (true) {
    Publish();
    CAPTime_Sleep(100);
  }

  Disconnect();
}