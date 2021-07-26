#include <thing.h>

#define WATER_LEVEL_SENSOR_NUM 2

const int kValvePin = 2;
const int kSoilMoisturePin = A0;

int valve_status_ = 0;
int soil_moisture_level_ = 0;

int SenseValveStatus() { return valve_status_; }

int SenseSoilMoistureLevel() {
  return (double)analogRead(kSoilMoisturePin) / 1024.0 * 100;
}

void ActuateValveOpenClose() {
  digitalWrite(kValvePin, HIGH);
  delay(1000);
  digitalWrite(kValvePin, LOW);
  valve_status_ = 0;
}

Thing thing((const char *)"SPCV1", 10, SafeSerial);

Value valve_status((const char *)"valve_status", SenseValveStatus, 0, 3, 3000);
Value soil_moisture_level((const char *)"soil_moisture_level",
                          SenseSoilMoistureLevel, 0, 100, 30000);
Function valve_open_close((const char *)"valve_on", ActuateValveOpenClose);
Tag tag_SmartPot("SmartPot");
Tag tag_SmartPotCV("SmartPotCV");

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() { pinMode(kValvePin, OUTPUT); }

void SetupThing() {
  // Setup Functions
  // thing.Add(valve_open);
  // thing.Add(valve_close);
  // valve_open_close.AddArgument(argTime);
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

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { thing.Loop(); }