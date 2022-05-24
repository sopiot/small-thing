#include "rf_staff_thing.h"

RFStaffThing::RFStaffThing() {}

RFStaffThing::RFStaffThing(int CE, int CSN) {
  _ce_pin = CE;
  _csn_pin = CSN;
  _radio = RF24(_ce_pin, _csn_pin, 4000000);
}

RFStaffThing::RFStaffThing(int CE, int CSN, uint64_t rx_address,
                           uint64_t tx_address) {
  _ce_pin = CE;
  _csn_pin = CSN;
  _radio = RF24(_ce_pin, _csn_pin);
  this->rx_address = rx_address;
  this->tx_address = tx_address;
}

RFStaffThing::~RFStaffThing() {}

void RFStaffThing::SetupSensor() {
  strcpy(value_name, "TestVal");
  SOPLOGLN(F("value_name: %s"), value_name);
  alive_cycle = 1;
  value_cycle = 1;
  generate_random_device_id();

  // pinMode(LED_BUILTIN, OUTPUT);
  // digitalWrite(LED_BUILTIN, HIGH);

  // digitalWrite(LED_PWR, LOW);
}

void RFStaffThing::SetupRFModule() {
#ifdef SOP_DEBUG
  Serial.begin(9600);

  // for wait user serial read...
  // while (!Serial) {
  // }
#endif

  if (!_radio.begin()) {
    SOPLOGLN(F("radio hardware is not responding!!"));
    while (1) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
    }
  }

  // _radio.powerDown();
  // delay(1000);
  // _radio.powerUp();

  // _radio.setChannel(76);
  _radio.enableDynamicPayloads();
  _radio.enableAckPayload();
  _radio.setPALevel(RF24_PA_MAX);

  _radio.openWritingPipe(tx_address);
  _radio.openReadingPipe(1, rx_address);
  _radio.stopListening();

  SOPLOGLN(F("SetupRFModule complete!!!"));
}

void RFStaffThing::A0SensorValueUpdate() {
  int sensor_value = analogRead(A0);
  sprintf(value_payload, "%d", sensor_value);
}

void RFStaffThing::D2SensorValueUpdate() {
  int sensor_value = digitalRead(2);
  sprintf(value_payload, "%d", sensor_value);
}

bool RFStaffThing::SendMessage(char *msg) {
  _radio.stopListening();

  SOPLOG(F("send "));
  for (int i = 0; i < SOPRF_LIMIT; i++) {
    if (msg[i] < 32) {
      msg[i] = ' ';
    }
    SOPLOG(F("%c"), msg[i]);
  }
  SOPLOGLN(F("..."));

  bool report = _radio.write(msg, strlen(msg));
  // _radio.writeAckPayload(1, msg, strlen(msg));
  // _radio.printPrettyDetails();
  // _radio.flush_rx();
  // _radio.flush_tx();
  _radio.startListening();
  return report;
}

void RFStaffThing::ReadRFPayload(int timeout) {
  _radio.startListening();
  SOPLOGLN(F("startListening"));
  long long current_time = millis();
  while (millis() - current_time < timeout) {
    if (_radio.available()) {
      int length = _radio.getDynamicPayloadSize();
      char tmp_recv[SOPRF_LIMIT] = {0};
      // memset(tmp, 0, SOPRF_LIMIT);

      if (length > 31) {
        Serial.print(F("too many byte"));
        break;
      }

      _radio.read(&tmp_recv, 32);
      // received_message[length] = '\0';

      SOPLOGLN(F("Recieved %d bytes : %s"), length, tmp_recv);

      Handle_recv_msg(tmp_recv);
      return;
    }
  }
  SOPLOGLN(F("endListening"));
}

void RFStaffThing::Handle_recv_msg(char *msg) {
  if (strncmp(msg, "RACK", 4) == 0) {
    Handle_RACK(msg);
  } else if (strncmp(msg, "EXEC", 4) == 0) {
    Handle_EXEC(msg);
  } else {
    SOPLOGLN(F("unknown message %s"), msg);
    return;
  }
}

