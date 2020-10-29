#include <XBee.h>
#include <thing_client.h>
//#include <IRremote.h>

//Sensor in/out pins
const int distanceTriggerPin = 22;
const int distanceEchoPin = 24;
const int motionSensorPin = 52;
const int ledPin = A10;

//IRsend irsend;      // IR pin # = 9 

int getDistance(long time)
{
  // Calculates the Distance in mm
  // ((time)*(Speed of sound))/ toward and backward of object) * 10

  int DistanceCalc; // Calculation variable
  DistanceCalc = ((time / 2.9) / 2); // Actual calculation in mm
  //DistanceCalc = time / 74 / 2; // Actual calculation in inches
  return DistanceCalc; // return calculated value
}

int DistanceSensor()
{
  long Duration = 0;
  digitalWrite(distanceTriggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(distanceTriggerPin, HIGH); // Trigger pin to HIGH
  delayMicroseconds(10); // 10us high
  digitalWrite(distanceTriggerPin, LOW); // Trigger pin to HIGH

  Duration = pulseIn(distanceEchoPin, HIGH); // Waits for the echo pin to get high
  // returns the Duration in microseconds

  int Distance_mm = getDistance(Duration); // Use function to calculate the distance
  return Distance_mm;
}

//Motion Sensor for Front Door
bool MotionSensor()
{
  if (digitalRead(motionSensorPin) == 1) return true;
  else return false;
}

void LEDFunction(void *pData)
{
  int power = -1, res;
  res = GetIntArgumentByName(pData, "power", &power);
  if(res == -1)
  {
    return;
  }

  analogWrite(ledPin, 255 * power);
}

//Create a client
ThingClient client_A1("Arduino", 60, Serial);

//Create values
Value distanceValue("distance", DistanceSensor, 0, 30000, 2000);
Value motionValue("motion", MotionSensor, 500);

//Create function
Function ledFunction("led", LEDFunction, 1, 1);

//Create argument
Argument argSwitch("power", 0, 1, INTEGER);

//Create FunctionAttribute
Attribute ledFunction_EnergyAttribute("Energy[J]",50, DOUBLE);

void setup() {
    // put your setup code here, to run once:  
       
    Serial.begin(9600);         
    delay(1000);    
    Serial.println("Start");

    ledFunction.Add_argument(argSwitch);    
    ledFunction.Add_functionattribute(ledFunction_EnergyAttribute);    

    //add values to client
    client_A1.Add(distanceValue);
    client_A1.Add(motionValue);

    //add function to client
    client_A1.Add(ledFunction);
    client_A1.Setting();
}

void loop() {
  // put your main code here, to run repeatedly:
  client_A1.DoLoop();
}
