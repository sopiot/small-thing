#include "rf_staff_thing.h"

// struct PayloadStruct {
//   char message[16];
//   uint64_t addr;
// };
// PayloadStruct payload;

RFStaffThing rf_staff_thing(7, 8, 0xFFFFFFFFF0LL, 0xFFFFFFFFF1LL);

void setup() {
  rf_staff_thing.SetupRFModule();
  rf_staff_thing.SetupSensor();
}

void loop() { rf_staff_thing.Run(); }