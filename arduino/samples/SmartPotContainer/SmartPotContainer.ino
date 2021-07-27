// pump(relay) + water level sensor + light sensor + IR

//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT Thing library
#include "thing.h"
#define WATER_LEVEL_SENSOR_NUM 2

//----------------------------------------
// Thing
//----------------------------------------
// Thing(class_name, alive_cycle, serial);
// Thing(class_name, serial);
// class name should not include '_'
Thing smart_pot_container_thing((const char *)"SmartPotContainer", 60,
                                SafeSerial);

/* Remote Controller Signals ( 110V power charging)
unsigned int kOn[] =
{9140,4484,588,528,580,528,588,532,588,528,584,532,584,536,588,532,584,532,584,1632,588,1632,588,1640,584,1632,584,536,584,1640,588,1632,588,1640,580,540,584,1636,584,560,560,520,584,536,580,528,588,568,552,528,588,1632,584,532,584,1632,596,1632,584,1628,588,1644,584,1636,588,1644,584,40932,9144,2224,576};
unsigned int kOff[] =
{9160,4484,588,528,588,528,588,532,584,532,584,564,556,532,584,520,596,528,588,1632,588,1640,580,1640,584,1644,584,520,596,1632,588,1632,596,1628,584,1644,584,1636,584,532,576,532,588,532,588,528,588,528,584,532,588,532,584,532,584,1636,584,1644,588,1640,576,1640,584,1636,584,1644,584,40944,9144,2224,584};
unsigned int kBrigthnessUp[] =
{9124,4488,588,532,584,532,576,544,572,528,588,528,588,528,588,528,588,532,584,1636,584,1644,576,1640,588,1644,576,532,588,1676,548,1644,576,1640,588,528,580,540,576,540,580,528,584,532,584,536,584,560,548,536,584,1640,580,1640,588,1640,576,1648,588,1632,588,1640,588,1640,580,1640,584};
unsigned int kBrightnessDown[] =
{9140,4496,576,532,588,540,568,536,584,544,576,532,584,532,588,532,576,536,580,1652,576,1648,576,1648,576,1648,576,540,576,1652,576,1640,576,1652,576,1644,584,540,576,532,576,540,568,540,580,540,580,540,584,532,584,536,584,1644,576,1652,568,1680,552,1640,576,1656,576,1644,576,1652,576,40956,9124,2232,576};
unsigned int kRed[] =
{9180,4460,612,512,604,504,612,512,604,500,604,508,604,512,612,496,616,512,604,1616,612,1616,604,1620,608,1612,612,512,604,1612,612,1616,604,1616,612,508,608,508,604,1620,604,512,604,512,604,512,612,504,612,504,616,1608,616,1612,608,508,612,1604,612,1612,612,1612,552,1672,548,1668,552,40972,9152,2220,580};
unsigned int kGreen[] =
{9164,4484,588,528,588,528,588,528,584,540,584,536,588,532,584,532,584,572,552,1644,576,1644,588,1644,584,1636,656,464,584,1668,552,1640,588,1672,548,1644,660,452,660,1572,576,536,580,540,652,460,660,464,584,532,652,464,584,1644,584,532,588,1644,576,1644,584,1636,584,1684,548,1640,580,40952,9160,2224,584};
unsigned int kBlue[] =
{9232,4428,644,480,644,508,596,524,600,516,600,516,600,516,600,472,648,508,608,1580,644,1584,640,1584,584,1648,588,532,584,1644,580,1640,588,1632,596,532,584,1636,592,1632,588,540,576,532,588,536,580,528,596,520,596,1636,584,532,584,540,580,1640,588,1640,584,1636,596,1632,588,1640,588};
unsigned int kWhite[] =
{9160,4492,580,536,576,544,576,540,576,544,576,532,584,540,580,536,580,540,576,1644,584,1644,576,1644,584,1644,576,540,580,1652,580,1640,588,1672,548,1648,588,1640,580,1640,588,528,584,536,588,532,584,532,584,536,584,528,588,540,576,540,580,1640,584,1636,584,1644,588,1640,576,1640,588,40952,9132,2256,596};
unsigned int kOrange[] =
{9152,4488,584,532,584,532,660,464,576,540,580,568,552,536,580,540,576,524,584,1644,648,1576,576,1644,588,1644,652,460,576,1644,648,1576,660,1564,652,468,580,528,588,540,644,1576,576,532,584,532,660,464,572,540,580,1640,588,1640,580,1640,588,532,656,1568,584,1640,588,1644,580,1644,584};
unsigned int kYellow[] =
{9152,4488,584,532,584,536,588,532,584,528,588,540,580,528,588,540,588,532,584,1636,584,1644,584,1640,576,1644,660,460,584,1644,576,1648,584,1640,588,1632,588,540,576,540,584,1636,584,572,548,532,584,524,588,528,660,460,588,1640,588,1632,584,540,576,1644,584,1640,576,1644,588,1640,584,40956,9144,2216,588};
unsigned int kCyan[] =
{9200,4464,604,512,612,500,612,500,612,504,612,504,612,504,608,508,616,500,616,1612,612,1616,612,1604,612,1608,616,500,616,1612,616,1612,612,1616,612,504,612,1612,604,512,616,1612,608,508,612,504,612,504,556,564,612,1604,560,568,548,1668,552,568,552,1672,548,1672,556,1668,556,1672,552};
unsigned int kPurple[] =
{9144,4484,588,532,588,572,548,532,584,564,556,524,592,520,596,532,588,528,588,1668,548,1636,596,1632,588,1628,600,520,600,1632,588,1640,584,1636,592,1636,584,1640,588,560,552,1640,584,572,548,524,592,520,596,532,576,532,580,528,588,1644,588,528,580,1636,584,1644,584,1624,588,1644,584,40940,9152,2216,584};
unsigned int kMusic1[] =
{9160,4488,588,520,588,528,588,568,552,528,588,528,584,532,584,532,584,532,588,1632,576,1640,588,1640,580,1640,584,536,584,1644,576,1640,660,1560,660,460,588,532,656,464,660,452,584,1644,588,528,588,528,588,540,576,1644,584,1636,584,1644,584,1644,580,540,580,1648,580,1640,584,1644,576,40952,9160,2216,584};
*/
// Remote Controller Signals (USB power charging)
unsigned int kOnSignal[] = {
    9024, 4460, 584, 564,  548, 520,  584, 532,   576,  568,  544, 528,
    580,  528,  588, 528,  576, 532,  584, 1636,  576,  1644, 576, 1644,
    580,  1632, 588, 528,  576, 1616, 604, 1632,  588,  1640, 576, 1640,
    576,  1632, 588, 524,  576, 572,  540, 572,   540,  532,  576, 532,
    584,  520,  588, 528,  580, 528,  588, 1632,  584,  1676, 540, 1632,
    584,  1640, 576, 1632, 588, 1632, 588, 40848, 9024, 2216, 584};
