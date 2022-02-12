// #include "ota.h"
#include "thing.h"

#define SENSOR_WINDOW 10

#define WATER_LEVEL_SENSOR_NUM 2
#define DEVICE_NAME "SPCV10"

const int kValvePin = 2;
const int kSoilMoisturePin = A0;

int valve_status_ = 0;
int soil_moisture_level_ = 0;

int SenseValveStatus() { return valve_status_; }

int SenseSoilMoistureLevel() {
  double sum = 0.0;

  for (int i = 0; i < SENSOR_WINDOW; i++) {
    sum += (1024.0 - analogRead(kSoilMoisturePin)) / 1024.0 * 1000;
    delay(1);
  }
  return (int)(sum / SENSOR_WINDOW);
}

void ActuateValveOpenClose(void *pData) {
  int valve_time = 0;
  GetIntArgumentByName(pData, "time", &valve_time);

  digitalWrite(kValvePin, HIGH);
  delay(valve_time * 1000);
  digitalWrite(kValvePin, LOW);
  valve_status_ = 0;
}

void ActuateValveOpen() {
  digitalWrite(kValvePin, HIGH);
  valve_status_ = 1;
}

void ActuateValveClose() {
  digitalWrite(kValvePin, LOW);
  valve_status_ = 0;
}

Thing thing((const char *)DEVICE_NAME, 60, SafeSerial);

Value valve_status((const char *)"valve_status", SenseValveStatus, 0, 3, 3000);
Value soil_moisture_level((const char *)"soil_moisture", SenseSoilMoistureLevel,
                          0, 1000, 30000);
Function valve_open_close((const char *)"on_while", ActuateValveOpenClose, 1);
Argument argTime((const char *)"time", 0, 100, INTEGER);
Function valve_open((const char *)"on", ActuateValveOpen);
Function valve_close((const char *)"off", ActuateValveClose);
Tag tag_SmartPot("SmartPot");
Tag tag_SmartPotCV("SmartPotCV");

void SetupSerial() { SafeSerial.begin(115200); }

void SetupModules() { pinMode(kValvePin, OUTPUT); }

void SetupThing() {
  // Setup Functions
  // thing.Add(valve_open);
  // thing.Add(valve_close);
  // valve_open_close.AddArgument(argTime);
  valve_open_close.AddTag(tag_SmartPot);
  valve_open_close.AddTag(tag_SmartPotCV);
  valve_open_close.AddArgument(argTime);
  valve_open.AddTag(tag_SmartPot);
  valve_open.AddTag(tag_SmartPotCV);
  valve_close.AddTag(tag_SmartPot);
  valve_close.AddTag(tag_SmartPotCV);
  thing.Add(valve_open_close);
  thing.Add(valve_open);
  thing.Add(valve_close);

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
  // WiFi_Setup("SoPIoT_2.4G", "/PeaCE/#1", DEVICE_NAME, "0000");
  SetupThing();
}

void loop() {
  // SOPOTA();
  thing.Loop();
}
