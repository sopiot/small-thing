#ifndef SMALL_THING_THING_H_
#define SMALL_THING_THING_H_

#include "attribute.h"
#include "common.h"
#include "function.h"
#include "value.h"

class Thing {
 public:
  // Constructor.
  // Developer can easily set class name and zigbee serial stream
  Thing();
  Thing(const char* class_name, int alive_cycle, Stream& serial);
  Thing(const char* class_name, Stream& serial);

  // Destrcutor
  ~Thing();

  // Attach stuffs to Thing.
  // For convenience, can be overrided with Value, Function, Attribute
  void Add(Value& v);
  void Add(Function& f);
  void Add(Attribute& a);

  // Setup
  // Call it after setting up Serial, client config, Functions, Values
  // Do not forget call this function after (zigbee) (ex. Serial.begin(9600))
  // This should be called in the setup() on Arduino
  void Setup();

  // Loop
  // Execute the configured Thing.
  // This should be called in the loop() on Arduino
  void Loop(int pub_period = 100);

 protected:
  /*
  virtual void subscribeByName(const uint8_t flags, const char* topic_name) = 0;
  virtual	void unsubscribeByName(const uint8_t flags, const char*
  topic_name) = 0;


  virtual void willtopicreqHandler(const message_header* msg) = 0;
  virtual void willmsgreqHandler(const message_header* msg) = 0;

  virtual void registerHandler(const msg_register* msg) = 0;
  virtual void pubackHandler(const msg_puback* msg) = 0;

*/

#ifdef USE_QOS2
  void pubrecHandler(const msg_pubqos2* msg);
  void pubrelHandler(const msg_pubqos2* msg);
  void pubcompHandler(const msg_pubqos2* msg);
#endif

  /*
  virtual void unsubackHandler(const msg_unsuback* msg) = 0; // it is needed,
  but will not be used in this library

  virtual void willtopicrespHandler(const msg_willtopicresp* msg) = 0;
  virtual void willmsgrespHandler(const msg_willmsgresp* msg) = 0;
*/
  void pingreqHandler();
  void pingrespHandler();
  void subackHandler(const msg_suback* msg);
  void disconnectHandler(const msg_disconnect* msg);
  void regack(const uint16_t topicId, const uint16_t messageId,
              const return_code_t return_code);
  void puback(const uint16_t topicId, const uint16_t messageId,
              const return_code_t return_code);
  void subscribe(const uint8_t flags, const uint16_t topicId);
  void subscribe(const uint8_t flags, const char* name);
  void unsubscribe(const uint8_t flags, const uint16_t topicId);
  void unsubscribe(const uint8_t flags, const char* name);
  void regackHandler(const msg_regack* msg);
  void publishHandler(const msg_publish* msg);
  void advertiseHandler(const msg_advertise* msg);
  void gwinfoHandler(const msg_gwinfo* msg);
  void connackHandler(const msg_connack* msg);
  void devregHandler();
  void devregackHandler(const msg_devregack* msg);

  // for debug
  void print_message_buffer_();
  void print_message_buffer_(void* buf, int length);
  void print_message_buffer_(int start, int length);

 private:
  // Set serial that connected to the zigbee output and input
  void SetSerial(Stream& serial);

  // Initialize client id with class name and mac address
  void GenerateClientId();

  // Get mac address of zigbee for client id
  void GetMacAddress();

  // Set class name of client
  void SetClassName(char* class_name);

  // Send Alive message to middleware for every alive cycle
  void SendAliveMessage();
  void sendAliveMessageNoCond();

  // Send Initial Value when register thing
  void SendInitialValueNoCond();

#ifdef USE_QOS2
  void pubrec(const msg_publish* msg);
  void pubrel(const msg_pubqos2* msg);
  void pubcomp(const msg_pubqos2* msg);
#endif

  bool waitForResponse();
  bool connected();
  bool valid();

  void searchgw(const uint8_t radius);
  void connect(const uint8_t flags, const uint16_t duration,
               const char* client_id_);
  bool registerTopic(const char* name);
  void publish(const uint8_t flags, const uint16_t topicId, const void* data,
               const uint8_t data_len);
  void devreg(void);

  void ReadZbeeTimeout(int timeout);
  void ReadZbeeIfAvailable();

  void pingreq();
  void pingresp(int flag);
  void disconnect(const uint16_t duration);

  void ParseStream(char* buf, uint16_t len);
  void dispatch();

  void unicast();
  void broadcast();
  void sendPacket();

  // f~ : function pointers
  bool compareTimeStamp(Value* t);

  void (*connect_handler_)();
  void (*disconnect_handler_)();

  // data members
  uint8_t num_values_;
  uint8_t num_functions_;
  uint8_t num_attributes_;

  Value* values_[MAX_VALUE_NUM];
  Function* functions_[MAX_FUNCTION_NUM];
  Attribute* attributes_[MAX_ATTRIBUTE_NUM];

  uint16_t id_1001_;
  uint16_t id_1002_;
  uint16_t id_2001_;
  uint16_t id_2002_;
  uint16_t id_2003_;

  char buffer[MAX_BUFFER_SIZE];
  char save_buffer[MAX_BUFFER_SIZE];
  uint8_t mac_address_[8];

  char* client_id_;
  char* class_name_;
  unsigned long alive_cycle_;

  /////// MQTT-SN
  XBee zbee_;

  bool connected_;
  XBeeAddress64 gateway_address_64_;
  uint16_t gateway_address_16_;
  uint8_t gateway_id_;
  ZBTxRequest zbee_tx_;
  ZBRxResponse zbee_rx_;
  // Set to true when we're waiting for some sort of acknowledgement from the
  // server that will transition our state.
  bool waiting_for_response_;
  bool valid_;
  bool registered_;
  bool device_register_;
  bool in_process_;  // for handling publish processing

  uint8_t response_wait_for_;

  uint16_t message_id_;

  uint8_t message_buffer_[MAX_BUFFER_SIZE];

  uint32_t response_timer_;
  uint8_t response_retries_;

  uint16_t registered_id_;

  uint16_t last_ping_;
};

#endif  // SMALL_THING_THING_H_
