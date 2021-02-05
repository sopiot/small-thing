#include "thing.h"

#include "utils.h"
#define MAC_ADDRESS_SIZE 16
#define READ_ZBEE_TIMEOUT 1000
#define SEARCH_RADIUS 100

static inline uint16_t bswap(const uint16_t val) {
  return (val << 8) | (val >> 8);
}

Thing::Thing()
    : waiting_for_response_(true),
      response_wait_for_(ADVERTISE),
      connected_(false),
      message_id_(0),
      gateway_id_(0),
      valid_(0),
      response_retries_(0),
      registered_(0),
      registered_id_(-1),
      zbee_(),
      in_process_(false) {
  num_values_ = 0;
  num_functions_ = 0;
}

// Default alive_cycle is 60
Thing::Thing(const char *class_name, Stream &serial) {
  Thing(class_name, 60, serial);
}

Thing::Thing(const char *class_name, int alive_cycle, Stream &serial)
    : waiting_for_response_(true),
      response_wait_for_(ADVERTISE),
      connected_(false),
      message_id_(0),
      gateway_id_(0),
      valid_(0),
      response_retries_(0),
      registered_(0),
      registered_id_(-1),
      zbee_() {
  zbee_.setSerial(serial);
  this->class_name_ = strdup(class_name);
  MEM_ALLOC_CHECK(class_name_);

  this->alive_cycle_ = alive_cycle;
  response_timer_ = millis();
  memset(message_buffer_, 0, MAX_BUFFER_SIZE);

  num_values_ = 0;
  num_functions_ = 0;
}

Thing::~Thing() {
  free(client_id_);
  free(class_name_);
}

void Thing::Add(Value &v) {
  if (num_values_ < MAX_VALUE_NUM) {
    values_[num_values_++] = &v;
  } else {
    SOPLOGLN(F("It cannot be added (more than limit)\n"));
  }
}

void Thing::Add(Function &f) {
  if (num_functions_ < MAX_FUNCTION_NUM) {
    functions_[num_functions_++] = &f;
  } else {
    SOPLOGLN(F("It cannot be added (more than limit)\n"));
  }
}

void Thing::Add(Attribute &a) {
  if (num_attributes_ < MAX_ATTRIBUTE_NUM) {
    attributes_[num_attributes_++] = &a;
  } else {
    SOPLOGLN(F("It cannot be added (more than limit)\n"));
  }
}

