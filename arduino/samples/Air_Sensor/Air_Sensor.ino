#include <thing.h>

// Module libraries
#include <DHT.h>
#include <GP2YDustSensor.h>
#include <pm2008_i2c.h>

// Pins
static const int kdustPin = A0;
static const int khumidPin = 2;
static const int kdustledPin = 3;

//----------------------------------------
// Modules
//----------------------------------------
#define kdustSenseMode 1

// Modules
#define HUMID_VALUE "humid"
#define TEMP_VALUE "temp"
#define DUST_VALUE "dust"
// #define DHT_TYPE DHT11
#define DHT_TYPE DHT22
DHT dht(khumidPin, DHT_TYPE);
PM2008_I2C pm2008_i2c;
GP2YDustSensor dustSensor(GP2YDustSensorType::GP2Y1010AU0F, kdustledPin,
                          kdustPin);

//----------------------------------------
// Thing
//----------------------------------------

// Thing declaration
// Thing(class_name, alive_cycle, serial);
// Thing(class_name, serial);
Thing thing((const char *)"AirSensor-3", 60, SafeSerial);

//----------------------------------------
// Values
// an SenseXXX overwrites a Value XXX
//----------------------------------------

// Value variables
int humid_status_;
int temp_status_;
int dust_status_;

// Getter functions of each Value variable
int SenseHumidStatus() { return humid_status_ = (int)dht.readHumidity(); }
int SenseTempStatus() { return temp_status_ = (int)dht.readTemperature(); }
int SensePM2008DustStatus() {
  uint8_t ret = pm2008_i2c.read();
  if (ret == 0) {
    // Serial.print("PM 1.0 (GRIMM) : ");
    // Serial.println(pm2008_i2c.pm1p0_grimm);
    // Serial.print("PM 2.5 (GRIMM) : : ");
    // Serial.println(pm2008_i2c.pm2p5_grimm);
    // Serial.print("PM 10 (GRIMM) : : ");
    // Serial.println(pm2008_i2c.pm10_grimm);
    // Serial.print("PM 1.0 (TSI) : ");
    // Serial.println(pm2008_i2c.pm1p0_tsi);
    // Serial.print("PM 2.5 (TSI) : : ");
    // Serial.println(pm2008_i2c.pm2p5_tsi);
    // Serial.print("PM 10 (TSI) : : ");
    // Serial.println(pm2008_i2c.pm10_tsi);
    // Serial.print("Number of 0.3 um : ");
    // Serial.println(pm2008_i2c.number_of_0p3_um);
    // Serial.print("Number of 0.5 um : ");
    // Serial.println(pm2008_i2c.number_of_0p5_um);
    // Serial.print("Number of 1 um : ");
    // Serial.println(pm2008_i2c.number_of_1_um);
    // Serial.print("Number of 2.5 um : ");
    // Serial.println(pm2008_i2c.number_of_2p5_um);
    // Serial.print("Number of 5 um : ");
    // Serial.println(pm2008_i2c.number_of_5_um);
    // Serial.print("Number of 10 um : ");
    // Serial.println(pm2008_i2c.number_of_10_um);

    return (int)pm2008_i2c.number_of_1_um;
  }

  return -1;
}

float pulse2ugm3(unsigned long pulse) {
  float value = (pulse - 1400) / 14.0;                // pulse에 -1400을 하고 14.0을 나누어 value에 저장합니다.
  if (value > 300) {                                        // value가 300보다 크면
    value = 0;                                                // value이 0으로 저장합니다.
  }
  return value;                                             // value 값을 반환합니다.
}

int SenseGP2YDustStatus() {
  // dustSensor.getDustDensity();
  // return (int)dustSensor.getRunningAverage();
int duration = pulseIn(3, HIGH);
  int samplingTime = 280;
  int deltaTime = 40;
  int sleepTime = 9680;

  float voMeasured = 0.0;
  float calcVoltage = 0.0;
  float dustDensity = 0.0;

  digitalWrite(kdustledPin, LOW);  // power on the LED
  delayMicroseconds(samplingTime);

  voMeasured = analogRead(kdustPin);  // read the dust value

  delayMicroseconds(deltaTime);
  digitalWrite(kdustledPin, HIGH);  // turn the LED off
  delayMicroseconds(sleepTime);

  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (5.0 / 1024.0);

  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  dustDensity = 0.17 * calcVoltage - 0.1;

  Serial.print("Raw Signal Value (0-1023): ");
  Serial.print(voMeasured);

  Serial.print(" - Voltage: ");
  Serial.print(calcVoltage);

  Serial.print(" - Dust Density: ");
  Serial.println(dustDensity);  // unit: mg/m3

  return dustDensity;
}

// Value declarations
// Value(name, sense_function, min, max, period(ms));
Value humid_status((const char *)"humid_status", SenseHumidStatus, 0, 100,
                   30000);
Value temp_status((const char *)"temp_status", SenseTempStatus, 0, 100, 30000);
Value dust_status((const char *)"dust_status", SensePM2008DustStatus, 0, 600,
                  30000);

// Value dust_status((const char *)"dust_status", SensePM2008DustStatus, 0,
// 10000,
//                   1000);
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
  pinMode(khumidPin, INPUT);

  // Attach modules
  dht.begin();
  pm2008_i2c.begin();
  pm2008_i2c.command();
  // dustSensor.setBaseline(
  //     0.4);  // set no dust voltage according to your own experiments
  // dustSensor.setCalibrationFactor(
  //     1.7);  // calibrate against precision instrument
  // dustSensor.begin();
  delay(100);
}

void SetupThing() {
  // Setup Functions

  // Setup Values
  thing.Add(humid_status);
  thing.Add(temp_status);
  thing.Add(dust_status);

  // Setup Thing
  thing.Setup();
}

//----------------------------------------
// Main
//----------------------------------------

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { thing.Loop(); }
