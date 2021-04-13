//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT Thing library
#include "thing.h"

// Module libraries

// Pins
static const int kSoundPin = A0;

//----------------------------------------
// Modules
//----------------------------------------

// Modules

// Global variables
const int kSampleWindow = 50;  // Sample window width in mS (50 mS = 20Hz)

unsigned int sample_ = 0;
int loudness_ = 0;

//----------------------------------------
// Thing
//----------------------------------------
Thing thing((const char *)"SoundSensor", 60, SafeSerial);

//----------------------------------------
// Value callback functions
//----------------------------------------
int SenseLoudness() {
  unsigned long startMillis = millis();  // Start of sample window
  unsigned int peakToPeak = 0;           // peak-to-peak level
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS

  while (millis() - startMillis < kSampleWindow) {
    sample_ = analogRead(0);
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

Value sound_value((const char *)"sound_value", SenseLoudness, 0, 2048, 1000);

//----------------------------------------
// Functions callback functions
//----------------------------------------

//----------------------------------------
// Setup
//----------------------------------------
void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kSoundPin, INPUT);

  // Attach modules
}

void SetupThing() {
  // Setup Functions

  // Setup Values
  thing.Add(sound_value);

  // Setup Thing
  thing.Setup();
}

void setup() {
  SetupSerial();
  SetupModules();

  SetupThing();
}

//----------------------------------------
// Main
//----------------------------------------
void loop() { thing.Loop(); }
