#include <XBee.h>
#include <Ultrasonic.h>
#include <thing_client.h>

//front_arduino1

//Sensor in/out pins
#define lightSensorPin1 11 // light sensor analog pin for main room
#define lightSensorPin2 12 // light sensor analog pin for meeting room
#define motionSensorPin 52 // infrared motion sensor digital pin for the front door

#define ledPin 26

Ultrasonic ultrasonic(48, 46);

//previous brightness values
int prevMainRoomLight = 0;
int prevMeetingRoomLight = 0;

//current brightness values
int currMainRoomLight = 0;
int currMeetingRoomLight = 0;

int UltraSonicSensor()
{
	return (int) ultrasonic.Ranging(CM);
}

//Light Sensor for Main Room
int LightSensorMainRoom()
{
  prevMainRoomLight = currMainRoomLight;
  currMainRoomLight = analogRead(lightSensorPin1);
  return currMainRoomLight;
}

//Brightness Change of Main Room
int LightDeltaMainRoom(){
  int delta = currMainRoomLight - prevMainRoomLight;
  if(delta >= 0) return delta;
  else return (0 - delta);
}

//Light Sensor for Meeting Room
int LightSensorMeetingRoom()
{
  prevMeetingRoomLight = currMeetingRoomLight;
  currMeetingRoomLight = analogRead(lightSensorPin2);
  return currMeetingRoomLight;
}

//Brightness Change of Meeting Room
int LightDeltaMeetingRoom(){
  int delta = currMeetingRoomLight - prevMeetingRoomLight;
  if(delta >= 0) return delta;
  else return (0 - delta);
}

//Motion Sensor for Front Door
bool MotionSensor()
{
  if(digitalRead(motionSensorPin) == 1) return true;
  else return false;
}

void led(bool on)
{
	if(on == true)
	{
		digitalWrite(ledPin, HIGH);
	}
	else
	{
		digitalWrite(ledPin, LOW);
	}

}

long microsecondsToCentimeters(long microseconds) {
	// The speed of sound is 340 m/s or 29 microseconds per centimeter.
	// The ping travels out and back, so to find the distance of the
	// object we take half of the distance traveled.
	return microseconds / 29 / 2;
}

//Create a client
ThingClient client_A1("Arduino", 3, Serial1);

//Create values
CapitalValue motionValue("Motion_Frontdoor", MotionSensor, 100);
CapitalValue distanceValue("Distance_Frontdoor", UltraSonicSensor, 2, 4000, 100);

CapitalFunction ledFunction("led", led);

void setup() {
	pinMode(ledPin, OUTPUT);


// put your setup code here, to run once:
//add values to client
  Serial.begin(9600);
  Serial1.begin(9600);
  client_A1.Add(motionValue);
  client_A1.Add(distanceValue);
  client_A1.Add(ledFunction);
  client_A1.Setting();

}

void loop() {
  // put your main code here, to run repeatedly:
  client_A1.DoLoop();
}
