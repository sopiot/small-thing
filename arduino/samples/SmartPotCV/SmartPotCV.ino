#include <thing.h>

#define WATER_LEVEL_SENSOR_NUM 2

const int kValvePin = 2;
const int kSoilMoisturePin = A0;

Thing thing((const char *)"SmartPotCV", 60, SafeSerial);

Tag tag_SmartPot("SmartPot");
Tag tag_SmartPotCV("SmartPotCV");

int valve_status_ = 0;
int soil_moisture_level_ = 0;

int SenseValveStatus() { return valve_status_; }

int SenseSoilMoistureLevel() {
  return (double)analogRead(kSoilMoisturePin) / 1024.0 * 100;
}

// Value declarations
// Value(name, sense_function, nValueTags, min, max, period(ms));
Value valve_status((const char *)"valve_status", SenseValveStatus, 5, 0, 3, 3000);
Value soil_moisture_level((const char *)"soil_moisture_level",
                          SenseSoilMoistureLevel, 5, 0, 100, 3000);

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------


void ActuateValveOpenClose(void *pData) {
  int *time = (int *)pData;
  digitalWrite(kValvePin, HIGH);
  delay((*time) * 1000);
  digitalWrite(kValvePin, LOW);
  valve_status_ = 0;
}

// Function declarations
// Function(name, actuate_function, arguments_num, function_attributes_num);
// Function valve_open((const char *)"valve_open", ActuateValveOn, 0, 0);
// Function valve_close((const char *)"valve_close", ActuateValveOff, 0, 0);
Argument argTime((const char *)"time", 0, 100, INTEGER);

Function valve_open_close((const char *)"valve_open_close", ActuateValveOpenClose, 1, 5);

//----------------------------------------
// Setup
//----------------------------------------

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kValvePin, OUTPUT);

  // Attach modules
}

void SetupThing() {
  // Setup Functions
  // thing.Add(valve_open);
  // thing.Add(valve_close);
  valve_open_close.AddArgument(argTime);
  valve_open_close.AddTag(tag_SmartPot);
  valve_open_close.AddTag(tag_SmartPotCV);
  thing.Add(valve_open_close);

  // Setup Values
  valve_status.AddTag(tag_SmartPot);
  valve_status.AddTag(tag_SmartPotCV);
  soil_moisture_level.AddTag(tag_SmartPot);
  soil_moisture_level.AddTag(tag_SmartPotCV);
  thing.Add(valve_status);
  thing.Add(soil_moisture_level);

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