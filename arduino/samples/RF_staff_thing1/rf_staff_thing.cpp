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
  strncpy(value_name, "TestVal", sizeof(value_name));
  SOPLOGLN(F("value_name: %s"), value_name);

  alive_cycle = 100;
  value_cycle = 100;
  generate_random_device_id();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void RFStaffThing::SetupRFModule() {
#ifdef SOP_DEBUG
  Serial.begin(9600);

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

  if (!memset(value_payload, 0, 16)) {
    SOPLOGLN(F("memset failed!!!"));
  }

  int ret = snprintf(value_payload, 16 + 1, "%d", sensor_value);
  if (ret < 0) {
    SOPLOGLN(F("snprintf error!!!"));
  } else {
    return;
  }
}

void RFStaffThing::D2SensorValueUpdate() {
  // window length == 120 * 1 sec = 120 sec
#define WINDOW_SIZE 60 * 1
  static int window[WINDOW_SIZE] = {0};
  static int window_index = 0;

  int sensor_value = digitalRead(2);
  int threshold = 1;
  int move_stack = 0;
  int move_detect = 0;
  window[window_index++] = sensor_value;

  for (int i = 0; i < WINDOW_SIZE; i++) {
    if (window[i] == 1) {
      move_stack++;
    }
    if (move_stack > threshold) {
      move_detect = 1;
      break;
    } else {
      move_detect = 0;
    }
  }

  if (window_index >= WINDOW_SIZE) {
    window_index = 0;
  }

  if (!memset(value_payload, 0, 16)) {
    SOPLOGLN(F("memset failed!!!"));
  }

  int ret = snprintf(value_payload, 16 + 1, "%d", move_detect);
  if (ret < 0) {
    SOPLOGLN(F("snprintf error..."));
  } else {
    SOPLOGLN(F("movement detected!!!"));
    return;
  }
}

void RFStaffThing::TestValueUpdate() {
  static bool first_execute = false;
  static unsigned int sensor_value;

  if (!first_execute) {
    first_execute = true;
    sensor_value = 0;
  } else {
    sensor_value++;
  }

  if (!memset(value_payload, 0, 16)) {
    SOPLOGLN(F("memset failed!!!"));
  }

  int ret = snprintf(value_payload, 16 + 1, "%u", sensor_value);
  if (ret < 0) {
    SOPLOGLN(F("snprintf error!!!"));
  } else {
    return;
  }
}

// only send ascii string
bool RFStaffThing::SendMessage(char *msg) {
  _radio.stopListening();

  SOPLOG(F("[SendMessage] send "));
  for (int i = 0; i < SOPRF_LIMIT; i++) {
    if (msg[i] < 32) {
      msg[i] = ' ';
    } else {
      SOPLOG(F("%c"), msg[i]);
    }
  }
  SOPLOGLN(F("..."));

  bool report = _radio.write(msg, strlen(msg));

  // _radio.writeAckPayload(1, msg, strlen(msg));
  // _radio.printPrettyDetails();
  // _radio.flush_rx();
  // _radio.flush_tx();

  _radio.startListening();
  if (report) {
    SOPLOGLN(F("[SendMessage] send success!!!"));
    return true;
  } else {
    SOPLOGLN(F("[SendMessage] send failed!!!"));
    return false;
  }
}

void RFStaffThing::ReadRFPayload(int timeout) {
  _radio.startListening();
  SOPLOGLN(F("[ReadRFPayload] startListening"));
  long long current_time = millis();
  while (millis() - current_time < timeout) {
    if (_radio.available()) {
      int length = _radio.getDynamicPayloadSize();
      memset(received_message, 0, SOPRF_LIMIT);

      if (length > SOPRF_LIMIT - 1) {
        SOPLOGLN(F("[ReadRFPayload] too many byte"));
        break;
      } else {
        _radio.read(&received_message, SOPRF_LIMIT);

        SOPLOGLN(F("[ReadRFPayload] Recieved %d bytes : %s"), length,
                 received_message);
        // Recieved 12 bytes : RACKBD44 BD444444 44444444 44444444 REG
        // BD44TestVal A10000V10000

        Handle_recv_msg(received_message);
        return;
      }
    }
  }
  SOPLOGLN(F("[ReadRFPayload] endListening"));
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
  if (!strncpy(target_device_id, msg + 4, 4)) {
    SOPLOGLN(F("strncpy failed..."));
  }

  if (strncmp(target_device_id, device_id, 4) != 0) {
    SOPLOGLN(
        F("[Handle_RACK] not my RACK message... this msg is for %s device"),
        target_device_id);
    return;
  } else {
    if (strncpy(device_id, msg + 8, 4)) {
      registered = true;
      SOPLOGLN(F("[Handle_RACK] Register success!!! assigned device_id: %s"),
               device_id);
      // Send_LIVE();
      Send_VAL();
    } else {
      SOPLOGLN(F("strncpy failed..."));
    }
  }
}

void RFStaffThing::Handle_EXEC(char *msg) {}

void RFStaffThing::Send_REG() {
  bool result = false;
  if (!registered) {
    while (!result) {
      memset(send_message, 0, SOPRF_LIMIT);
      char cycle_info[16];

      memmove(send_message, "REG ", 4);
      memmove(send_message + 4, device_id, 4);
      memmove(send_message + 8, value_name, 8);
      snprintf(cycle_info, 16 + 1, "A%dV%d", alive_cycle, value_cycle);
      strncpy(send_message + 16, cycle_info, 16);
      SOPLOGLN(F("[Send_REG] broadcast %s"), send_message);

      result = SendMessage(send_message);
    }
  } else {
    SOPLOGLN(F("[Send_REG] already registered"));
  }
}
void RFStaffThing::Send_VAL() {
  long long current_time = micros();
  long long duration;
  // char float_str[8] = {0};
  if (registered) {
    if (current_time - last_value_update_time < 0) {
      SOPLOGLN(F("[Send_VAL] overflow occured!!!"));
      duration = __UINT32_MAX__ - last_value_update_time + current_time;
    } else {
      duration = current_time - last_value_update_time;
    }
    if (duration > value_cycle) {
      SOPLOGLN(F("[Send_VAL] Send VAL..."));

      // A0SensorValueUpdate();
      D2SensorValueUpdate();
      // TestValueUpdate();

      snprintf(send_message, 8 + 1, "VAL %s", device_id);
      memmove(send_message + 8, value_name, 8);
      memmove(send_message + 16, value_payload, 16);
      long long current_time_for_rf = micros();
      SendMessage(send_message);

      last_value_update_time = current_time;
    } else {
      return;
    }
  } else {
    SOPLOGLN(F("[Send_VAL] not registered..."));
  }
}

void RFStaffThing::Send_EACK(char *function_name, char *result_payload) {
  if (registered) {
  }
}

// void RFStaffThing::Send_LIVE() {
//   if (registered) {
//     long long current_time = millis();
//     if (current_time - last_alive_time > value_cycle) {
//       SOPLOGLN(F("[Send_LIVE] Send LIVE..."));
//       // A0SensorValueUpdate();
//       snprintf(send_message, 8, "LIVE%s", device_id);
//       SendMessage(send_message);
//       last_alive_time = current_time;
//     }
//   }
// }

void RFStaffThing::Loop() {
  Send_REG();
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
  SoPSleep(alive_cycle);
  _radio.powerUp();
  digitalWrite(LED_BUILTIN, HIGH);
}
