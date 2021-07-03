#ifndef SMALL_THING_THING_H_
#define SMALL_THING_THING_H_

#include "common.h"
#include "function.h"
#include "utils.h"
#include "value.h"

#define MAC_ADDRESS_SIZE 16
#define READ_ZBEE_TIMEOUT 1000
#define ESCAPE_ZBEE_TIMEOUT 50
#define SEARCH_RADIUS 100

class Thing {
 public:
  // Constructor.
  // Developer can easily set class name and zigbee serial stream
  Thing();
  Thing(const char* class_name, int alive_cycle, Stream& serial);
  Thing(const char* class_name, Stream& serial);

  // Destrcutor
  ~Thing();

  const char* ClientId();

  // Attach stuffs to Thing.
  // For convenience, can be overrided with Value, Function, Attribute
  void Add(Value& v);
  void Add(Function& f);

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
  void PubrecHandler(const msg_pubqos2* msg);
  void PubrelHandler(const msg_pubqos2* msg);
  void PubcompHandler(const msg_pubqos2* msg);
#endif

  /*
  virtual void unsubackHandler(const msg_unsuback* msg) = 0; // it is needed,
  but will not be used in this library

  virtual void willtopicrespHandler(const msg_willtopicresp* msg) = 0;
  virtual void willmsgrespHandler(const msg_willmsgresp* msg) = 0;
  */

  void PingreqHandler();
  void PingrespHandler();
  void SubackHandler(const msg_suback* msg);
  void DisconnectHandler(const msg_disconnect* msg);
  void Regack(const uint16_t topicId, const uint16_t messageId,
              const return_code_t return_code);
  void Puback(const uint16_t topicId, const uint16_t messageId,
              const return_code_t return_code);
  void Subscribe(const uint8_t flags, const uint16_t topicId);
  void Subscribe(const uint8_t flags, const char* name);
  void Unsubscribe(const uint8_t flags, const uint16_t topicId);
  void Unsubscribe(const uint8_t flags, const char* name);
  void RegackHandler(const msg_regack* msg);
  void PublishHandler(const msg_publish* msg);
  void AdvertiseHandler(const msg_advertise* msg);
  void GwinfoHandler(const msg_gwinfo* msg);
  void ConnackHandler(const msg_connack* msg);

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
  void SendAliveMessageNoCond();

  // Send Initial Value when register thing
  void SendInitialValueNoCond();

#ifdef USE_QOS2
  void Pubrec(const msg_publish* msg);
  void Pubrel(const msg_pubqos2* msg);
  void Pubcomp(const msg_pubqos2* msg);
#endif

  bool GatewayConnected();
  bool GatewayReady();

  void Searchgw(const uint8_t radius);
  void Connect(const uint8_t flags, const uint16_t duration,
               const char* client_id_);
  bool RegisterTopic(const char* name);
  void Publish(const uint8_t flags, const uint16_t topicId, const void* data,
               const uint8_t data_len);
  void RegisterToMIddleware();

  void ReadZbeeTimeout(int timeout);
  void ReadZbeeIfAvailable();

  void Pingreq();
  void Pingresp(int flag);
  void Disconnect(const uint16_t duration);

  void ParseMQTTSNStream(char* buf, uint16_t len);

  void Unicast();
  void Broadcast();
  void SendPacket();

  // f~ : function pointers
  bool CompareTimeStamp(Value* t);

  void PrintTags();
  void PrintTopicID();
  void TestPublish();

  void (*connect_handler_)();
  void (*disconnect_handler_)();

  // data members
  uint8_t num_values_;
  uint8_t num_functions_;

  Value* values_[MAX_VALUE_NUM];
  Function* functions_[MAX_FUNCTION_NUM];

  uint16_t id_1001_;
  uint16_t id_1002_;
  uint16_t id_2001_;
  uint16_t id_2002_;
  uint16_t id_2003_;
  uint16_t id_2010_;
  uint16_t id_2011_;
  uint16_t id_2012_;
  uint16_t id_2013_;
  uint16_t id_2014_;
  uint16_t id_2015_;
  uint16_t id_2016_;

  char publish_buffer[MAX_BUFFER_SIZE];
  char receive_buffer[MAX_BUFFER_SIZE];
  uint8_t mac_address_[8];

  char* client_id_;
  char* class_name_;
  unsigned long alive_cycle_;

  XBee zbee_;
  XBeeAddress64 gateway_address_64_;
  uint16_t gateway_address_16_;
  uint8_t gateway_id_;
  ZBTxRequest zbee_tx_;
  ZBRxResponse zbee_rx_;

  /** Set to true when we're waiting for some sort of acknowledgement from the
   *server that will transition our state.
   */

  bool gateway_connected_;
  bool gateway_response_wait_;
  bool gateway_ready_;
  bool middleware_registered_;

  uint8_t protocal_response_wait_;
  uint16_t message_id_;
  uint8_t message_buffer_[MAX_BUFFER_SIZE];
  uint16_t registered_id_;
  uint16_t last_ping_;
};

#endif  // SMALL_THING_THING_H_