// unsigned int kOffSignal[] =
// {9080,4396,644,528,576,536,576,532,576,532,576,532,576,536,576,540,580,500,604,1572,644,1620,604,1576,640,1576,640,540,568,1584,644,1612,596,1584,640,532,576,1576,640,544,560,520,596,528,580,540,568,508,608,504,596,1584,632,512,604,1536,676,1584,632,1584,640,1572,640,1576,640,1588,628,40796,9080,2152,640};
unsigned int kOffSignal[] = {
    9008, 4456, 580, 528,  576, 536,  576, 524,   584,  532,  576, 532,
    576,  532,  576, 532,  568, 540,  580, 1628,  580,  1640, 576, 1648,
    576,  1636, 576, 532,  576, 1644, 580, 1632,  588,  1632, 584, 524,
    584,  1636, 572, 532,  572, 528,  576, 540,   568,  568,  540, 540,
    568,  528,  588, 1632, 576, 496,  624, 1636,  572,  1640, 576, 1644,
    576,  1632, 584, 1636, 576, 1636, 584, 40816, 9012, 2216, 576};
unsigned int kRedSignal[] = {
    9052, 4396, 640, 536,  588, 520,  580, 536,   572,  508,  604, 532,
    576,  540,  576, 504,  600, 512,  600, 1572,  640,  1576, 640, 1624,
    596,  1580, 644, 532,  576, 1576, 640, 1576,  640,  1580, 644, 528,
    580,  540,  572, 1580, 640, 536,  576, 544,   576,  532,  576, 532,
    576,  540,  568, 1584, 640, 1576, 652, 504,   604,  1576, 648, 1572,
    640,  1584, 632, 1584, 640, 1572, 640, 40828, 9084, 2156, 640};
