// #include "ota.h"
#include "thing.h"
#include "wiring_private.h"

#define CO2_CYCLE 2 * 1000
#define DEVICE_NAME "CO2Sensor3"

Uart mySerial(&sercom0, 5, 6, SERCOM_RX_PAD_1, UART_TX_PAD_0);

const int kPWNPin = 2;
const int kAnalogOutputPin = A0;

int analog_ppm_;
int pwm_ppm_;

void SERCOM0_Handler() { mySerial.IrqHandler(); }

int SenseAnalogOutputPPM() {
  double result = analogRead(kAnalogOutputPin) / 1024.0;
  return (int)(result / 0.0005);
}

int SensePWMOutputPPM() { return analog_ppm_; }

int SenseUartOutputPPM() {
  String inString;

  if (mySerial.available()) {
    inString = mySerial.readStringUntil('\n');
    SOPLOGLN((char *)inString.c_str());
  }

  inString.trim();
  inString = inString.substring(0, inString.indexOf('ppm') - 3);
  inString.trim();
  SOPLOGLN((char *)inString.c_str());
  if (inString.indexOf(' ') != -1) {
    inString = inString.substring(inString.indexOf(' '), inString.length());
    inString.trim();
    SOPLOGLN((char *)inString.c_str());
  }

  return atoi((char *)inString.c_str());
}

Thing thing((const char *)DEVICE_NAME, 60, SafeSerial);
Value analogPPM((const char *)"CO2", SenseUartOutputPPM, 0, 10000,
                CO2_CYCLE);  // CO2 Sensor output cycle is 2 SEC
// Value pwmPPM((const char *)"pwmPPM", SensePWMOutputPPM, 0, 2, 5000);

void SetupSerial() { SafeSerial.begin(115200); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kAnalogOutputPin, INPUT);
  //   pinMode(kPWNPin, INPUT);

  // Attach modules
}

void SetupThing() {
  // Setup Functions

  // Setup Values
  thing.Add(analogPPM);
  // thing.Add(pwmPPM);

  // Setup Thing
  thing.Setup();
}

void setup() {
  pinPeripheral(5, PIO_SERCOM_ALT);
  pinPeripheral(6, PIO_SERCOM_ALT);

  mySerial.begin(38400);

  SetupSerial();
  // thing.SetupXbee();
  SetupModules();
  // WiFi_Setup("SoPIoT_2.4G", "/PeaCE/#1", DEVICE_NAME, "0000");
  SetupThing();
}

void loop() {
  // SOPOTA();
  thing.Loop();
}
