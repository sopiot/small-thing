#include <thing.h>

#define WATER_LEVEL_SENSOR_NUM 2

const int kPumpPin = 2;
const int kSoilMoisturePin = A1;
const int kWaterLevelPin[WATER_LEVEL_SENSOR_NUM] = {3, 4};

Thing thing((const char *)"SmartPotBP", 60, SafeSerial);

Tag tag_SmartPot("SmartPot");
Tag tag_SmartPotBP("SmartPotBP");
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

// Value declarations
// Value(name, sense_function, nValueTags, min, max, period(ms));
Value pump_status((const char *)"pump_status", SensePumpStatus, 5, 0, 3, 3000);
Value water_level((const char *)"water_level", SenseWaterLevel, 5, 0, 100, 3000);
Value soil_moisture_level((const char *)"soil_moisture_level",
                          SenseSoilMoisture, 5, 0, 1024, 3000);

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

void ActuatePumpOnOff(void *pData) {
  int *time = (int *)pData;
  SOPLOGLN("PUMP ON : ");
  SOPLOGLN(*time);
  digitalWrite(kPumpPin, HIGH);
  delay(300);
  SOPLOGLN("PUMP OFF");
  digitalWrite(kPumpPin, LOW);
  pump_status_ = 0;
}

Argument argTime((const char *)"time", 0, 100, INTEGER);

// Function declarations
// Function(name, actuate_function, arguments_num, function_tags_num);
Function pump_on_off((const char *)"pump", ActuatePumpOnOff, 1, 5);

//----------------------------------------
// Setup
//----------------------------------------

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kPumpPin, OUTPUT);

  // Attach modules
}

void SetupThing() {
  // Setup Functions
  pump_on_off.AddArgument(argTime);
  // pump_on_off.AddTag(tag_SmartPot);
  // pump_on_off.AddTag(tag_SmartPotBP);
  thing.Add(pump_on_off);

  // Setup Values
  // pump_status.AddTag(tag_SmartPot);
  // pump_status.AddTag(tag_SmartPotBP);
  // water_level.AddTag(tag_SmartPot);
  // water_level.AddTag(tag_SmartPotBP);
  // soil_moisture_level.AddTag(tag_SmartPot);
  // soil_moisture_level.AddTag(tag_SmartPotBP);
  thing.Add(pump_status);
  thing.Add(water_level);
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
