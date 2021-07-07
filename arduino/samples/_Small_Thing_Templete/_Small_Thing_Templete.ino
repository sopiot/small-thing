#include <thing.h>

int SenseIntValue() { return 23; }
double SenseDoubleValue() { return 12.3; }
bool SenseBoolValue() { return true; }
char* SenseStringValue() { return "string"; }

int ActuateIntFunction() { return 23; }
double ActuateDoubleFunction() { return 12.3; }
bool ActuateBoolFunction() { return true; }
char* ActuateStringFunction() { return "string"; }

int ActuateIntArgumentFunction(void* data) {
  int int_tmp;
  double double_tmp;

  GetIntArgumentByName(data, "int_argument", &int_tmp);
  GetDoubleArgumentByName(data, "double_argument", &double_tmp);

  return 23;
}
double ActuateDoubleArgumentFunction(void* data) {
  int int_tmp;
  double double_tmp;

  GetIntArgumentByName(data, "int_argument", &int_tmp);
  GetDoubleArgumentByName(data, "double_argument", &double_tmp);

  return double_tmp + int_tmp;
}
bool ActuateBoolArgumentFunction(void* data) {
  int int_tmp;
  double double_tmp;

  GetIntArgumentByName(data, "int_argument", &int_tmp);
  GetDoubleArgumentByName(data, "double_argument", &double_tmp);

  return true;
}
char* ActuateStringArgumentFunction(void* data) {
  int int_tmp;
  double double_tmp;
  char string_tmp[256];

  GetIntArgumentByName(data, "int_argument", &int_tmp);
  GetDoubleArgumentByName(data, "double_argument", &double_tmp);

  sprintf(string_tmp, "int_tmp : %d, double_tmp : %fl", int_tmp, double_tmp);

  return string_tmp;
}

Thing thing("Template", 60, SafeSerial);

Value int_value("int_value", SenseValveStatus, 0, 3, 3000);
Value double_value("double_value", SenseValveStatus, 0, 3, 3000);
Value bool_value("bool_value", SenseValveStatus, 0, 3, 3000);
Value string_value("string_value", SenseValveStatus, 0, 3, 3000);

Function int_function("int_function", ActuateIntFunction);
Function double_function("double_function", ActuateDoubleFunction);
Function bool_function("bool_function", ActuateBoolFunction);
Function string_function("string_function", ActuateStringFunction);

Function int_argument_function("int_argument_function",
                               ActuateIntArgumentFunction, 1);
Function double_argument_function("double_argument_function",
                                  ActuateDoubleArgumentFunction, 1);
Function bool_argument_function("bool_argument_function",
                                ActuateBoolArgumentFunction, 1);
Function string_argument_function("string_argument_function",
                                  ActuateStringArgumentFunction, 1);

Argument int_argument("int_argument", 0, 100, INTEGER);
Argument double_argument("double_argument", 0, 100.0, DOUBLE);

Tag tag1("tag1");
Tag tag2("tag2");

void SetupSerial() { SafeSerial.begin(9600); }

void SetupModules() {}

void SetupThing() {
  int_value.AddTag(tag1);
  int_value.AddTag(tag2);
  double_value.AddTag(tag1);
  double_value.AddTag(tag2);
  bool_value.AddTag(tag1);
  bool_value.AddTag(tag2);
  string_value.AddTag(tag1);
  string_value.AddTag(tag2);
  thing.Add(int_value);
  thing.Add(double_value);
  thing.Add(bool_value);
  thing.Add(string_value);

  int_function.AddTag(tag1);
  int_function.AddTag(tag2);
  double_function.AddTag(tag1);
  double_function.AddTag(tag2);
  bool_function.AddTag(tag1);
  bool_function.AddTag(tag2);
  string_function.AddTag(tag1);
  string_function.AddTag(tag2);
  thing.Add(int_function);
  thing.Add(double_function);
  thing.Add(bool_function);
  thing.Add(string_function);

  int_argument_function.AddArgument(int_argument);
  int_argument_function.AddArgument(double_argument);
  double_argument_function.AddArgument(int_argument);
  double_argument_function.AddArgument(double_argument);
  bool_argument_function.AddArgument(int_argument);
  bool_argument_function.AddArgument(double_argument);
  string_argument_function.AddArgument(int_argument);
  string_argument_function.AddArgument(double_argument);

  int_argument_function.AddTag(tag1);
  int_argument_function.AddTag(tag2);
  double_argument_function.AddTag(tag1);
  double_argument_function.AddTag(tag2);
  bool_argument_function.AddTag(tag1);
  bool_argument_function.AddTag(tag2);
  string_argument_function.AddTag(tag1);
  string_argument_function.AddTag(tag2);
  thing.Add(int_argument_function);
  thing.Add(double_argument_function);
  thing.Add(bool_argument_function);
  thing.Add(string_argument_function);

  thing.Setup();
}

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { thing.Loop(); }