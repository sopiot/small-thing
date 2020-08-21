#include <XBee.h>
#include <thing_client.h>

//rear_a2

//Library and definition for DHT11(Humidity & Temperature)
#include <dht11.h>
dht11 DHT11;

//Sensor in/out pins
#define DHT11PIN 23 // humidity & temperature sensor digital pin
#define motionSensorPin 52 //infrared motion sensor digital pin for the rear door

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

//Motion Sensor for Rear Door
bool MotionSensor()
{
  if(digitalRead(motionSensorPin) == 1) return true;
  else return false;
}

//Create a client
ThingClient client_A2("Client_A2_Reardoor", Serial1);

//Create values
CapitalValue humidityValue("Humidity_Reardoor", HumiditySensor, 0.0, 100.0);
CapitalValue temperatureValue("Temperature_Reardoor", TemperatureSensor, 0.0, 100.0);
CapitalValue motionValue("Motion_Reardoor", MotionSensor);

void setup() {
// put your setup code here, to run once:

//add values to client_A2
  client_A2.Add(humidityValue);
  client_A2.Add(temperatureValue);
  client_A2.Add(motionValue);
    
  Serial1.begin(9600);
  Serial.begin(9600);
  
  client_A2.Setting();
   
}

void loop() {
  // put your main code here, to run repeatedly:
  client_A2.DoLoop(100);
}
