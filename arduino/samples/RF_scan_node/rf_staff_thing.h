#ifndef RF_STAFF_THING_H_
#define RF_STAFF_THING_H_

// #include <SPI.h>

#include "Arduino.h"
#include "RF24.h"
// #include "printf.h"

class RFStaffThing {
 public:
  // pin 7 == CE pin
  // pin 8 == CSN pin
  bool role = false;
  char received_message[32];
  char send_message[32];
  char device_id[4];
  uint64_t tx_address = 0xFFFFFFFFF1LL;
  uint64_t rx_address = 0xFFFFFFFFF0LL;
  int _ce_pin;
  int _csn_pin;

  RF24 _client;

  // RF24 _client;

  RFStaffThing();
  RFStaffThing(int CE, int CSN);
  RFStaffThing(int CE, int CSN, uint64_t rx_address, uint64_t tx_address);
  ~RFStaffThing();

  void SetupSensor();
  void SetupRFModule();
  char* Send_payload_with_ack(char* sent_message, int timeout);
  bool Register();
  void Run();
};

#endif  // SMALL_THING_FUNCTION_H_
