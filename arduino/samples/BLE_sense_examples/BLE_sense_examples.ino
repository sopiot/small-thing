/************************************
library for Arduino Nano 33 BLE Sense
************************************/

#include <Arduino_APDS9960.h>
#include <Arduino_HTS221.h>
#include <Arduino_LPS22HB.h>
#include <Arduino_LSM9DS1.h>

float accel_x, accel_y, accel_z;
float gyro_x, gyro_y, gyro_z;
float mag_x, mag_y, mag_z;
float Pressure;
float Temperature, Humidity;
int Proximity;

const int ledPin1 = 22;
const int ledPin2 = 23;
const int ledPin3 = 24;

void setup() {
  Serial.begin(9600);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  if (!IMU.begin()) {  // LSM9DSI센서 시작
    Serial.println("LSM9DSI sensor error!");
    while (1)
      ;
  }
  if (!BARO.begin()) {  // LPS22HB센서 시작
    Serial.println("LPS22HB sensor error!");
    while (1)
      ;
  }
  if (!HTS.begin()) {  // HTS221센서 시작
    Serial.println("HTS221 sensor error!");
    while (1)
      ;
  }
  if (!APDS.begin()) {  // APDS9960센서 시작
    Serial.println("APDS9960 sensor error!");
    while (1)
      ;
  }
}
void loop() {
  //가속도센서
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(accel_x, accel_y, accel_z);
    Serial.print("Acceleration sensor = ");
    Serial.print(accel_x);
    Serial.print(", ");
    Serial.print(accel_y);
    Serial.print(", ");
    Serial.println(accel_z);
  }
  //자이로센서
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gyro_x, gyro_y, gyro_z);
    Serial.print("gyro sensor = ");
    Serial.print(gyro_x);
    Serial.print(", ");
    Serial.print(gyro_y);
    Serial.print(", ");
    Serial.println(gyro_z);
  }
  //지자계센서
  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(mag_x, mag_y, mag_z);
    Serial.print("magneticField sensor = ");
    Serial.print(mag_x);
    Serial.print(", ");
    Serial.print(mag_y);
    Serial.print(", ");
    Serial.println(mag_z);
  }
  //압력센서
  Pressure = BARO.readPressure();
  Serial.print("Pressure sensor = ");
  Serial.println(Pressure);
  //온도센서
  Temperature = HTS.readTemperature();
  Serial.print("Temperature sensor = ");
  Serial.println(Temperature);
  //습도센서
  Humidity = HTS.readHumidity();
  Serial.print("Humidity sensor = ");
  Serial.println(Humidity);
  //근접센서
  Proximity = APDS.readProximity();
  Serial.print("Proximity sensor = ");
  Serial.println(Proximity);
  //제스쳐센서
  if (APDS.gestureAvailable()) {
    int gesture = APDS.readGesture();
    switch (gesture) {
      case GESTURE_UP:
        Serial.println("Detected UP gesture");
        digitalWrite(ledPin1, LOW);
        delay(1000);
        digitalWrite(ledPin1, HIGH);
        break;

      case GESTURE_DOWN:
        Serial.println("Detected DOWN gesture");
        digitalWrite(ledPin2, LOW);
        delay(1000);
        digitalWrite(ledPin2, HIGH);
        break;

      case GESTURE_LEFT:
        Serial.println("Detected LEFT gesture");
        digitalWrite(ledPin3, LOW);
        delay(1000);
        digitalWrite(ledPin3, HIGH);
        break;

      case GESTURE_RIGHT:
        Serial.println("Detected RIGHT gesture");
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
        digitalWrite(LED_BUILTIN, LOW);
        break;

      default:
        // ignore
        break;
    }
  }

  Serial.println("----------------------------------------------------");
  delay(500);
}
