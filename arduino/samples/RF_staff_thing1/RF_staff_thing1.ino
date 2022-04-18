#include "rf_staff_thing.h"

RFStaffThing rf_staff_thing(7, 8, 0xFFFFFFFFF1LL, 0xFFFFFFFFF0LL);

void setup() {
  rf_staff_thing.SetupRFModule();
  rf_staff_thing.SetupSensor();
}

void loop() { rf_staff_thing.Loop(); }
