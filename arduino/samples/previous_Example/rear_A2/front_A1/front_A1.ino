#include <XBee.h>
#include <thing_client.h>

//front_arduino1

//Sensor in/out pins
#define lightSensorPin1 11 // light sensor analog pin for main room
#define lightSensorPin2 12 // light sensor analog pin for meeting room
#define motionSensorPin 52 // infrared motion sensor digital pin for the front door

//previous brightness values
int prevMainRoomLight = 0;
int prevMeetingRoomLight = 0;

//current brightness values
int currMainRoomLight = 0;
int currMeetingRoomLight = 0;

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

//Create a client
ThingClient client_A1("Client_A1_Frontdoor", Serial1);

//Create values
CapitalValue lightValue1("Light_Mainroom", LightSensorMainRoom, 0, 2000, 2000);
CapitalValue lightValue2("Light_Meetingroom", LightSensorMeetingRoom, 0, 2000, 2000);
CapitalValue lightDelta1("Light_Change_Mainroom", LightDeltaMainRoom, 0, 2000, 2000);
CapitalValue lightDelta2("Light_Change_Meetingroom", LightDeltaMeetingRoom, 0, 2000, 2000);
CapitalValue motionValue("Motion_Frontdoor", MotionSensor, 100);



void setup() {
// put your setup code here, to run once:

//add values to client
  client_A1.Add(lightValue1);
  client_A1.Add(lightValue2);
  client_A1.Add(lightDelta1);
  client_A1.Add(lightDelta2);
  client_A1.Add(motionValue);

  Serial1.begin(9600);
  
  client_A1.Setting();
    
}

void loop() {
  // put your main code here, to run repeatedly:
  client_A1.DoLoop();
}
