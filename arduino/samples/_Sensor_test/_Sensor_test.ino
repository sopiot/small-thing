#include <DHT.h>
#include <pm2008_i2c.h>

#define LIGHT_PIN A0
#define SOUND_PIN A1
#define HUMID_PIN 2
#define DISTANCE_TRIGGER_PIN 3
#define DISTANCE_RECEIVE_PIN 4
#define RELAY_PIN 5
#define SEND_PIN 6
#define MOVEMENT_PIN 7

#define DHTTYPE DHT11

DHT dht(HUMID_PIN, DHTTYPE);
PM2008_I2C pm2008_i2c;

int LightSensor() {
  int light_value = (int)analogRead(LIGHT_PIN);
  Serial.print("light : ");
  Serial.println(light_value);

  return light_value;
}

int DistanceSensor() {
  long Duration = 0;
  digitalWrite(DISTANCE_TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(DISTANCE_TRIGGER_PIN, HIGH);  // Trigger pin to HIGH
  delayMicroseconds(10);                     // 10us high
  digitalWrite(DISTANCE_TRIGGER_PIN, LOW);   // Trigger pin to HIGH

  Duration = pulseIn(DISTANCE_RECEIVE_PIN,
                     HIGH);  // Waits for the echo pin to get high
                             // returns the Duration in microseconds
  int Distance_mm =
      ((Duration / 2.9) / 2);  // Use function to calculate the distance
  Serial.print("distance : ");
  Serial.println(Distance_mm);

  return Distance_mm;
}

void HumidTempSensor() {
  float h = dht.readHumidity();     // 변수 h에 습도 값을 저장
  float t = dht.readTemperature();  // 변수 t에 온도 값을 저장

  Serial.print("Humidity: ");     // 문자열 Humidity: 를 출력한다.
  Serial.print(h);                // 변수 h(습도)를 출력한다.
  Serial.print("%#");             // %를 출력한다
  Serial.print("Temperature: ");  // 이하생략
  Serial.print(t);
  Serial.println(" C");
}

int SoundSensor() {
  int sound_value = (int)analogRead(SOUND_PIN);
  Serial.print("sound : ");
  Serial.println(sound_value);

  return sound_value;
}

int DustSensor() {
  uint8_t ret = pm2008_i2c.read();
  int pm10 = 0;
  int pm2p0 = 0;
  int pm1p0 = 0;
  if (ret == 0) {
    Serial.print("PM 1.0 (GRIMM) : ");
    pm1p0 = pm2008_i2c.pm1p0_grimm;
    Serial.println(pm2008_i2c.pm1p0_grimm);
    Serial.print("PM 2.5 (GRIMM) : : ");
    pm2p0 = pm2008_i2c.pm2p5_grimm;
    Serial.println(pm2008_i2c.pm2p5_grimm);
    Serial.print("PM 10 (GRIMM) : : ");
    pm10 = pm2008_i2c.pm10_grimm;
    Serial.println(pm2008_i2c.pm10_grimm);
    Serial.print("PM 1.0 (TSI) : ");
    Serial.println(pm2008_i2c.pm1p0_tsi);
    Serial.print("PM 2.5 (TSI) : : ");
    Serial.println(pm2008_i2c.pm2p5_tsi);
    Serial.print("PM 10 (TSI) : : ");
    Serial.println(pm2008_i2c.pm10_tsi);
    Serial.print("Number of 0.3 um : ");
    Serial.println(pm2008_i2c.number_of_0p3_um);
    Serial.print("Number of 0.5 um : ");
    Serial.println(pm2008_i2c.number_of_0p5_um);
    Serial.print("Number of 1 um : ");
    Serial.println(pm2008_i2c.number_of_1_um);
    Serial.print("Number of 2.5 um : ");
    Serial.println(pm2008_i2c.number_of_2p5_um);
    Serial.print("Number of 5 um : ");
    Serial.println(pm2008_i2c.number_of_5_um);
    Serial.print("Number of 10 um : ");
    Serial.println(pm2008_i2c.number_of_10_um);
  }

  return pm2p0;
}

int Movement_Sensor() {
  int movement_value = (int)digitalRead(MOVEMENT_PIN);
  Serial.print("movement_value : ");
  Serial.println(movement_value);

  return movement_value;
}

void setup() {
#if BOARD_SERIAL_IS_ONE
  Serial1.begin(115200);
#else
  Serial.begin(115200);
#endif

  dht.begin();
  pm2008_i2c.begin();
  pm2008_i2c.command();

  pinMode(HUMID_PIN, INPUT);
  pinMode(DISTANCE_TRIGGER_PIN, OUTPUT);
  pinMode(DISTANCE_RECEIVE_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(SEND_PIN, OUTPUT);
  pinMode(SOUND_PIN, INPUT);
  pinMode(MOVEMENT_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  delay(500);
  Serial.println("");
  HumidTempSensor();
  DistanceSensor();
  LightSensor();
  SoundSensor();
  DustSensor();
  Movement_Sensor();
}
