#include "rf_staff_thing.h"

RFStaffThing::RFStaffThing() {}

RFStaffThing::RFStaffThing(int CE, int CSN) {
  _ce_pin = CE;
  _csn_pin = CSN;
  _radio = RF24(_ce_pin, _csn_pin);
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
  alive_cycle = 10;
  value_cycle = 10;
  generate_random_device_id();
}

void RFStaffThing::SetupRFModule() {
  Serial.begin(115200);

  // for wait user serial read...
  // while (!Serial) {
  // }

  if (!_radio.begin()) {
    SOPLOGLN(F("radio hardware is not responding!!"));
    while (1) {
    }
  }

  _radio.powerDown();
  delay(1000);
  _radio.powerUp();

  _radio.setPALevel(RF24_PA_MAX);
  _radio.setChannel(76);
  _radio.enableDynamicPayloads();
  _radio.enableAckPayload();

  _radio.openWritingPipe(tx_address);
  _radio.openReadingPipe(1, rx_address);
  _radio.stopListening();

  SOPLOGLN(F("SetupRFModule complete!!!"));
}

void RFStaffThing::SensorValueUpdate() {
  int sensor_value = analogRead(A0);
  sprintf(value_payload, "%d", sensor_value);
}

void RFStaffThing::SendMessage(char *msg) {
  _radio.stopListening();

  SOPLOG(F("send "));
  for (int i = 0; i < SOPLOG_LIMIT; i++) {
    if (msg[i] < 32) {
      msg[i] = ' ';
    }
    SOPLOG(F("%c"), msg[i]);
  }
  SOPLOGLN(F("..."));

  _radio.write(msg, strlen(msg));
  _radio.printPrettyDetails();
}

void RFStaffThing::ReadRFPayload(int timeout) {
  _radio.startListening();
  SOPLOGLN(F("startListening"));
  long long current_time = millis();
  while (millis() - current_time < timeout) {
    if (_radio.available()) {
      int length = _radio.getDynamicPayloadSize();
      memset(received_message, 0, SOPLOG_LIMIT);

      if (length > 31) {
        Serial.print(F("too many byte"));
        break;
      }

      _radio.read(&received_message, 32);
      // received_message[length] = '\0';

      SOPLOGLN(F("Recieved %d bytes : %s"), length, received_message);

      Handle_recv_msg(received_message);
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
    SOP_DEBUG(F("Register success!!! assigned device_id: %s"), device_id);
    Send_LIVE();
    Send_VAL();
  }
}

void RFStaffThing::Handle_EXEC(char *msg) {}

void RFStaffThing::Send_REG() {
  if (!registered) {
    memset(send_message, 0, SOPLOG_LIMIT);
    char cycle_info[16];

    SOPLOGLN(F("[Send_REG] broadcasting REG..."));

    memcpy(send_message, "REG ", 4);
    memcpy(send_message + 4, device_id, 4);
    memcpy(send_message + 8, value_name, 8);
    sprintf(cycle_info, "A%dV%d", alive_cycle, value_cycle);
    strcpy(send_message + 16, cycle_info);
    SendMessage(send_message);
  }
}
void RFStaffThing::Send_VAL() {
  if (registered) {
    long long current_time = millis();
    if (current_time - last_value_update_time > value_cycle) {
      SOP_DEBUG(F("[Send_VAL] Send VAL..."));
      SensorValueUpdate();
      sprintf(send_message, "VAL %s", device_id);
      memcpy(send_message + 8, value_name, 8);
      memcpy(send_message + 16, value_payload, 16);
      SendMessage(send_message);
      last_value_update_time = current_time;
    }
  }
}

void RFStaffThing::Send_EACK(char *function_name, char *result_payload) {
  if (registered) {
  }
}

void RFStaffThing::Send_LIVE() {
  if (registered) {
    long long current_time = millis();
    if (current_time - last_alive_time > value_cycle) {
      SOP_DEBUG(F("[Send_LIVE] Send LIVE..."));
      SensorValueUpdate();
      sprintf(send_message, "LIVE%s", device_id);
      SendMessage(send_message);
      last_alive_time = current_time;
    }
  }
}

void RFStaffThing::generate_random_device_id() {
  for (int i = 0; i < sizeof(device_id); i++) {
    randomSeed(analogRead(0));
    device_id[i] = "0123456789ABCDEF"[random(0, 16)];
    delay(10);
  }
  SOP_DEBUG(F("[generate_random_device_id] gen device_id: %s..."), device_id);
}

void RFStaffThing::Loop() {
  Send_REG();
  Send_VAL();
  Send_LIVE();
  ReadRFPayload(5000);
}