#include <thing.h>

#define WATER_LEVEL_SENSOR_NUM 2

const int kPumpPin = 2;
const int kWaterLevelPin[WATER_LEVEL_SENSOR_NUM] = {3, 4};

Thing thing((const char *)"SmartPotCP", 60, SafeSerial);

Tag tag_SmartPot("SmartPot");
Tag tag_SmartPotCP("SmartPotCP");

int pump_status_ = 0;

int water_level_[2];
double water_percentage_ = 0.0;
double unit = 100.0 / WATER_LEVEL_SENSOR_NUM;

int SensePumpStatus() { return pump_status_; }

int SenseWaterLevel() {
  double sum = 0;
  for (int i = 0; i < WATER_LEVEL_SENSOR_NUM; i++) {
    water_level_[i] = digitalRead(kWaterLevelPin[i]);
    sum += (water_level_[i] == 0) ? unit : 0.0;
  }

  return (int)sum;
}

// Value declarations
// Value(name, sense_function, min, max, period(ms));
Value pump_status((const char *)"pump_status", SensePumpStatus, 5, 0, 3, 3000);
Value water_level((const char *)"water_level", SenseWaterLevel, 5, 0, 100, 3000);

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

void ActuatePumpOn(void *pData) {
  digitalWrite(kPumpPin, HIGH);
  // delay(300);
  pump_status_ = 1;
}

void ActuatePumpOff(void *pData) {
  digitalWrite(kPumpPin, LOW);
  // delay(300);
  pump_status_ = 0;
}

// Function declarations
// Function(name, actuate_function, arguments_num, function_tags_num);
Function pump_on((const char *)"pump_on", ActuatePumpOn, 0, 5);
Function pump_off((const char *)"pump_off", ActuatePumpOff, 0, 5);

//----------------------------------------
// Setup
//----------------------------------------

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kPumpPin, OUTPUT);
  digitalWrite(kPumpPin, HIGH);
  for (int i = 0; i < WATER_LEVEL_SENSOR_NUM; i++) {
    pinMode(kWaterLevelPin[i], INPUT);
  }
  // Attach modules
}

void SetupThing() {
  // Setup Functions
  pump_on.AddTag(tag_SmartPot);
  pump_on.AddTag(tag_SmartPotCP);
  pump_off.AddTag(tag_SmartPot);
  pump_off.AddTag(tag_SmartPotCP);
  thing.Add(pump_on);  // pin 2
  thing.Add(pump_off);

  // Setup Values
  pump_status.AddTag(tag_SmartPot);
  pump_status.AddTag(tag_SmartPotCP);
  water_level.AddTag(tag_SmartPot);
  water_level.AddTag(tag_SmartPotCP);
  thing.Add(pump_status);
  thing.Add(water_level);  // pin 3, 4

  // Setup Thing
  thing.Setup();
}

//----------------------------------------
// Main
//----------------------------------------

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { thing.Loop(); }