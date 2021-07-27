#include "thing.h"

const int kIRPin = 7;
const int kLightPin = A0;

int led_status_ = 0;

unsigned int kOnSignal[] = {
    9024, 4460, 584, 564,  548, 520,  584, 532,   576,  568,  544, 528,
    580,  528,  588, 528,  576, 532,  584, 1636,  576,  1644, 576, 1644,
    580,  1632, 588, 528,  576, 1616, 604, 1632,  588,  1640, 576, 1640,
    576,  1632, 588, 524,  576, 572,  540, 572,   540,  532,  576, 532,
    584,  520,  588, 528,  580, 528,  588, 1632,  584,  1676, 540, 1632,
    584,  1640, 576, 1632, 588, 1632, 588, 40848, 9024, 2216, 584};

unsigned int kOffSignal[] = {
    9008, 4456, 580, 528,  576, 536,  576, 524,   584,  532,  576, 532,
    576,  532,  576, 532,  568, 540,  580, 1628,  580,  1640, 576, 1648,
    576,  1636, 576, 532,  576, 1644, 580, 1632,  588,  1632, 584, 524,
    584,  1636, 572, 532,  572, 528,  576, 540,   568,  568,  540, 540,
    568,  528,  588, 1632, 576, 496,  624, 1636,  572,  1640, 576, 1644,
    576,  1632, 584, 1636, 576, 1636, 584, 40816, 9012, 2216, 576};

void custom_delay_usec(unsigned long uSecs) {
  unsigned long Start = micros();
  unsigned long endMicros = Start + uSecs;
  if (endMicros < Start) {  // Check if overflow
    while (micros() > Start) {
    }
  }
  while (micros() < endMicros) {
  }
}

void space(unsigned int Time) {
  digitalWrite(kIRPin, LOW);
  if (Time > 0) {
    unsigned long Start = micros();
    unsigned long endMicros = Start + Time - 4;
    custom_delay_usec(Time);
  }
}

void mark(unsigned int Time) {
  unsigned long Start = micros();
  unsigned long endMicros = Start + Time;
  int count = 0;

  while (micros() < endMicros) {
    digitalWrite(kIRPin, HIGH);
    custom_delay_usec(10);
    digitalWrite(kIRPin, LOW);
    custom_delay_usec(9);
    count++;
  }
}

void SendIR(unsigned int *signal, int length) {
  digitalWrite(kIRPin, LOW);

  for (unsigned int i = 0; i < length; i++) {
    unsigned long Start = micros();
    if (i & 1)
      space(signal[i]);
    else
      mark(signal[i]);
  }
  digitalWrite(kIRPin, LOW);
  // delay(100);
}

int SenseLedStatus() { return led_status_; }
int SenseBrightness() { return analogRead(kLightPin); }

void ActuateLedOn() {
  SOPLOGLN(F("[LED DEBUG]: LED On"));
  SendIR(kOnSignal, sizeof(kOnSignal) / sizeof(kOnSignal[0]));
  led_status_ = 1;
}

void ActuateLedOff() {
  SOPLOGLN(F("[LED DEBUG]: LED Off"));
  SendIR(kOffSignal, sizeof(kOffSignal) / sizeof(kOffSignal[0]));
  led_status_ = 0;
}

Thing thing((const char *)"SmartPotCL1", 60, SafeSerial);

Value led_status((const char *)"led_status", SenseLedStatus, 0, 3, 3000);
Value brightness((const char *)"brightness", SenseBrightness, 0, 1024, 3000);
Function led_on((const char *)"led_on", ActuateLedOn);
Function led_off((const char *)"led_off", ActuateLedOff);
Tag tag_SmartPot("SmartPot");
Tag tag_SmartPotCL("SmartPotCL");

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kIRPin, OUTPUT);

  // Attach modules
}

void SetupThing() {
  // Setup Functions
  led_on.AddTag(tag_SmartPot);
  led_on.AddTag(tag_SmartPotCL);
  led_off.AddTag(tag_SmartPot);
  led_off.AddTag(tag_SmartPotCL);
  thing.Add(led_on);  // pin 7
  thing.Add(led_off);

  // Setup Values
  led_status.AddTag(tag_SmartPot);
  led_status.AddTag(tag_SmartPotCL);
  brightness.AddTag(tag_SmartPot);
  brightness.AddTag(tag_SmartPotCL);
  thing.Add(led_status);
  thing.Add(brightness);  // A0

  // Setup Thing
  thing.Setup();
}

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { thing.Loop(); }