void RFStaffThing::Handle_RACK(char *msg) {
  char target_device_id[5];
  strncpy(target_device_id, msg + 4, 4);
  if (strncmp(target_device_id, device_id, 4) != 0) {
    SOPLOGLN(F("not my RACK message... this msg is for %s device"),
             target_device_id);
    return;
  } else {
    strncpy(device_id, msg + 8, 4);
    registered = true;
    SOPLOGLN(F("Register success!!! assigned device_id: %s"), device_id);
    Send_LIVE();
    Send_VAL();
  }
}

void RFStaffThing::Handle_EXEC(char *msg) {}

bool RFStaffThing::Send_REG() {
  if (!registered) {
    memset(send_message, 0, SOPRF_LIMIT);
    char cycle_info[16];

    SOPLOGLN(F("[Send_REG] broadcasting REG..."));

    memcpy(send_message, "REG ", 4);
    SOPLOGLN(F("send_message: %s"), send_message);
    memcpy(send_message + 4, device_id, 4);
    SOPLOGLN(F("send_message: %s"), send_message);
    memcpy(send_message + 8, send_message, 8);
    SOPLOGLN(F("send_message: %s"), send_message);
    sprintf(cycle_info, "A%dV%d", alive_cycle, value_cycle);
    SOPLOGLN(F("cycle_info: %s"), cycle_info);
    strcpy(send_message + 16, cycle_info);
    SOPLOGLN(F("send_message: %s"), send_message);
    return SendMessage(send_message);
  }
}
void RFStaffThing::Send_VAL() {
  long long current_time = micros();
  // char float_str[8] = {0};
  if (registered) {
    if (current_time - last_value_update_time > value_cycle) {
      SOPLOGLN(F("[Send_VAL] Send VAL..."));
      // A0SensorValueUpdate();
      D2SensorValueUpdate();
      sprintf(send_message, "VAL %s", device_id);
      memcpy(send_message + 8, value_name, 8);
      memcpy(send_message + 16, value_payload, 16);
      long long current_time_for_rf = micros();
      SendMessage(send_message);
      int duration_for_rf = (micros() - current_time_for_rf);
      // safe_dtostrf(duration_for_rf, 4, 2, float_str);
      SOPLOGLN(F("[Send_VAL] duration_for_rf: %d"), duration_for_rf);
      last_value_update_time = current_time;
    }
  }
  // memset(float_str, 0, 8);
  int duration = (micros() - current_time);
  // safe_dtostrf(duration, 4, 2, float_str);
  SOPLOGLN(F("[Send_VAL] duration: %d"), duration);
}

void RFStaffThing::Send_EACK(char *function_name, char *result_payload) {
  if (registered) {
  }
}

void RFStaffThing::Send_LIVE() {
  if (registered) {
    long long current_time = millis();
    if (current_time - last_alive_time > value_cycle) {
      SOPLOGLN(F("[Send_LIVE] Send LIVE..."));
      // A0SensorValueUpdate();
      sprintf(send_message, "LIVE%s", device_id);
      SendMessage(send_message);
      last_alive_time = current_time;
    }
  }
}

void RFStaffThing::Loop() {
  bool result = Send_REG();
  if(result)
  {
    registered = true;
    SOPLOGLN(F("Register Sucesss!!!"));
  } else {
    registered = true;
    SOPLOGLN(F("Register Failed..."));    
  }
  Send_VAL();
  // Send_LIVE();

  if (registered && mode == SENSOR_ONLY) {
    DeviceSleep();
  } else {
    ReadRFPayload(5000);
  }
}

void RFStaffThing::generate_random_device_id() {
  for (int i = 0; i < sizeof(device_id); i++) {
    randomSeed(analogRead(0));
    device_id[i] = "0123456789ABCDEF"[random(0, 16)];
    delay(10);
  }
  SOPLOGLN(F("[generate_random_device_id] gen device_id: %s..."), device_id);
}

void RFStaffThing::DeviceSleep() {
  SOPLOGLN(F("[DeviceSleep]"));
  digitalWrite(LED_BUILTIN, LOW);
  _radio.powerDown();
  SoPSleep(alive_cycle * 1000);
  _radio.powerUp();
  digitalWrite(LED_BUILTIN, HIGH);
}
