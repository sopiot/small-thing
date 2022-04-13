// #include "ota.h"
#include "thing.h"

#define DEVICE_NAME "Distance1"

const int kTriggerPin = 3;
const int kReceivePin = 2;

int distance_status_;

int SenseDistanceStatus() {
  long Duration = 0;
  digitalWrite(kTriggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(kTriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(kTriggerPin, LOW);

  Duration = pulseIn(kReceivePin, HIGH);
  int Distance_mm = ((Duration / 2.9) / 2);

  return Distance_mm;
}

Thing thing((const char *)DEVICE_NAME, 60, SafeSerial);
Value distance_status((const char *)"distance", SenseDistanceStatus, 0, 10000,
                      1000);

void SetupSerial() { SafeSerial.begin(115200); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kTriggerPin, OUTPUT);
  pinMode(kReceivePin, INPUT);

  // Attach modules
}

void SetupThing() {
  // Setup Functions

  // Setup Values
  thing.Add(distance_status);

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
