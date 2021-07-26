#include <thing.h>

#define WATER_LEVEL_SENSOR_NUM 2
#define PUMP_PIN_NUM 4

const int kPumpPin[PUMP_PIN_NUM] = {2, 5, 6, 7};
const int kSoilMoisturePin = A0;
const int kWaterLevelPin[WATER_LEVEL_SENSOR_NUM] = {3, 4};

int pump_status_ = 0;
int water_level_[2];
double water_percentage_ = 0.0;
double unit = 100.0 / WATER_LEVEL_SENSOR_NUM;

int SensePumpStatus() { return pump_status_; }

int SenseSoilMoisture() {
  return (double)analogRead(kSoilMoisturePin) / 1024.0 * 100;
}

int SenseWaterLevel() {
  double sum = 0;
  for (int i = 0; i < WATER_LEVEL_SENSOR_NUM; i++) {
    water_level_[i] = digitalRead(kWaterLevelPin[i]);
    sum += (water_level_[i] == 0) ? unit : 0.0;
  }

  return (int)sum;
}

void ActuatePumpOnOff(void *pData) {
  SOPLOGLN("PUMP ON : ");
  for (int i = 0; i < PUMP_PIN_NUM; i++) {
    digitalWrite(kPumpPin[i], HIGH);
  }

  delay(1000);
  SOPLOGLN("PUMP OFF");

  for (int i = 0; i < PUMP_PIN_NUM; i++) {
    digitalWrite(kPumpPin[i], LOW);
  }
  pump_status_ = 0;
}

Thing thing((const char *)"SPBP1", 60, SafeSerial);

Value pump_status((const char *)"pump_status", SensePumpStatus, 0, 3, 3000);
Value water_level((const char *)"water_level", SenseWaterLevel, 0, 100, 30000);
Value soil_moisture_level((const char *)"soil_moisture_level",
                          SenseSoilMoisture, 0, 1024, 30000);

Function pump_on_off((const char *)"pump", ActuatePumpOnOff, 1);
Argument argTime((const char *)"time", 0, 100, INTEGER);
Tag tag_SmartPot("SmartPot");
Tag tag_SmartPotBP("SmartPotBP");

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  for (int i = 0; i < PUMP_PIN_NUM; i++) {
    pinMode(kPumpPin[i], OUTPUT);
    digitalWrite(kPumpPin[i], LOW);
  }
}

void SetupThing() {
  // Setup Functions
  pump_on_off.AddArgument(argTime);
  pump_on_off.AddTag(tag_SmartPot);
  pump_on_off.AddTag(tag_SmartPotBP);
  thing.Add(pump_on_off);

  // Setup Values
  pump_status.AddTag(tag_SmartPot);
  pump_status.AddTag(tag_SmartPotBP);
  water_level.AddTag(tag_SmartPot);
  water_level.AddTag(tag_SmartPotBP);
  soil_moisture_level.AddTag(tag_SmartPot);
  soil_moisture_level.AddTag(tag_SmartPotBP);
  thing.Add(pump_status);
  thing.Add(water_level);
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
