//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT Thing library
#include "thing.h"

// IR signal
unsigned int TV_ONOFF[] = {
    9080, 4420, 600, 516,  584, 524,  568, 1640,  576,  516,  588, 520,
    580,  516,  576, 524,  576, 524,  576, 1644,  584,  1640, 580, 520,
    576,  1644, 576, 1644, 584, 1644, 576, 1644,  576,  1640, 580, 520,
    588,  516,  580, 520,  576, 1636, 584, 512,   584,  516,  584, 524,
    576,  516,  580, 1640, 580, 1640, 588, 1640,  576,  524,  576, 1644,
    576,  1640, 588, 1640, 580, 1640, 576, 40148, 9068, 2196, 584};
unsigned int Air_Conditioner_ON[] = {
    3108, 9848, 476, 1524, 584, 432, 568, 444,  496, 540,  456, 1556,
    484,  524,  484, 524,  576, 424, 484, 504,  596, 432,  568, 396,
    520,  512,  504, 512,  576, 424, 508, 500,  584, 440,  484, 1556,
    504,  504,  592, 1496, 540, 416, 508, 516,  576, 432,  576, 452,
    456,  1544, 484, 1564, 568, 464, 528, 1468, 568, 1468, 484};
unsigned int Air_Conditioner_OFF[] = {
    3160, 9760, 552, 1512, 524, 468, 544, 480,  524, 480,  524, 1504,
    540,  480,  524, 480,  516, 488, 512, 1500, 544, 1500, 544, 480,
    512,  492,  512, 492,  512, 472, 540, 492,  512, 472,  540, 480,
    524,  488,  524, 480,  524, 480, 512, 492,  512, 1512, 524, 472,
    532,  1524, 524, 472,  540, 492, 512, 492,  512, 1504, 540};
unsigned int Bulb_ON[] = {
    9084, 4528, 560, 552,  556, 564,  556, 560,  548, 556,  560, 560,
    560,  560,  548, 572,  548, 560,  556, 1668, 560, 1672, 564, 1668,
    552,  1672, 560, 1668, 548, 1676, 556, 1668, 552, 1672, 564, 1668,
    552,  560,  556, 1676, 548, 560,  560, 560,  556, 560,  552, 1672,
    564,  556,  552, 568,  548, 1676, 548, 560,  560, 1672, 548, 1672,
    560,  1672, 548, 556,  556, 1672, 556};
unsigned int Bulb_OFF[] = {
    9112, 4520, 636, 480,  636, 488,  624, 492,  632, 520,  592, 480,
    632,  492,  560, 552,  628, 496,  600, 1624, 632, 1592, 616, 1612,
    612,  1616, 612, 1656, 592, 1604, 628, 1600, 624, 1604, 600, 1628,
    628,  1596, 632, 1592, 616, 508,  624, 492,  608, 508,  632, 1596,
    612,  504,  640, 484,  632, 484,  632, 488,  624, 1604, 632, 1592,
    632,  1596, 628, 484,  640, 1592, 632};

// Module libraries

// Pins
static const int kIRSendPin = 8;

//----------------------------------------
// Modules
//----------------------------------------

// Modules

//----------------------------------------
// Thing
//----------------------------------------

// Thing declaration
// Thing(class_name, alive_cycle, serial);
// Thing(class_name, serial);
Thing IRremote_thing((const char *)"IRremote", 60, SafeSerial);

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables

// Getter functions of each Value variable

// Value declarations
// Value(name, sense_function, min, max, period(ms));

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

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
  digitalWrite(kIRSendPin, LOW);
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
    digitalWrite(kIRSendPin, HIGH);
    custom_delay_usec(10);
    digitalWrite(kIRSendPin, LOW);
    custom_delay_usec(9);
    count++;
  }
}

void SendIR(unsigned int *signal, int length) {
  digitalWrite(kIRSendPin, LOW);

  // FIX IT!!!
  // i < 72  ->  hard coding
  for (unsigned int i = 0; i < 72; i++) {
    unsigned long Start = micros();
    if (i & 1)
      space(signal[i]);
    else
      mark(signal[i]);
  }

  digitalWrite(kIRSendPin, LOW);
}

void TVPowerFunction(void *pData) {
  // int arg_value = 1, res;
  // res = GetIntArgumentByName(pData, ARG_TV_SWITCH, &arg_value);
  // if (res == -1)
  //     return;

  SendIR(TV_ONOFF, sizeof(TV_ONOFF) / sizeof(unsigned int));
}

void AirOnFunction(void *pData) {
  // int arg_value = 1, res;
  // res = GetIntArgumentByName(pData, ARG_AIR_SWITCH, &arg_value);
  // if (res == -1)
  //     return;

  SendIR(Air_Conditioner_ON, sizeof(Air_Conditioner_ON) / sizeof(unsigned int));
}

void AirOffFunction(void *pData) {
  // int arg_value = 1, res;
  // res = GetIntArgumentByName(pData, ARG_AIR_SWITCH, &arg_value);
  // if (res == -1)
  //     return;

  SendIR(Air_Conditioner_OFF,
         sizeof(Air_Conditioner_OFF) / sizeof(unsigned int));
}

void BulbOnFunction(void *pData) {
  // int arg_value = 1, res;
  // res = GetIntArgumentByName(pData, ARG_BULB_SWITCH, &arg_value);
  // if (res == -1)
  //     return;

  SendIR(Bulb_ON, sizeof(Bulb_ON) / sizeof(unsigned int));
}

void BulbOffFunction(void *pData) {
  // int arg_value = 1, res;
  // res = GetIntArgumentByName(pData, ARG_BULB_SWITCH, &arg_value);
  // if (res == -1)
  //     return;

  SendIR(Bulb_OFF, sizeof(Bulb_OFF) / sizeof(unsigned int));
}

// Function declarations
// Function(name, actuate_function, arguments_num, function_attributes_num);
Function tv_power((const char *)"tv_power", TVPowerFunction, 0, 0);
Function air_on((const char *)"air_on", AirOnFunction, 0, 0);
Function air_off((const char *)"air_off", AirOffFunction, 0, 0);
Function bulb_on((const char *)"bulb_on", BulbOnFunction, 0, 0);
Function bulb_off((const char *)"bulb_off", BulbOffFunction, 0, 0);

//----------------------------------------
// Setup
//----------------------------------------

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kIRSendPin, OUTPUT);

  // Attach modules
}

void SetupThing() {
  // Setup Functions
  IRremote_thing.Add(tv_power);
  IRremote_thing.Add(air_on);
  IRremote_thing.Add(air_off);
  IRremote_thing.Add(bulb_on);
  IRremote_thing.Add(bulb_off);

  // Setup Values

  // Setup Thing
  IRremote_thing.Setup();
}

//----------------------------------------
// Main
//----------------------------------------

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { IRremote_thing.Loop(); }
