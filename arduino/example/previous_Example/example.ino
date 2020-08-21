#include <XBee.h>

#include <thing_client.h>

void myFunc(int t)
{
  Serial.println("Hello, World!");
  Serial.println(t);
}

int ReturnOne()
{
  return 1;
}

char client_name[] = "Timer";
char value_name[] = "onlyone";
char function_name[] = "HelloWorld";

ThingClient myclient(client_name, Serial1);

CapitalValue myvalue(value_name, ReturnOne, 0, 1);
CapitalFunction myfunction(function_name, myFunc, 0, 1);

void setup() {
  myclient.Add(myvalue);
  myclient.Add(myfunction);
  Serial.begin(9600);
  Serial1.begin(9600);  
  myclient.Setting();
  // put your setup code here, to run once:
  
}

void loop() {
  // put your main code here, to run repeatedly:
  myclient.DoLoop();
}
