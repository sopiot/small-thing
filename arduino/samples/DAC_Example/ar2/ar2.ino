#include <XBee.h>
#include <thing_client.h>
#include <SoftwareSerial.h>

#define SOFTSERIAL_RX_PIN  10
#define SOFTSERIAL_TX_PIN  11
#define pressurePin A8

//Sensor in/out pins
#define lightPin A10

char *voiceBuffer[] =
{
    "No sound",
    "Turn on the light",
    "Turn off the light",
    "Play music",
    "Pause",
    "Next",
    "Previous",
    "Up",
    "Down",
    "Turn on the TV",
    "Turn off the TV",
    "Increase temperature",
    "Decrease temperature",
    "What's the time",
    "Open the door",
    "Close the door",
    "Left",
    "Right",
    "Stop",
    "Start",
    "Mode 1",
    "Mode 2",
    "Go",
};

SoftwareSerial softSerial(SOFTSERIAL_RX_PIN,SOFTSERIAL_TX_PIN);

char *VoiceRecognizeSensor(char *pszInput, int nLength)
{
  static char cmd = 0;
  if(softSerial.available())
  {
     cmd = (int)softSerial.read();
  }
  strcpy(pszInput, voiceBuffer[cmd]);

  return pszInput;
}

int LightSensor()
{
  return analogRead(lightPin);
}

int PressureSensor()
{
  return (int)analogRead(pressurePin);
}


//Create a client
ThingClient client_A3("Arduino", 60, Serial);

//Create values
Value pressureValue("pressure", PressureSensor, 0, 1024, 1000);
Value lightValue("light", LightSensor, 0, 2000, 1000);
Value voiceValue("voice", VoiceRecognizeSensor, 0, 50, 1000);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  softSerial.begin(9600);
  softSerial.listen();
   
  //add values to client
  client_A3.Add(pressureValue);
  client_A3.Add(lightValue);
  client_A3.Add(voiceValue);

  client_A3.Setting();
 }

void loop() {
  // put your main code here, to run repeatedly:
  client_A3.DoLoop();
}
