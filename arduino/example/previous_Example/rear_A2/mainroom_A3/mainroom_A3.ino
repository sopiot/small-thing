//mainroom_arduino3

#include <XBee.h>

#include <thing_client.h>

//Library and definition for DHT11(Humidity & Temperature)
#include <dht11.h>
dht11 DHT11;

//Sensor in/out pins
#define DHT11PIN 23 // humidity & temperature sensor digital pin
#define measurePin 10// dust sensor analog pin 
#define ledPower 22 // dust sensor digital pin  
#define soundSensorPin 13 // sound sensor analog pin

//Some additional definitions for dust sensor
#define samplingTime 280
#define deltaTime 40
#define sleepTime 9680

//Some additional definitions for amp(MAX9814)
#define sampleWindow 25 // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;


//Humidity & Tempearture Sensor Functions
double HumiditySensor()
{
  DHT11.read(DHT11PIN);
  return ((double)DHT11.humidity);
}

double TemperatureSensor()
{
  DHT11.read(DHT11PIN);
  return ((double)DHT11.temperature);
}

//Dust Sensor Funtion

double DustSensor()
{
  float voMeasured = 0;
  float calcVoltage = 0;
  float dustDensity = 0;

  digitalWrite(ledPower, LOW); // power on the LED
  delayMicroseconds(samplingTime);

  voMeasured = analogRead(measurePin); // read the dust value

  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH); // turn the LED off
  delayMicroseconds(sleepTime);

  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (5.0 / 1024.0);

  // linear equation taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  dustDensity = 0.17 * calcVoltage - 0.1;// mg/m^3(milligram)

  return (double)(dustDensity*1000);// ug/m^3(microgram)
  
}

//Sound Sensor(microphone amplifier) Function
double SoundSensor(){
  unsigned long startMillis= millis();  // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
     sample = analogRead(soundSensorPin);
      
     if (sample < 1024)  // toss out spurious readings
     {
        if (sample > signalMax)
        {
           signalMax = sample;  // save just the max levels
        }
        else if (sample < signalMin)
        {
           signalMin = sample;  // save just the min levels
        }
     }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  return (double)(peakToPeak * 3.3) / 8.0;  // convert into voltage * 128
}

//Create a client & Values
ThingClient client_A3("Client_A3_Mainroom", Serial1);

CapitalValue humidityValue("Humidity_Mainroom", HumiditySensor, 0.0, 100.0, 3000);
CapitalValue temperatureValue("Temperature_Mainroom", TemperatureSensor, 0.0, 100.0, 3000);
CapitalValue dustValue("Dust_Mainroom", DustSensor, 0.0, 1000.0, 3000);
CapitalValue soundValue("Sound_Mainroom", SoundSensor, 0.0, 450.0, 2000);// original maximum value is 422.4

void setup() {
  // put your setup code here, to run once:
  
  client_A3.Add(humidityValue);
  client_A3.Add(temperatureValue);
  client_A3.Add(dustValue);
  client_A3.Add(soundValue);
  
  Serial1.begin(9600);
  pinMode(ledPower, OUTPUT);  
  client_A3.Setting();
}

void loop() {
  // put your main code here, to run repeatedly:
  client_A3.DoLoop();
}
