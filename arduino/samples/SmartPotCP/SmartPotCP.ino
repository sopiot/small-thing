// #include "ota.h"
#include "thing.h"

#define WATER_LEVEL_SENSOR_NUM 2
#define PUMP_PIN_NUM 4
#define DEVICE_NAME "SPCP2"

const int kPumpPin[PUMP_PIN_NUM] = {2, 5, 6, 7};
const int kWaterLevelPin[WATER_LEVEL_SENSOR_NUM] = {3, 4};

Thing thing((const char *)DEVICE_NAME, 60, SafeSerial);

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
Value pump_status((const char *)"pump_status", SensePumpStatus, 0, 3, 3000);
Value water_level((const char *)"water", SenseWaterLevel, 0, 100, 30000);

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

void ActuatePumpOnoff(void *pData) {
  int pump_time = 0;
  GetIntArgumentByName(pData, "time", &pump_time);

  for (int i = 0; i < PUMP_PIN_NUM; i++) digitalWrite(kPumpPin[i], HIGH);
  // for (int i = 0; i < PUMP_PIN_NUM; i++) digitalWrite(kPumpPin[i], LOW);

  delay(pump_time * 1000);

  for (int i = 0; i < PUMP_PIN_NUM; i++) digitalWrite(kPumpPin[i], LOW);
  // for (int i = 0; i < PUMP_PIN_NUM; i++) digitalWrite(kPumpPin[i], HIGH);

  pump_status_ = 0;
}

void ActuatePumpOn() {
  for (int i = 0; i < PUMP_PIN_NUM; i++) digitalWrite(kPumpPin[i], HIGH);
  // for (int i = 0; i < PUMP_PIN_NUM; i++) digitalWrite(kPumpPin[i], LOW);

  pump_status_ = 1;
}

void ActuatePumpoff() {
  for (int i = 0; i < PUMP_PIN_NUM; i++) digitalWrite(kPumpPin[i], LOW);
  // for (int i = 0; i < PUMP_PIN_NUM; i++) digitalWrite(kPumpPin[i], HIGH);

  pump_status_ = 0;
}

// Function declarations
// Function(name, actuate_function, arguments_num, function_tags_num);
Function pump_onoff((const char *)"pump_onoff", ActuatePumpOnoff, 1);
Function pump_on((const char *)"pump_on", ActuatePumpOn);
Function pump_off((const char *)"pump_off", ActuatePumpoff);
Argument argTime((const char *)"time", 0, 100, INTEGER);

//----------------------------------------
// Setup
//----------------------------------------

void SetupSerial() { SafeSerial.begin(115200); }

void SetupModules() {
  // Setup Pin mode
  for (int i = 0; i < PUMP_PIN_NUM; i++) {
    pinMode(kPumpPin[i], OUTPUT);
    // at lab CP LOW is pump on and high is off
    digitalWrite(kPumpPin[i], LOW);
    // digitalWrite(kPumpPin[i], HIGH);
  }

  for (int i = 0; i < WATER_LEVEL_SENSOR_NUM; i++) {
    pinMode(kWaterLevelPin[i], INPUT);
  }
  // Attach modules
}

void SetupThing() {
  // Setup Functions
  pump_onoff.AddTag(tag_SmartPot);
  pump_onoff.AddTag(tag_SmartPotCP);
  pump_onoff.AddArgument(argTime);
  thing.Add(pump_onoff);  // pin 2
  thing.Add(pump_on);     // pin 2
  thing.Add(pump_off);    // pin 2

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
  // WiFi_Setup("SoPIoT_2.4G", "/PeaCE/#1", DEVICE_NAME, "0000");
  SetupThing();
}

void loop() {
  // SOPOTA();
  thing.Loop();
}
