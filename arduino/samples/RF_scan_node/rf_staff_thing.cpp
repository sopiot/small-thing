#include "rf_staff_thing.h"

RFStaffThing::RFStaffThing() {}

RFStaffThing::RFStaffThing(int CE, int CSN) {
  _ce_pin = CE;
  _csn_pin = CSN;
  _client = RF24(_ce_pin, _csn_pin);
}

RFStaffThing::RFStaffThing(int CE, int CSN, uint64_t rx_address,
                           uint64_t tx_address) {
  _ce_pin = CE;
  _csn_pin = CSN;
  _client = RF24(_ce_pin, _csn_pin);
  this->rx_address = rx_address;
  this->tx_address = tx_address;
}

RFStaffThing::~RFStaffThing() {}

void RFStaffThing::SetupSensor() {
  bool registered = false;

  if (!registered) {
    registered = this->Register();
  }

  Serial.println(F("SetupSensor complete!!!"));
}

void RFStaffThing::SetupRFModule() {
  Serial.begin(115200);
  while (!Serial) {
  }

  if (!_client.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {
    }
  }

  _client.setPALevel(RF24_PA_LOW);
  _client.enableDynamicPayloads();

  _client.enableAckPayload();

  _client.openWritingPipe(tx_address);
  _client.openReadingPipe(1, rx_address);
  _client.stopListening();

  Serial.println(F("SetupRFModule complete!!!"));
}

char* RFStaffThing::Send_payload_with_ack(char* sent_message, int timeout) {
  _client.stopListening();
  unsigned long start_timer = micros();
  _client.write(sent_message, sizeof(sent_message));
  unsigned long end_timer = micros();

  char* node_ID = NULL;
  uint8_t pipe;

  _client.startListening();
  unsigned long listen_start_timer = millis();
  while (millis() - listen_start_timer < timeout) {
    if (_client.available()) {
      int length = _client.getDynamicPayloadSize();
      if (length > 31) {
        Serial.print(F("too many byte"));
        break;
      }
      // char* received_message = (char*)malloc(sizeof(char) * (length + 1));
      _client.read(&received_message, length);
      received_message[length] = '\0';

      Serial.print(F(" Recieved "));
      Serial.print(length);
      Serial.print(F(" bytes on pipe "));
      Serial.print(pipe);
      Serial.print(F(": "));
      Serial.print(received_message);

      if (strncmp(received_message, "RACK", 4) == 0) {
        Serial.println(F(" ack is come"));
        memcpy(device_id, received_message + 4, 4);
        _client.stopListening();
      } else {
        Serial.println(F(" Not ack"));
        delay(200);
      }
    }
  }

  return 0;
}

bool RFStaffThing::Register() {
  while (1) {
    Serial.println(F("send REG ..."));
    char* node_ID = Send_payload_with_ack("REG ", 5000);
    if (node_ID) {
      Serial.println(F("register complete!!!"));
      return true;
    }
  }
}

void RFStaffThing::Run() {
  Serial.println(F("Run executed"));
  _client.stopListening();
  unsigned long start_timer = micros();
  int sensor_value = analogRead(0);
  sprintf(send_message, "VAL %s%d", device_id, sensor_value);
  _client.write(send_message, sizeof(send_message));
  unsigned long end_timer = micros();
  delay(1000);
}