//----------------------------------------
// Libraries
//----------------------------------------

// SoPIoT Thing library
#include <thing.h>

#include "wiring_private.h"

Uart mySerial(&sercom0, 5, 6, SERCOM_RX_PAD_1, UART_TX_PAD_0);

void SERCOM0_Handler() { mySerial.IrqHandler(); }

// Module libraries

// Pins
static const int kPWNPin = 2;
static const int kAnalogOutputPin = A0;

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
Thing CO2Sensor_thing((const char *)"CO2Sensor-3", 60, SafeSerial);

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables
int analog_ppm_;
int pwm_ppm_;

// Getter functions of each Value variable

int SenseAnalogOutputPPM() {
  double result = analogRead(kAnalogOutputPin) / 1024.0;
  return (int)(result / 0.0005);
}

int SensePWMOutputPPM() { return analog_ppm_; }
int SenseUartOutputPPM() {
  String inString;

  if (mySerial.available()) {
    inString = mySerial.readStringUntil('\n');
    SOPLOGLN((char*)inString.c_str());
  }

  inString.trim();
  inString = inString.substring(0, inString.indexOf(' '));

  return atoi((char*)inString.c_str());
}

// Value declarations
// Value(name, sense_function, min, max, period(ms));
Value analogPPM((const char *)"co2_level", SenseUartOutputPPM, 0, 10000, 30000);
// Value pwmPPM((const char *)"pwmPPM", SensePWMOutputPPM, 0, 2, 5000);

//----------------------------------------
// Functions
// an ActuateXXX actuates a Function XXX
//----------------------------------------

// Function declarations
// Function(name, actuate_function, arguments_num, function_attributes_num);

//----------------------------------------
// Setup
//----------------------------------------

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kAnalogOutputPin, INPUT);
  //   pinMode(kPWNPin, INPUT);

  // Attach modules
}

void SetupThing() {
  // Setup Functions

  // Setup Values
  CO2Sensor_thing.Add(analogPPM);
  // CO2Sensor_thing.Add(pwmPPM);

  // Setup Thing
  CO2Sensor_thing.Setup();
}

//----------------------------------------
// Main
//----------------------------------------

void setup() {
  pinPeripheral(5, PIO_SERCOM_ALT);
  pinPeripheral(6, PIO_SERCOM_ALT);

  mySerial.begin(38400);

  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { CO2Sensor_thing.Loop(); }