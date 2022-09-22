/***********************************
 * SoPIoT Light Sensor Thing Sample
 **********************************/

#include "thing.h"

static const int kLightPin = A0;

Thing light_sensor((const char *)"Light", 60, SafeSerial);

int brightness_;

int SenseBrightness() { return analogRead(kLightPin); }

int Actuatefunction(void *args) {
  int int_arg;
  SOPLOGLN("in Actuatefunction");
  GetIntArgumentByName(args, "args_test", &int_arg);
  SOPLOGLN("int_arg: %d", int_arg);
  return int_arg;
}

Value brightness((const char *)"brightness", SenseBrightness, 0, 2048, 1000);

void SetupSerial() { SafeSerial.begin(115200); }

void SetupModules() {}

void SetupThing() {
  delay(500);

  brightness.AddTag("light");

  light_sensor.Add(brightness);

  light_sensor.Setup();
}

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { light_sensor.Loop(); }