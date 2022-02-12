// #include "ota.h"
#include "thing.h"

#define DEVICE_NAME "Sound3"

const int kSoundPin = A0;
const int kLightPin = A1;
const int kSampleWindow = 100;  // Sample window width in mS (50 mS = 20Hz)
unsigned int sample_ = 0;
int loudness_ = 0;

int SenseLoudness() {
  unsigned long startMillis = millis();  // Start of sample window
  unsigned int peakToPeak = 0;           // peak-to-peak level
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  while (millis() - startMillis < kSampleWindow) {
    sample_ = analogRead(kSoundPin);
    if (sample_ < 1024)  // toss out spurious readings
    {
      if (sample_ > signalMax)
        signalMax = sample_;  // save just the max levels
      else if (sample_ < signalMin)
        signalMin = sample_;  // save just the min levels
    }
  }

  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  loudness_ = peakToPeak;
  return peakToPeak;
}

int SenseBrightness() { return analogRead(kLightPin); }

Thing thing((const char *)DEVICE_NAME, 60, SafeSerial);

Value sound_value((const char *)"sound", SenseLoudness, 0, 2048, 5000);
Value brightness((const char *)"brightness", SenseBrightness, 0, 1024, 5000);

void SetupSerial() { SafeSerial.begin(115200); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kSoundPin, INPUT);
  pinMode(kLightPin, INPUT);

  // Attach modules
}

void SetupThing() {
  // Setup Functions

  // Setup Values
  thing.Add(sound_value);
  thing.Add(brightness);

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
