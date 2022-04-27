const int kmovement1Pin = 2;

void SetupSerial() { Serial.begin(115200); }

void SetupModules() {
  // Setup Pin mode
  pinMode(kmovement1Pin, INPUT);

  // Attach modules
}

void setup() {
  SetupSerial();
  SetupModules();
}

void loop() {
  Serial.println(digitalRead(kmovement1Pin));
  delay(100);
}
