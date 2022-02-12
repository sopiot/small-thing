// #include "ota.h"
#include "thing.h"

#define DEVICE_NAME "Move1"
#define WINDOW_NUM 30

const int kmovement1Pin = 2;
int window[WINDOW_NUM] = {0};
int idx = 0;

int SenseMovementStatus() {
  int sum = 0;
  window[idx] = (int)digitalRead(kmovement1Pin);
  idx++;

  if (idx == WINDOW_NUM) idx = 0;

  for (int i = 0; i < WINDOW_NUM; i++) {
    sum += window[i];
  }

  if (sum > 0)
    return 1;
  else
    return 0;
}

Thing movement_thing((const char *)DEVICE_NAME, 60, SafeSerial);
Value movement_value((const char *)"movement_value", SenseMovementStatus, 0, 2,
                     1000);
Tag movement_tag("movement");

void SetupSerial() { SafeSerial.begin(115200); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kmovement1Pin, INPUT);

  // Attach modules
}

void SetupThing() {
  // Setup Functions

  // Setup Values
  movement_value.AddTag(movement_tag);
  movement_thing.Add(movement_value);

  // Setup Thing
  movement_thing.Setup();
}

void setup() {
  SetupSerial();
  SetupModules();
  // WiFi_Setup("SoPIoT_2.4G", "/PeaCE/#1", DEVICE_NAME, "0000");
  SetupThing();
}

void loop() {
  // SOPOTA();
  movement_thing.Loop();
}
