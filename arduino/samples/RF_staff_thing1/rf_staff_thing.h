#ifndef RF_STAFF_THING_H_
#define RF_STAFF_THING_H_

#include <Arduino.h>
#include <stdarg.h>

#include "RF24.h"
#include "common.h"

// struct Service {
//   char service_name[8];
//   void *callback_function = nullptr;
// };

// struct ValueService : public Service {
//   char value_payload[SOPLOG_LIMIT / 2];
// };

// // TODO: implement this
// struct FunctionService : public Service {
//   char function_payload[SOPLOG_LIMIT / 2];
// };

class RFStaffThing {
 public:
  // pin 7 == CE pin
  // pin 8 == CSN pin

  bool registered = false;
  char received_message[SOPLOG_LIMIT];
  char send_message[SOPLOG_LIMIT];
  char device_id[4];
  uint64_t tx_address = 0xFFFFFFFFF1LL;
  uint64_t rx_address = 0xFFFFFFFFF0LL;
  int _ce_pin;
  int _csn_pin;

  char value_name[8];
  char value_payload[16];
  long long last_value_update_time = 0;
  int value_cycle = 10;

  long long last_alive_time = 0;
  int alive_cycle = 10;

  RF24 _radio;

  // RF24 _client;

  RFStaffThing();
  RFStaffThing(int CE, int CSN);
  RFStaffThing(int CE, int CSN, uint64_t rx_address, uint64_t tx_address);
  ~RFStaffThing();

  void SetupSensor();
  void SetupRFModule();
  void SensorValueUpdate();

  void SendMessage(char *msg);
  void ReadRFPayload(int timeout = 1000);

  void Handle_RACK(char *msg);
  void Handle_EXEC(char *msg);
  void Handle_recv_msg(char *msg);

  void Send_REG();
  void Send_VAL();
  void Send_EACK(char *function_name, char *result_payload);
  void Send_LIVE();

  void Loop();

  void generate_random_device_id();
};

#endif  // RF_STAFF_THING_H_
