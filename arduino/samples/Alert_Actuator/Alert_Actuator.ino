/**************************************
 * SoPIoT Alert Actuator Thing Sample
 *************************************/

#include "thing.h"

static const int kLEDPin = 13;

Thing alert_actuator((const char *)"Alert", 60, SafeSerial);

bool Alertfunction(void *args) {
  bool bool_arg;
  GetBoolArgumentByName(args, "args_test", &bool_arg);
  SOPLOGLN("bool_arg: %d", bool_arg);

  if (bool_arg) {
    digitalWrite(kLEDPin, HIGH);
    SOPLOGLN("Alertfunction ON");
  } else {
    digitalWrite(kLEDPin, LOW);
    SOPLOGLN("Alertfunction OFF");
  }

  return bool_arg;
}

Function alert_func("dummy_function", Alertfunction, 1);
Argument arg("args_test", 0, 2, BOOL);

void SetupSerial() { SafeSerial.begin(115200); }

void SetupModules() {
  pinMode(kLEDPin, OUTPUT);
  digitalWrite(kLEDPin, LOW);
}

void SetupThing() {
  delay(500);

  alert_func.AddTag("alert");

  alert_actuator.Add(alert_func);

  alert_actuator.Setup();
}

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { alert_actuator.Loop(); }