unsigned int kGreenSignal[] = {
    9052, 4396, 640, 544,  576, 532,  576, 528,   580,  508,  608, 528,
    580,  508,  596, 540,  576, 532,  576, 1576,  648,  1576, 640, 1584,
    632,  1584, 644, 512,  604, 1576, 640, 1576,  648,  1576, 580, 1632,
    588,  568,  540, 1632, 584, 532,  588, 520,   584,  524,  584, 536,
    588,  556,  552, 560,  548, 1628, 588, 568,   540,  1636, 584, 1636,
    584,  1640, 580, 1632, 588, 1632, 588, 40892, 9032, 2216, 584};
unsigned int kBlueSignal[] = {
    9076, 4396, 644, 508,  604, 512,  596, 508,   604,  504,  604, 472,
    640,  512,  596, 512,  604, 504,  604, 1572,  644,  1584, 644, 1572,
    640,  1576, 640, 512,  604, 1576, 640, 1584,  632,  1584, 632, 520,
    592,  1576, 640, 1588, 628, 516,  604, 512,   596,  508,  608, 508,
    596,  512,  604, 1576, 640, 516,  596, 512,   604,  1576, 640, 1584,
    632,  1620, 600, 1580, 632, 1584, 640, 40796, 9020, 2216, 588};
unsigned int kStrobeSignal[] = {
    9020, 4464, 568, 544,  572, 528,  580, 528,   576,  540,  580, 528,
    580,  524,  584, 532,  576, 532,  576, 1648,  572,  1640, 576, 1644,
    576,  1644, 576, 532,  576, 1644, 576, 1652,  572,  1640, 580, 1640,
    580,  1640, 576, 1644, 576, 1636, 584, 528,   580,  568,  544, 536,
    580,  528,  580, 528,  584, 532,  576, 536,   576,  572,  540, 1644,
    580,  1644, 580, 1644, 580, 1632, 584, 40884, 9024, 2208, 588};

// Pins
const int kLightPin = A0;       // light sensor
const int kTransmitterPin = 7;  // IR transmitter
const int kWaterLevelPin[WATER_LEVEL_SENSOR_NUM] = {2, 3};
const int kRelayPin = 6;     // pump
const int kMiniRedPin = 12;  // TODO: led status for water level low

// led helper function
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
  digitalWrite(kTransmitterPin, LOW);
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
    digitalWrite(kTransmitterPin, HIGH);
    custom_delay_usec(10);
    digitalWrite(kTransmitterPin, LOW);
    custom_delay_usec(9);
    count++;
  }
}

void SendIR(unsigned int *signal, int length) {
  digitalWrite(kTransmitterPin, LOW);

  for (unsigned int i = 0; i < length; i++) {
    unsigned long Start = micros();
    if (i & 1)
      space(signal[i]);
    else
      mark(signal[i]);
  }
  digitalWrite(kTransmitterPin, LOW);
}

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables
int pump_status_ = 0;
int water_level_[2];
int water_percentage_ = 0;
double unit = 100.0 / WATER_LEVEL_SENSOR_NUM;
int brightness_ = 0;
int led_status_ = 0;  // for plant(value in IR)
int led_color_ = 0;   // 0: red, 1: green, 2: strobe

int mini_led_status_ = 0;  // for water level status

