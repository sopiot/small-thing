#include "light.h"
#include "thing.h"

using namespace sopiot;
using namespace std;

Light::Light(const std::string &broker_ip, int broker_port,
             const std::string &class_name, int alive_cycle)
    : Thing(broker_ip, broker_port, class_name, alive_cycle) {
  value_on_ = NULL;
  value_brightness_ = NULL;
  value_color_ = NULL;

  on_off_ = NULL;
  set_brightness_ = NULL;
  set_color_ = NULL;

  on_ = 0;
  brightness_ = 0;
  color_ = 0;
}

Light::~Light() {
  delete value_on_;
  delete value_brightness_;
  delete value_color_;

  delete on_off_;
  delete set_brightness_;
  delete set_color_;
}

// Value adder
void Light::addSenseOn(IntegerValue pfunc) {
  value_on_ = new Value("on", pfunc, 0, 1, 1000);
}

void Light::addSenseBrightness(IntegerValue pfunc) {
  value_brightness_ = new Value("brightness", pfunc, 0, 1000, 1000);
}

void Light::addSenseColor(IntegerValue pfunc) {
  value_color_ = new Value("color", pfunc, 0, 0xFFFFFF, 1000);
}

//------------------------------------
// Value setter
//------------------------------------
void Light::setOn(int on) { on_ = on; }
void Light::setBrightness(int brightness) { brightness_ = brightness; }
void Light::setColor(int color) { color_ = color; }

//------------------------------------
// Value Getter
//------------------------------------
int Light::getOn() { return on_; }
int Light::getBrightness() { return brightness_; }
int Light::getColor() { return color_; }

//------------------------------------
// Function callback functions
//------------------------------------
void Light::addOnOff(VoidFunction pfunc) {
  on_off_ = new Function("on_off", pfunc);
}

void Light::addSetBrightness(VoidPointerFunction pfunc) {
  set_brightness_ = new Function("set_brightness", pfunc);
  static Argument brightness("arg_brightness", 0, 1000, INTEGER);
  set_brightness_->Add_argument(brightness);
}

void Light::addSetColor(VoidPointerFunction pfunc) {
  set_color_ = new Function("set_color", pfunc);
  static Argument color("arg_color", 0, 0xFFFFF, INTEGER);
  set_color_->Add_argument(color);
}

//------------------------------------
// etc functions
//------------------------------------

void Light::SetupThing() {
  RequireChecker();

  // Setup Values
  Add(*value_on_);
  Add(*value_brightness_);
  Add(*value_color_);

  // Setup Functions
  Add(*on_off_);
  Add(*set_brightness_);
  Add(*set_color_);

  // Setup Thing
  Thing::Setup();
}

void Light::SetupBroker(char *argv[]) {
  std::string broker_ip = std::string(argv[1]);
  int broker_port = atoi(argv[2]);

  Thing::SetBroker(broker_ip, broker_port);
}

int Light::RequireChecker() {
  SOPLOG("RequireChecker Start\n");

  // Value check
  if (value_on_ == NULL) {
    SOPLOG("Please fill out the callback function \"SenseOn\".\n");
    required = false;
    exit(0);
  }

  if (value_brightness_ == NULL) {
    SOPLOG("Please fill out the callback function \"SenseBrightness\".\n");
    required = false;
    exit(0);
  }

  if (value_color_ == NULL) {
    SOPLOG("Please fill out the callback function \"SenseColor\".\n");
    required = false;
    exit(0);
  }

  // Function check
  if (on_off_ == NULL) {
    SOPLOG("Please fill out the callback function \"OnOff\".\n");
    required = false;
    exit(0);
  }

  if (set_brightness_ == NULL) {
    SOPLOG("Please fill out the callback function \"SetBrightness\".\n");
    required = false;
    exit(0);
  }

  if (set_color_ == NULL) {
    SOPLOG("Please fill out the callback function \"SetColor\".\n");
    required = false;
    exit(0);
  }

  if (required) {
    static Attribute attribute("Light", "Light", STRING);
    AddPredefine(attribute);
  } else {
    SOPLOG("Please fill out all the callback functions\n");
    exit(0);
  }
}

void Light::Loop(int argc, char *argv[]) {
  if (argc == 3)
    SetupBroker(argv);

  while (true) {
    Publish();
    CAPTime_Sleep(100);
  }

  Disconnect();
}