void Thing::Setup() {
  GenerateClientId();

  SOPLOGLN(F("Setup SEARCHGW"));
  ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
  while (!valid()) {
    searchgw(SEARCH_RADIUS);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
  }

  // CONNECT, CONNACK
  SOPLOGLN(F("Setup CONNECT"));
  while (!connected()) {
    snprintf(buffer, MAX_BUFFER_SIZE, "%s", client_id_);
    connect(0, 50, buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
  }

  // REGISTER, REGACK
  SOPLOGLN(F("Setup REGISTER Topic MT1001"));
  do {
    snprintf(buffer, MAX_BUFFER_SIZE, MT1001, client_id_);  // id_1001_
    registerTopic(buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
  } while ((id_1001_ = registered_id_) == (UINT16_MAX));

  SOPLOGLN(F("Setup SUBSCRIBE Topic MT1001"));
  subscribe(QOS_FLAG, buffer);
  ReadZbeeTimeout(READ_ZBEE_TIMEOUT);

  registered_id_ = UINT16_MAX;
  SOPLOGLN(F("Setup REGISTER Topic MT1002"));
  do {
    snprintf(buffer, MAX_BUFFER_SIZE, MT1002, client_id_);  // id1105
    registerTopic(buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
  } while ((id_1002_ = registered_id_) == (UINT16_MAX));

  SOPLOGLN(F("Setup SUBSCRIBE Topic MT1002"));
  subscribe(QOS_FLAG, buffer);
  ReadZbeeTimeout(READ_ZBEE_TIMEOUT);

  registered_id_ = UINT16_MAX;
  SOPLOGLN(F("Setup REGISTER Topic TM2001"));
  do {
    snprintf(buffer, MAX_BUFFER_SIZE, TM2001, client_id_);  // id_2001_
    registerTopic(buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
  } while ((id_2001_ = registered_id_) == (UINT16_MAX));

  registered_id_ = UINT16_MAX;
  SOPLOGLN(F("Setup REGISTER Topic TM2002"));
  do {
    snprintf(buffer, MAX_BUFFER_SIZE, TM2002, client_id_);  // id_2002_
    registerTopic(buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
  } while ((id_2002_ = registered_id_) == (UINT16_MAX));

  registered_id_ = UINT16_MAX;
  SOPLOGLN(F("Setup REGISTER Topic TM2003"));
  do {
    snprintf(buffer, MAX_BUFFER_SIZE, TM2003, client_id_);  // id_2003_
    registerTopic(buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
  } while ((id_2003_ = registered_id_) == (UINT16_MAX));

  SOPLOGLN(F("Setup REGISTER Value Topics"));
  for (int i = 0; i < num_values_; i++) {
    registered_id_ = UINT16_MAX;
    do {
      snprintf(buffer, MAX_BUFFER_SIZE, COMMON0000, client_id_,
               values_[i]->name());
      registerTopic(buffer);

      SOPLOG(F("Value Topic: "));
      SOPLOGLN(buffer);

      ReadZbeeTimeout(READ_ZBEE_TIMEOUT);

    } while ((values_[i]->set_publish_id(registered_id_)) == (UINT16_MAX));
  }

  SOPLOGLN(F("Setup REGISTER Function Topics"));
  for (int i = 0; i < num_functions_; i++) {
    registered_id_ = UINT16_MAX;
    do {
      snprintf(buffer, MAX_BUFFER_SIZE, MT1003, functions_[i]->name(),
               client_id_);  // id1003
      registerTopic(buffer);
      ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    } while ((functions_[i]->set_id_1003(registered_id_)) == (UINT16_MAX));

    subscribe(QOS_FLAG, buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);

    // registered_id_ = UINT16_MAX;
    // memset(buffer, 0, MAX_BUFFER_SIZE);
    // do {
    //   snprintf(buffer, MAX_BUFFER_SIZE, TM2004_DEPRECATED,
    //            client_id_);  // id2004_deprecated
    //   registerTopic(buffer);
    //   ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    // } while ((functions_[i]->set_id_2004_deprecated(registered_id_)) ==
    //          (UINT16_MAX));

    registered_id_ = UINT16_MAX;
    do {
      snprintf(buffer, MAX_BUFFER_SIZE, TM2004, functions_[i]->name(),
               client_id_);  // id2004
      registerTopic(buffer);
      ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    } while ((functions_[i]->set_id_2004(registered_id_)) == (UINT16_MAX));

    SOPLOG(F("Function Topic: "));
    SOPLOGLN(buffer);
  }

  SOPLOGLN(F("[SUCCESS] Register & Subscribe Topics Finished."));

  devreg();
  sendAliveMessageNoCond();
  SOPLOGLN(F("[SUCCESS] Registeration to the Middleware Finished."));
}

void Thing::Loop(int pub_period) {
  SOPLOGLN(F("[LED DEBUG] Loop Start here!"));
  bool time_passed = false;
  bool changed = false;

  // Wait for Response about Value
  SendAliveMessage();
  // Wait for Response about Value
  ReadZbeeIfAvailable();
  for (uint8_t i = 0; i < num_values_; i++) {
    time_passed = compareTimeStamp(values_[i]);
    if (time_passed) {
      changed = values_[i]->capVal2str(buffer);
      // Value to Json String
      if (changed) {
        publish(QOS_FLAG, values_[i]->publish_id(), buffer, strlen(buffer));
      } else {
        SOPLOGLN(F("[DEBUG] Value is not changed. Not publishing value"));
      }
      // if (time_passed) {
      //   publish(QOS_FLAG, values_[i]->publish_id(), buffer, strlen(buffer));
      //   sendAliveMessageNoCond();
      // }
    } else {
      SOPLOGLN(F(
          "[DEBUG] Value publish cycle is not finished. Not publishing value"));
    }
    ReadZbeeIfAvailable();
  }
  SOPLOGLN(F("[INT DEBUG] Loop finished"));
  //delay(pub_period);
}

//----------------------------------------
// Private Functions
//----------------------------------------

void Thing::SetSerial(Stream &serial) { zbee_.setSerial(serial); }

void Thing::GetMacAddress() {
  // Serial High
  uint8_t shCmd[] = {'S', 'H'};
  // Serial Low
  uint8_t slCmd[] = {'S', 'L'};
  AtCommandRequest atRequestSH = AtCommandRequest(shCmd);
  AtCommandRequest atRequestSL = AtCommandRequest(slCmd);
  AtCommandResponse atResponseSH = AtCommandResponse();
  AtCommandResponse atResponseSL = AtCommandResponse();

  while (1) {
    zbee_.send(atRequestSH);

    if (zbee_.readPacket(5000)) {
      if (zbee_.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
        zbee_.getResponse().getAtCommandResponse(atResponseSH);
        if (atResponseSH.isOk()) {
          for (int i = 0; i < atResponseSH.getValueLength(); i++) {
            mac_address_[i] = atResponseSH.getValue()[i];
          }
          break;
        }
      }
    } else {
      SOPLOGLN(F("Serial High READ ERROR!!"));
      delay(1000);
    }
  }

  delay(1000);
  while (1) {
    zbee_.send(atRequestSL);

    if (zbee_.readPacket(5000)) {
      if (zbee_.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
        zbee_.getResponse().getAtCommandResponse(atResponseSL);
        if (atResponseSL.isOk()) {
          for (int i = 0; i < atResponseSL.getValueLength(); i++) {
            mac_address_[i + 4] = atResponseSL.getValue()[i];
          }
          break;
        }
      }
    } else {
      SOPLOGLN(F("Serial Low READ ERROR!!"));
      delay(1000);
    }
  }
}

void Thing::GenerateClientId() {
  // generate mac address and put it on mac_address_
  GetMacAddress();

  int len = 0;
  char temp_mac_address[MAC_ADDRESS_SIZE + 1] = "";
  for (int i = 0; i < 8; i++) {
    len +=
        sprintf(temp_mac_address + len, "%.2X", (unsigned char)mac_address_[i]);
  }

  client_id_ = (char *)malloc(
      sizeof(char) * (strlen(class_name_) + strlen(temp_mac_address) + 2));
  MEM_ALLOC_CHECK(client_id_);

  sprintf(client_id_, "%s_%s", class_name_, temp_mac_address);
  SOPLOG(F("Client ID: "));
  SOPLOGLN(client_id_);
};

void Thing::SetClassName(char *class_name) { class_name_ = class_name; }

bool Thing::compareTimeStamp(Value *t) {
  bool time_passed = false;
  unsigned long curr_time = 0;
  unsigned long diff_time = 0;
  unsigned long last_sent_time = 0;

  last_sent_time = t->get_last_sent_time();

  // set true for the initial case
  if (last_sent_time == 0) {
    t->set_last_sent_time();
    return true;
  }

  curr_time = millis();

  diff_time = curr_time - last_sent_time;

  // millis() goes back to zero after approximately 50 days
  if ((diff_time < 0) ||
      (diff_time >= (unsigned long)t->get_sleep_ms_interval())) {
    t->set_last_sent_time();
    time_passed = true;
  } else {
    time_passed = false;
  }

  return time_passed;
}

// publish(QOS_FLAG, id_2003_, NULL, 0);
void Thing::SendAliveMessage() {
  static unsigned long curr_time = 0;
  static unsigned long last_sent_time = 0;
  unsigned long diff_time = 0;
  char *pszDummy = (char *)"AliveCycleReached";

  curr_time = millis();

  // set true for the initial case
  if (last_sent_time == 0) {
    last_sent_time = curr_time;
    return;
  }

  diff_time = curr_time - last_sent_time;

  // send aliveness twice in a period.
  // assume two QoS-0 packet in a row should not be missed!
  // millis() goes back to zero after approximately 50 days
  if ((diff_time < 0) || (diff_time >= alive_cycle_ / 2 * 1000)) {
    SOPLOG(F("[DEBUG] Send Alive in "));
    SOPLOGLN(diff_time);
    
    publish(QOS_FLAG, id_2003_, pszDummy, strlen(pszDummy));
    last_sent_time = curr_time;
  }
  return;
}

void Thing::sendAliveMessageNoCond() {
  char *pszDummy = (char *)"AliveNoCond";

  publish(QOS_FLAG, id_2003_, pszDummy, strlen(pszDummy));
}

//////////////////////////////////////////////////////////////////////////

void Thing::ReadZbeeTimeout(int timeout) {
  while (zbee_.readPacket(timeout)) {
    if (zbee_.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      ZBTxStatusResponse txStatus;
      zbee_.getResponse().getZBTxStatusResponse(txStatus);
      if (txStatus.isSuccess())
        SOPLOGLN(F("[SUCCESS] Zigbee Send Success... waiting for response"));
      else
        SOPLOGLN(F("[ERROR] Zigbee Send Fail."));
    } else if (zbee_.getResponse().getApiId() == ZB_RX_RESPONSE) {
      zbee_.getResponse().getZBRxResponse(zbee_rx_);
      SOPLOGLN(F("[SUCCESS] Zigbee Receive Success... parsing the stream"));

      ParseStream((char *)zbee_rx_.getData(), zbee_rx_.getDataLength());
      timeout = 50;  // exit instantly after receiving a packet from gateway
    } else if (zbee_.getResponse().isError()) {
      SOPLOGLN(F("[ERROR] ZigBee Response Error."));
    } else {
      SOPLOGLN(F("[WARNING] Unexpected Response."));
    }
  }
}

void Thing::ReadZbeeIfAvailable() {
  while (1) {
    zbee_.readPacket();

    if (zbee_.getResponse().isAvailable()) {
      if (zbee_.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
        ZBTxStatusResponse txStatus;
        zbee_.getResponse().getZBTxStatusResponse(txStatus);
        if (txStatus.isSuccess())
          SOPLOGLN(F("[SUCCESS] Zigbee Send Success... waiting for response"));
        else
          SOPLOGLN(F("[ERROR] Zigbee Send Fail."));
      } else if (zbee_.getResponse().getApiId() == ZB_RX_RESPONSE) {
        zbee_.getResponse().getZBRxResponse(zbee_rx_);
        SOPLOGLN(F("[LED DEBUG SUCCESS] Zigbee Receive Success"));
        ParseStream((char *)zbee_rx_.getData(), zbee_rx_.getDataLength());
      } else if (zbee_.getResponse().isError()) {
        SOPLOGLN(F("[ERROR] ZigBee Response Error."));
      } else {
        SOPLOGLN(F("[WARNING] Unexpected Response."));
      }
    } else {
      SOPLOGLN(F("[LED DEBUG] zbee_.getResponse().isAvailable() is false"));
      break;
    }
  }
}

bool Thing::valid() { return valid_; }

bool Thing::waitForResponse() {
  if (waiting_for_response_) {
    // TODO: Watch out for overflow.
    if ((millis() - response_timer_) > (T_RETRY * 1000L)) {
      response_timer_ = millis();
      SOPLOGLN(F("response_failed\n"));
      if (response_retries_ == 0) {
        waiting_for_response_ = false;
        disconnectHandler(NULL);
      } else {
        unicast();
      }

      --response_retries_;
    }
  }

  return waiting_for_response_;
}

bool Thing::connected() { return connected_; }

void Thing::ParseStream(char *buf, uint16_t len) {
  if (len > MAX_BUFFER_SIZE) {
    SOPLOGLN(F("[ERROR] packet length is larger than MAX_BUFFER_SIZE"));

    return;
  }
  memcpy(message_buffer_, (const void *)buf, len);
  dispatch();
}

void Thing::dispatch() {
  message_header *response_message = (message_header *)message_buffer_;
  SOPLOG(F("[DEBUG] response_msg len="));
  SOPLOGLN(response_message->length);
  SOPLOG(F("[DEBUG] response_msg type= "));
  SOPLOGLN(response_message->type, HEX);

  switch (response_message->type) {
    case ADVERTISE:
      if (!waiting_for_response_ ||
          (response_wait_for_ != ADVERTISE && response_wait_for_ != GWINFO))
        return;
      advertiseHandler((msg_advertise *)message_buffer_);
      break;

    case GWINFO:
      if (response_wait_for_ != GWINFO) return;
      gwinfoHandler((msg_gwinfo *)message_buffer_);
      break;

    case CONNACK:
      if (!waiting_for_response_ || response_wait_for_ != CONNACK) return;
      connackHandler((msg_connack *)message_buffer_);
      break;

    case REGACK:
      if (!waiting_for_response_ || response_wait_for_ != REGACK) {
        SOPLOGLN(
            F("[ERROR] in !waiting_for_response_ || response_wait_for_ != "
              "REGACK"));
        return;
      }

      regackHandler((msg_regack *)message_buffer_);
      break;

    case PUBLISH:
      if (in_process_) {
        SOPLOGLN(
            F("[ERROR] Ignore PUBLISH message for waiting previous publish "
              "message"));
        return;
      }
      publishHandler((msg_publish *)message_buffer_);
      break;

    case SUBACK:
      if (!waiting_for_response_ || response_wait_for_ != SUBACK) return;
      subackHandler((msg_suback *)message_buffer_);
      break;

    case PINGREQ:
      pingreqHandler();
      break;

    case PINGRESP:
      if (!waiting_for_response_ || response_wait_for_ != PINGRESP) return;
      pingrespHandler();
      break;

    case DISCONNECT:
      disconnectHandler((msg_disconnect *)message_buffer_);
      break;

    case DEVREG:
      // cannot be happend
      devregHandler();
      break;

    case DEVREGACK:
      //	if (!waiting_for_response_ || response_wait_for_ != DEVREGACK)
      // return;
      devregackHandler((msg_devregack *)message_buffer_);
      break;

#ifdef USE_QOS2
    case PUBREC:
      pubrecHandler((msg_pubqos2 *)message_buffer_);
      break;

    case PUBREL:
      pubrelHandler((msg_pubqos2 *)message_buffer_);
      break;

    case PUBCOMP:
      pubcompHandler((msg_pubqos2 *)message_buffer_);
      break;
#endif

    default:
      SOPLOGLN(F("[ERROR] Not supported type."));
      return;
  }

  waiting_for_response_ = false;
}

void Thing::broadcast() {
  message_header *hdr = reinterpret_cast<message_header *>(message_buffer_);
  uint16_t addr16 = ZB_BROADCAST_ADDRESS;
  XBeeAddress64 addr64 = XBeeAddress64(0, 0xffff);

  // TODO(ikess): check what is diffrence between ZB_TX_UNICAST,
  // ZB_TX_BROADCAST??
  zbee_tx_ = ZBTxRequest(addr64, addr16, SEARCH_RADIUS, ZB_TX_UNICAST,
                         message_buffer_, hdr->length, DEFAULT_FRAME_ID);
  // zbee_tx_ = ZBTxRequest(addr64, message_buffer_, hdr->length);

  sendPacket();
}

void Thing::sendPacket() {
  SOPLOGLN(F("Send packet!"));
  zbee_.send(zbee_tx_);
}

void Thing::unicast() {
  if (!valid_) {
    broadcast();
    return;
  }

  message_header *hdr = reinterpret_cast<message_header *>(message_buffer_);

  zbee_tx_ = ZBTxRequest(gateway_address_64_, message_buffer_, hdr->length);
  sendPacket();

  if (!waiting_for_response_) {
    response_timer_ = millis();
    response_retries_ = N_RETRY;
  }

  /*
  int8_t hi = ((client_id_ >> 8) & 0xff);
  int8_t lo = ((client_id_ >> 0) & 0xff);
  buffer[0] = 0x7e;
  buffer[1] = hi;
  buffer[2] = lo;
  for(int i = 3; i<hdr->length+3; i++) buffer[i] = message_buffer_[i-3];

  Serial.write("mySerial sends message\n"));
  for(int i = 0; i< hdr->length+3; i++){CPDBG(F(" 0x"));
  CPDBG(F(buffer[i],HEX);} CPDBG(F();


  mySerial.write(buffer, hdr->length+3);
  mySerial.flush();
  */
}

void Thing::advertiseHandler(const msg_advertise *msg) {
  if (!valid_) {
    gateway_id_ = msg->gw_id;
    valid_ = true;
  }
}

void Thing::gwinfoHandler(const msg_gwinfo *msg) {
  if (!valid_) {
    gateway_address_64_ = zbee_rx_.getRemoteAddress64();
    gateway_address_16_ = zbee_rx_.getRemoteAddress16();

    gateway_id_ = msg->gw_id;

    valid_ = true;
  }
}

void Thing::connackHandler(const msg_connack *msg) {
  connected_ = 1;
  if (connect_handler_ != NULL) connect_handler_();
}

void Thing::regackHandler(const msg_regack *msg) {
  SOPLOGLN(F("in regackHandler"));
  if (msg->return_code == 0 && bswap(msg->message_id) == message_id_) {
    SOPLOGLN(F("in regackHandler if"));
    registered_id_ = bswap(msg->topic_id);
  }
  SOPLOGLN(F("in regackHandler if end"));
}

void Thing::disconnectHandler(const msg_disconnect *msg) {
  if (connected_ && disconnect_handler_ != NULL) disconnect_handler_();
  connected_ = false;
  valid_ = false;
  registered_ = false;
}

void Thing::publishHandler(const msg_publish *msg) {
  return_code_t ret = REJECTED_INVALID_TOPIC_ID;

  SOPLOG("publshHandler - meesage topic id: ");
  SOPLOGLN(msg->topic_id);

  uint16_t topic_id = bswap(msg->topic_id);

  if (topic_id == id_1001_) {
    snprintf(buffer, MAX_BUFFER_SIZE, "%s", msg->data);
    char errNo = buffer[0];

    SOPLOG(F("Error no :"));
    SOPLOGLN(buffer);

    if (errNo == '0') {
      // registered_ = !registered_;
      registered_ = true;
    } else if (strcmp(buffer, "-4") == 0) {
      SOPLOGLN(F("Duplicated Thing error message from middleware."));
      // registered_ = !registered_;
      registered_ = true;
    } else {
      // do nothing
    }

    SOPLOG(F("Registered: "));
    SOPLOGLN(registered_);

#ifdef USE_QOS2
    in_process_ = true;
    if (msg->flags & FLAG_QOS_2) {
      ret = ACCEPTED;
      pubrec(msg);
    }
    in_process_ = false;
#endif

    return;
  }
  /*
  if (topic_id == id_1002_)
  {
          publish(QOS_FLAG, id_2003_, NULL, 0);
          return ACCEPTED;
  }*/

  for (int i = 0; i < num_functions_; i++) {
    SOPLOGLN(functions_[i]->id_1003());
    if (topic_id == functions_[i]->id_1003()) {
      int success = -1;
      char *pTokPtr = NULL;
      char *t_name = NULL;
      char *t_args = NULL;

      in_process_ = true;
      strncpy(save_buffer, msg->data,
              MAX_BUFFER_SIZE - sizeof(msg_publish));  // safe cpy
#ifdef USE_QOS2
      if (msg->flags & FLAG_QOS_2) {
        ret = ACCEPTED;
        pubrec(msg);
      }
#endif
      in_process_ = false;

      t_name = strtok_r(save_buffer, ":", &pTokPtr);
      t_args = strtok_r(NULL, ":", &pTokPtr);

      switch (functions_[i]->function_classifier()) {
        case VOID:
          functions_[i]->Execute(t_args, &success);
          break;
        case INTEGER:
        case DOUBLE:
        case BOOL:
        default:
          success = -7;
          break;  // cannot be occured
      }

      snprintf(buffer, MAX_BUFFER_SIZE,
               "{\"scenario\" : \"%s\" , \"error\" : %d }", t_name, success);
      publish(QOS_FLAG, functions_[i]->id_2004(), buffer, strlen(buffer));

      return;
    }
  }
}

void Thing::subackHandler(const msg_suback *msg) {}

void Thing::searchgw(const uint8_t radius) {
  msg_searchgw *msg = reinterpret_cast<msg_searchgw *>(message_buffer_);

  msg->length = sizeof(msg_searchgw);
  msg->type = SEARCHGW;
  msg->radius = radius;

  broadcast();
  waiting_for_response_ = true;
  response_wait_for_ = GWINFO;
}

void Thing::connect(const uint8_t flags, const uint16_t duration,
                    const char *client_id_) {
  msg_connect *msg = reinterpret_cast<msg_connect *>(message_buffer_);

  msg->length = sizeof(msg_connect) + strlen(client_id_);
  msg->type = CONNECT;
  msg->flags = flags;
  msg->protocol_id = PROTOCOL_ID;
  msg->duration = bswap(duration);
  strcpy(msg->client_id, client_id_);

  unicast();
  connected_ = false;
  waiting_for_response_ = true;
  response_wait_for_ = CONNACK;
}

void Thing::disconnect(const uint16_t duration) {
  msg_disconnect *msg = reinterpret_cast<msg_disconnect *>(message_buffer_);

  msg->length = sizeof(message_header);
  msg->type = DISCONNECT;

  if (duration > 0) {
    msg->length += sizeof(msg_disconnect);
    msg->duration = bswap(duration);
  }

  unicast();
  waiting_for_response_ = true;
}

bool Thing::registerTopic(const char *name) {
  SOPLOGLN(F("in registerTopic"));
  if (!waiting_for_response_) {
    ++message_id_;

    // Fill in the next table entry, but we only increment the counter to
    // the next topic when we get a REGACK from the broker. So don't issue
    // another REGISTER until we have resolved this one.

    registered_id_ = -1;
    msg_register *msg = reinterpret_cast<msg_register *>(message_buffer_);

    msg->length = sizeof(msg_register) + strlen(name);
    msg->type = REGISTER;
    msg->topic_id = 0;
    msg->message_id = bswap(message_id_);
    strcpy(msg->topic_name, name);

    unicast();
    waiting_for_response_ = true;
    response_wait_for_ = REGACK;
    return true;
  } else {
    SOPLOGLN(F("waiting_for_response_ is false error!"));
  }

  return false;
}

void Thing::publish(const uint8_t flags, const uint16_t topicId,
                    const void *data, const uint8_t data_len) {
  ++message_id_;

  msg_publish *msg = reinterpret_cast<msg_publish *>(message_buffer_);

  msg->length = sizeof(msg_publish) + data_len;
  msg->type = PUBLISH;
  msg->flags = flags;
  msg->topic_id = bswap(topicId);
  msg->message_id = bswap(message_id_);
  memcpy(msg->data, data, data_len);

  unicast();

  if ((flags & QOS_MASK) == FLAG_QOS_2) {
    waiting_for_response_ = true;
    response_wait_for_ = PUBREC;
    while (true) {
      ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
      if (response_wait_for_ == PUBREC) {
        msg->length = sizeof(msg_publish) + data_len;
        msg->type = PUBLISH;
        msg->flags = flags;
        msg->flags |= FLAG_DUP;
        msg->topic_id = bswap(topicId);
        msg->message_id = bswap(message_id_);
        memcpy(msg->data, data, data_len);
        unicast();
      } else
        break;
    }
  }
}

void Thing::devreg() {
  SOPLOGLN(F("DEVREG start"));

  msg_devreg *msg = reinterpret_cast<msg_devreg *>(message_buffer_);
  response_wait_for_ = DEVREGACK;
  waiting_for_response_ = true;

  SOPLOG(F("num_values_: "));
  SOPLOGLN(num_values_);
  for (int i = 0; i < num_values_; i++) {
    device_register_ = false;
    while (!device_register_) {
      message_id_++;
      msg->type = DEVREG;
      msg->pub_id = bswap(i);
      msg->status = VALUE;
      msg->message_id = bswap(message_id_);
      values_[i]->GetInformation(buffer);
      SOPLOGLN(buffer);
      memcpy(msg->data, buffer, strlen(buffer) + 1);
      msg->length = sizeof(msg_devreg) + strlen(buffer) + 1;
      unicast();
      ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    }
  }

  SOPLOG(F("num_attributes_: "));
  SOPLOGLN(num_attributes_);
  for (int i = 0; i < num_attributes_; i++) {
    device_register_ = false;
    while (!device_register_) {
      message_id_++;
      msg->type = DEVREG;
      msg->pub_id = bswap(i);
      msg->status = ATTRIBUTE;
      msg->message_id = bswap(message_id_);
      attributes_[i]->GetInformation(buffer);
      SOPLOGLN(buffer);
      memcpy(msg->data, buffer, strlen(buffer) + 1);
      msg->length = sizeof(msg_devreg) + strlen(buffer) + 1;
      unicast();
      ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    }
  }

  SOPLOG(F("num_functions_: "));
  SOPLOGLN(num_functions_);
  for (int i = 0; i < num_functions_; i++) {
    device_register_ = false;
    while (!device_register_) {
      message_id_++;
      msg->type = DEVREG;
      msg->pub_id = bswap(i);
      msg->status = FUNCTION;
      msg->message_id = bswap(message_id_);
      functions_[i]->GetInformation(buffer);
      SOPLOGLN(buffer);
      memcpy(msg->data, buffer, strlen(buffer) + 1);
      msg->length = sizeof(msg_devreg) + strlen(buffer) + 1;
      unicast();
      ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    }

    SOPLOG(F("num_arguments: "));
    SOPLOGLN(functions_[i]->ncurArguments());
    for (int j = 0; j < functions_[i]->ncurArguments(); j++) {
      device_register_ = false;
      while (!device_register_) {
        message_id_++;
        msg->type = DEVREG;
        msg->pub_id = bswap(j);
        msg->status = ARGUMENT;
        msg->message_id = bswap(message_id_);
        functions_[i]->getIdxArgument(j)->GetInformation(buffer);
        memcpy(msg->data, buffer, strlen(buffer) + 1);
        msg->length = sizeof(msg_devreg) + strlen(buffer) + 1;
        unicast();
        ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
      }
    }

    SOPLOG(F("num_function_attributes: "));
    SOPLOGLN(functions_[i]->ncurFunctionAttributes());
    for (int j = 0; j < functions_[i]->ncurFunctionAttributes(); j++) {
      device_register_ = false;
      while (!device_register_) {
        message_id_++;
        msg->type = DEVREG;
        msg->pub_id = bswap(j);
        msg->status = FUNCTION_ATTRIBUTE;
        msg->message_id = bswap(message_id_);
        functions_[i]->getIdxFunctionAttribute(j)->GetInformation(buffer);
        memcpy(msg->data, buffer, strlen(buffer) + 1);
        msg->length = sizeof(msg_devreg) + strlen(buffer) + 1;
        unicast();
        ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
      }
    }
  }

  // set alive cycle
  device_register_ = false;
  while (!device_register_) {
    message_id_++;
    msg->type = DEVREG;
    msg->pub_id = 0;
    msg->status = DURATION;
    msg->message_id = bswap(message_id_);
    snprintf(buffer, MAX_BUFFER_SIZE, "%ul", alive_cycle_);

    SOPLOG(F("alive cycle: "));
    SOPLOGLN(buffer);

    memcpy(msg->data, buffer, strlen(buffer) + 1);
    msg->length = sizeof(msg_devreg) + strlen(buffer) + 1;
    unicast();
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
  }

  // finish
  registered_ = false;
  while (!registered_) {
    SOPLOGLN(F("start of FINISH state"));

    message_id_++;
    msg->length = sizeof(msg_devreg);
    msg->type = DEVREG;

    SOPLOG(F("ID2001: "));
    SOPLOGLN(id_2001_);

    msg->pub_id = bswap(id_2001_);
    msg->message_id = bswap(message_id_);
    msg->length = sizeof(msg_devreg);
    msg->status = FINISH;
    unicast();
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);

    SOPLOG(F("end of FINISH state: "));
    SOPLOGLN(registered_);
  }

  waiting_for_response_ = false;
  SOPLOGLN(F("out of FINISH state"));
}

void Thing::devregackHandler(const msg_devregack *msg) {
  SOPLOGLN(F("[SUCCESS] REGISTER_THING SUCESS"));
  device_register_ = true;
}

void Thing::devregHandler() {
  SOPLOGLN(F("[ERROR] INVALID PACKET"));
  // do nothing
}

void Thing::subscribe(const uint8_t flags, const uint16_t topicId) {
  ++message_id_;

  msg_subscribe *msg = reinterpret_cast<msg_subscribe *>(message_buffer_);

  msg->length = sizeof(msg_subscribe);
  msg->type = SUBSCRIBE;
  // msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_PREDEFINED_ID;
  msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_NAME;
  msg->message_id = bswap(message_id_);
  msg->topic_id = bswap(topicId);

  unicast();

  if ((flags & QOS_MASK) == FLAG_QOS_1 || (flags & QOS_MASK) == FLAG_QOS_2) {
    waiting_for_response_ = true;
    response_wait_for_ = SUBACK;
  }
}

// for debug
void Thing::print_message_buffer_(int start, int length) {
  // print message_buffer_ BYTE message_buffer_[start] to message_buffer_[start
  // + length]
  if (length == -1) length = MAX_BUFFER_SIZE;
  for (int i = start; i < start + length; i++) {
    Serial.print(message_buffer_[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
}

void Thing::print_message_buffer_(void *buf, int length) {
  // print message_buffer_ BYTE *buf to *(buf + length)
  char *tmp = (char *)buf;
  if (length == -1) length = MAX_BUFFER_SIZE;
  for (int i = 0; i < length; i++) {
    Serial.print(*(tmp + i), HEX);
    Serial.print(" ");
  }
  Serial.println("");
}

void Thing::print_message_buffer_() {
  // print all message_buffer_ BYTE
  for (int i = 0; i < MAX_BUFFER_SIZE; i++) {
    Serial.print(message_buffer_[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
}
void Thing::subscribe(const uint8_t flags, const char *name) {
  ++message_id_;

  msg_subscribe *msg = reinterpret_cast<msg_subscribe *>(message_buffer_);

  msg->length = sizeof(msg_subscribe) + strlen(name) - sizeof(uint16_t);
  msg->type = SUBSCRIBE;
  // msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_PREDEFINED_ID;
  msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_NAME;
  msg->message_id = bswap(message_id_);
  msg->topic_id = 0;
  strcpy(msg->topic_name, name);

  unicast();

  if ((flags & QOS_MASK) == FLAG_QOS_1 || (flags & QOS_MASK) == FLAG_QOS_2) {
    waiting_for_response_ = true;
    response_wait_for_ = SUBACK;
  }
}

void Thing::pingresp(int flag) {
  message_header *msg = reinterpret_cast<message_header *>(message_buffer_);
  msg->length = sizeof(message_header);
  msg->type = PINGRESP;

  if (flag == 0)
    unicast();
  else
    broadcast();
}

void Thing::unsubscribe(const uint8_t flags, const char *name) {
  ++message_id_;

  msg_unsubscribe *msg = reinterpret_cast<msg_unsubscribe *>(message_buffer_);

  msg->length = sizeof(msg_unsubscribe);
  msg->type = UNSUBSCRIBE;
  // msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_PREDEFINED_ID;
  msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_NAME;
  msg->message_id = bswap(message_id_);
  msg->topic_id = 0;
  strcpy(msg->topic_name, name);

  unicast();

  if ((flags & QOS_MASK) == FLAG_QOS_1 || (flags & QOS_MASK) == FLAG_QOS_2) {
    waiting_for_response_ = true;
    response_wait_for_ = UNSUBACK;
  }
}

void Thing::unsubscribe(const uint8_t flags, const uint16_t topicId) {
  ++message_id_;

  msg_unsubscribe *msg = reinterpret_cast<msg_unsubscribe *>(message_buffer_);

  msg->length = sizeof(msg_unsubscribe);
  msg->type = UNSUBSCRIBE;
  // msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_PREDEFINED_ID;
  msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_NAME;
  msg->message_id = bswap(message_id_);
  msg->topic_id = bswap(topicId);

  unicast();

  if ((flags & QOS_MASK) == FLAG_QOS_1 || (flags & QOS_MASK) == FLAG_QOS_2) {
    waiting_for_response_ = true;
    response_wait_for_ = UNSUBACK;
  }
}

void Thing::pingreq() {
  msg_pingreq *msg = reinterpret_cast<msg_pingreq *>(message_buffer_);
  msg->length = sizeof(msg_pingreq) + strlen(client_id_);
  msg->type = PINGREQ;
  strcpy(msg->client_id, client_id_);

  unicast();
  last_ping_ = waiting_for_response_ = true;
  response_wait_for_ = PINGRESP;
}

void Thing::pingreqHandler() {
  if (zbee_rx_.getRemoteAddress16() == gateway_address_16_)
    pingresp(0);
  else
    pingresp(1);
}

void Thing::pingrespHandler() {}

#ifdef USE_QOS2
void Thing::pubrec(const msg_publish *recv_msg) {
  waiting_for_response_ = true;
  response_wait_for_ = PUBREL;
  while (response_wait_for_ == PUBREL) {
    msg_pubqos2 *msg = reinterpret_cast<msg_pubqos2 *>(message_buffer_);
    msg->length = sizeof(msg_pubqos2);
    msg->type = PUBREC;
    msg->message_id = recv_msg->message_id;

    //		CPDBG(F(msg->length);
    //		CPDBG(F(msg->type);
    //		CPDBG(F(msg->message_id);

    unicast();
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
  }
}

void Thing::pubrel(const msg_pubqos2 *recv_msg) {
  waiting_for_response_ = true;
  response_wait_for_ = PUBCOMP;
  while (response_wait_for_ == PUBCOMP) {
    msg_pubqos2 *msg = reinterpret_cast<msg_pubqos2 *>(message_buffer_);
    msg->length = sizeof(msg_pubqos2);
    msg->type = PUBREL;
    msg->message_id = recv_msg->message_id;
    unicast();
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
  }
}

void Thing::pubcomp(const msg_pubqos2 *recv_msg) {
  msg_pubqos2 *msg = reinterpret_cast<msg_pubqos2 *>(message_buffer_);
  msg->length = sizeof(msg_pubqos2);
  msg->type = PUBCOMP;
  msg->message_id = recv_msg->message_id;

  unicast();
}

void Thing::pubrecHandler(const msg_pubqos2 *msg) { pubrel(msg); }

void Thing::pubrelHandler(const msg_pubqos2 *msg) {
  waiting_for_response_ = false;
  response_wait_for_ = PUBLISH;
  pubcomp(msg);
}

void Thing::pubcompHandler(const msg_pubqos2 *msg) {
  waiting_for_response_ = false;
  response_wait_for_ = PUBLISH;
}

#endif