// Getter functions of each Value variable
int SenseWaterLevel() {
  double sum = 0;

  for (int i = 0; i < WATER_LEVEL_SENSOR_NUM; i++) {
    water_level_[i] = digitalRead(kWaterLevelPin[i]);
    sum += (water_level_[i] == 0) ? unit : 0.0;
  }

  water_percentage_ = (int)sum;
  if (water_percentage_ == 0) {
    digitalWrite(kMiniRedPin, HIGH);
  } else {
    digitalWrite(kMiniRedPin, LOW);
  }

  return water_percentage_;
}

// Getter functions of each Value variable
int SensePumpStatus() { return pump_status_; }

int SenseBrightness() {
  brightness_ = analogRead(kLightPin);
  return brightness_;
}

int SenseLEDStatus() { return led_status_; }

// Value declarations
// Value(name, sense_function, min, max, period(ms));
Value water_level((const char *)"water_level", SenseWaterLevel, 0, 100, 1000);
Value brightness((const char *)"brightness", SenseBrightness, 0, 1000, 3000);
Value pump_status((const char *)"pump_status", SensePumpStatus, 0, 1, 1000);
Value led_status((const char *)"led_status", SenseLEDStatus, 0, 1, 3000);

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

void ActuateLEDOn(void *pData) {
  SendIR(kOnSignal, sizeof(kOnSignal) / sizeof(kOnSignal[0]));
  led_status_ = 1;
}

void ActuateLEDOff(void *pData) {
  SendIR(kOffSignal, sizeof(kOffSignal) / sizeof(kOffSignal[0]));
  led_status_ = 0;
}

void ActuateChangeColor(void *pData) {
  if (led_status_ == 0) {
    return;
  }

  led_color_ = (led_color_ + 1) % 3;
  switch (led_color_) {
    case 0:
      SendIR(kRedSignal, sizeof(kRedSignal) / sizeof(kRedSignal[0]));
      break;
    case 1:
      SendIR(kGreenSignal, sizeof(kGreenSignal) / sizeof(kGreenSignal[0]));
      break;
    case 2:
      SendIR(kBlueSignal, sizeof(kBlueSignal) / sizeof(kBlueSignal[0]));
      break;
    default:
      SendIR(kGreenSignal, sizeof(kGreenSignal) / sizeof(kGreenSignal[0]));
      break;
  }
}

// pump function
void ActuatePumpOn(void *pData) {
  digitalWrite(kRelayPin, HIGH);
  pump_status_ = 1;
}

void ActuatePumpOff(void *pData) {
  digitalWrite(kRelayPin, LOW);
  pump_status_ = 0;
}

Function led_on((const char *)"led_on", ActuateLEDOn, 0, 0);
Function led_off((const char *)"led_off", ActuateLEDOff, 0, 0);
Function change_color((const char *)"change_color", ActuateChangeColor, 0, 0);

Function pump_on((const char *)"pump_on", ActuatePumpOn, 0, 0);
Function pump_off((const char *)"pump_off", ActuatePumpOff, 0, 0);

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kRelayPin, OUTPUT);
  pinMode(kTransmitterPin, OUTPUT);
  pinMode(kMiniRedPin, OUTPUT);
  for (int i = 0; i < WATER_LEVEL_SENSOR_NUM; i++) {
    pinMode(kWaterLevelPin[i], INPUT);
  }

  // Setup initial state
  // pump
  digitalWrite(kRelayPin, LOW);
  pump_status_ = 0;

  // led
  SendIR(kOffSignal, sizeof(kOffSignal) / sizeof(kOffSignal[0]));
  led_status_ = 0;
  led_color_ = 0;
}

void SetupThing() {
  // Setup Functions
  smart_pot_container_thing.Add(pump_on);
  smart_pot_container_thing.Add(pump_off);

  smart_pot_container_thing.Add(led_on);
  smart_pot_container_thing.Add(led_off);
  smart_pot_container_thing.Add(change_color);

  // Setup Values
  smart_pot_container_thing.Add(pump_status);
  smart_pot_container_thing.Add(water_level);

  smart_pot_container_thing.Add(brightness);
  smart_pot_container_thing.Add(led_status);

  // Setup Thing
  smart_pot_container_thing.Setup();
}

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { smart_pot_container_thing.Loop(); }