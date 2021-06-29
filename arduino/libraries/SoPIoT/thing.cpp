#include "thing.h"
#include "utils.h"

static inline uint16_t bswap(const uint16_t val) {
  return (val << 8) | (val >> 8);
}

Thing::Thing()
    : gateway_response_wait_(true),
      protocal_response_wait_(ADVERTISE),
      gateway_connected_(false),
      message_id_(0),
      gateway_id_(0),
      gateway_ready_(0),
      middleware_registered_(0),
      registered_id_(UINT16_MAX),
      zbee_(),
      publish_process_(false) {
  num_values_ = 0;
  num_functions_ = 0;
}

// Default alive_cycle is 60
Thing::Thing(const char* class_name, Stream& serial) {
  Thing(class_name, 60, serial);
}

Thing::Thing(const char* class_name, int alive_cycle, Stream& serial)
    : gateway_response_wait_(true),
      protocal_response_wait_(ADVERTISE),
      gateway_connected_(false),
      message_id_(0),
      gateway_id_(0),
      gateway_ready_(0),
      middleware_registered_(0),
      registered_id_(UINT16_MAX),
      zbee_() {
  zbee_.setSerial(serial);
  this->class_name_ = strdup(class_name);
  MEM_ALLOC_CHECK(class_name_);

  this->alive_cycle_ = alive_cycle;
  memset(message_buffer_, 0, MAX_BUFFER_SIZE);

  num_values_ = 0;
  num_functions_ = 0;
}

Thing::~Thing() {
  free(client_id_);
  free(class_name_);
}

const char* Thing::ClientId() { return client_id_; }

void Thing::Add(Value& v) {
  if (num_values_ < MAX_VALUE_NUM) {
    values_[num_values_++] = &v;
  } else {
    SOPLOGLN(F("It cannot be added (more than limit)\n"));
  }
}

void Thing::Add(Function& f) {
  if (num_functions_ < MAX_FUNCTION_NUM) {
    functions_[num_functions_++] = &f;
  } else {
    SOPLOGLN(F("It cannot be added (more than limit)\n"));
  }
}

void Thing::Setup() {
  SOPLOGLN(F("==== Small Thing start ===="));
  SOPLOGLN(F("Start to get clientID"));
  GenerateClientId();
  SOPLOGLN(F("ClientID : %s"), client_id_);

  SOPLOGLN(F("==== Setup SEARCHGW ===="));
  while (!GatewayReady()) {
    searchgw(SEARCH_RADIUS);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
  }
  SOPLOGLN(F("Gateway ID : %d"), gateway_id_);

  SOPLOGLN(F("==== Setup CONNECT ===="));
  while (!GatewayConnected()) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, "%s", client_id_);
    connect(0, 50, publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
  }
  SOPLOGLN(F("CONNECT success"));

  // Start REGISTER & SUBSCRIBE
  SOPLOGLN(F("==== Setup REGISTER & SUBSCRIBE ===="));
  registered_id_ = UINT16_MAX;
  while (registered_id_ == UINT16_MAX) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, MT1001, client_id_);  // id_1001_
    registerTopic(publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    id_1001_ = registered_id_;
  }
  SOPLOGLN(F("REGISTER Topic %s"), publish_buffer);

  subscribe(QOS_FLAG, publish_buffer);
  ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
  SOPLOGLN(F("SUBSCRIBE Topic %s"), publish_buffer);

  // Author: thsvkd
  // Remove PingReq part

  registered_id_ = UINT16_MAX;
  while (registered_id_ == UINT16_MAX) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, TM2001, client_id_);  // id_2001_
    registerTopic(publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    id_2001_ = registered_id_;
  }
  SOPLOGLN(F("REGISTER Topic %s"), publish_buffer);

  registered_id_ = UINT16_MAX;
  while (registered_id_ == UINT16_MAX) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, TM2002, client_id_);  // id_2002_
    registerTopic(publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    id_2002_ = registered_id_;
  }
  SOPLOGLN(F("REGISTER Topic %s"), publish_buffer);

  // publish(QOS_FLAG, registered_id_, "{}", 3);
  // SOPLOGLN(F("Send UNREGISTER for clean start"));

  registered_id_ = UINT16_MAX;
  while (registered_id_ == UINT16_MAX) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, TM2003, client_id_);  // id_2003_
    registerTopic(publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    id_2003_ = registered_id_;
  }
  SOPLOGLN(F("REGISTER Topic %s"), publish_buffer);

  SOPLOGLN(F("==== Setup REGISTER *New* Topics for Small Thing ===="));

  registered_id_ = UINT16_MAX;
  while (registered_id_ == UINT16_MAX) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, TM2010, client_id_);  // id_2010_
    registerTopic(publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    id_2010_ = registered_id_;
  }
  SOPLOGLN(F("REGISTER Topic %s"), publish_buffer);

  registered_id_ = UINT16_MAX;
  while (registered_id_ == UINT16_MAX) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, TM2011, client_id_);  // id_2011_
    registerTopic(publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    id_2011_ = registered_id_;
  }
  SOPLOGLN(F("REGISTER Topic %s"), publish_buffer);

  registered_id_ = UINT16_MAX;
  while (registered_id_ == UINT16_MAX) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, TM2012, client_id_);  // id_2012_
    registerTopic(publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    id_2012_ = registered_id_;
  }
  SOPLOGLN(F("REGISTER Topic %s"), publish_buffer);

  registered_id_ = UINT16_MAX;
  while (registered_id_ == UINT16_MAX) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, TM2013, client_id_);  // id_2013_
    registerTopic(publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    id_2013_ = registered_id_;
  }
  SOPLOGLN(F("REGISTER Topic %s"), publish_buffer);

  registered_id_ = UINT16_MAX;
  while (registered_id_ == UINT16_MAX) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, TM2014, client_id_);  // id_2014_
    registerTopic(publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    id_2014_ = registered_id_;
  }
  SOPLOGLN(F("REGISTER Topic %s"), publish_buffer);

  registered_id_ = UINT16_MAX;
  while (registered_id_ == UINT16_MAX) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, TM2015, client_id_);  // id_2015_
    registerTopic(publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    id_2015_ = registered_id_;
  }
  SOPLOGLN(F("REGISTER Topic %s"), publish_buffer);

  registered_id_ = UINT16_MAX;
  while (registered_id_ == UINT16_MAX) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, TM2016, client_id_);  // id_2016_
    registerTopic(publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    id_2016_ = registered_id_;
  }
  SOPLOGLN(F("REGISTER Topic %s"), publish_buffer);

  // Start Value
  SOPLOGLN(F("==== Setup REGISTER Value Topics ===="));
  for (int i = 0; i < num_values_; i++) {
    registered_id_ = UINT16_MAX;
    while (values_[i]->set_publish_id(registered_id_) == UINT16_MAX) {
      snprintf(publish_buffer, MAX_BUFFER_SIZE, COMMON0000, client_id_,
               values_[i]->GetName());
      registerTopic(publish_buffer);

      SOPLOGLN(F("REGISTER Topic %s"), publish_buffer);
      ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    }
    values_[i]->AddTag(client_id_);
  }

  // Start Function
  SOPLOGLN(F("==== Setup REGISTER Function Topics ===="));
  for (int i = 0; i < num_functions_; i++) {
    registered_id_ = UINT16_MAX;
    while ((functions_[i]->set_id_1003(registered_id_)) == UINT16_MAX) {
      snprintf(publish_buffer, MAX_BUFFER_SIZE, MT1003,
               functions_[i]->GetName(), client_id_);
      registerTopic(publish_buffer);

      SOPLOGLN(F("REGISTER Topic %s"), publish_buffer);
      ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    }

    subscribe(QOS_FLAG, publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    SOPLOGLN(F("SUBSCRIBE Topic %s"), publish_buffer);

    registered_id_ = UINT16_MAX;
    while ((functions_[i]->set_id_2004(registered_id_)) == UINT16_MAX) {
      snprintf(publish_buffer, MAX_BUFFER_SIZE, TM2004,
               functions_[i]->GetName(), client_id_);
      registerTopic(publish_buffer);

      SOPLOGLN(F("REGISTER Topic %s"), publish_buffer);
      ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    }
    functions_[i]->AddTag(client_id_);
  }

  PrintTopicID();
  // TestPublish();

  Register();

  SendAliveMessageNoCond();
  SendInitialValueNoCond();
  SOPLOGLN(F("==== Registeration to the Middleware Finish ===="));
}

void Thing::PrintTags() {
  for (int i = 0; i < num_values_; i++) {
    SOPLOGLN(F("tag num: %d"), values_[i]->getTagNum());
    for (int j = 0; j < values_[i]->getTagNum(); j++) {
      SOPLOGLN(F("values_[%d] tag[%d]: %s"), i, j,
               values_[i]->getTag(j)->GetName());
    }
  }

  for (int i = 0; i < num_functions_; i++) {
    SOPLOGLN(F("tag num: %d"), functions_[i]->getTagNum());
    for (int j = 0; j < functions_[i]->getTagNum(); j++) {
      SOPLOGLN(F("functions_[%d] tag[%d]: %s"), i, j,
               functions_[i]->getTag(j)->GetName());
    }
  }
}

void Thing::PrintTopicID() {
  SOPLOGLN(F("id_1001_: %d"), id_1001_);
  SOPLOGLN(F("id_2001_: %d"), id_2001_);
  SOPLOGLN(F("id_2002_: %d"), id_2002_);
  SOPLOGLN(F("id_2003_: %d"), id_2003_);
  SOPLOGLN(F("id_2010_: %d"), id_2010_);
  SOPLOGLN(F("id_2011_: %d"), id_2011_);
  SOPLOGLN(F("id_2012_: %d"), id_2012_);
  SOPLOGLN(F("id_2013_: %d"), id_2013_);
  SOPLOGLN(F("id_2014_: %d"), id_2014_);
  SOPLOGLN(F("id_2015_: %d"), id_2015_);
  SOPLOGLN(F("id_2016_: %d"), id_2016_);

  for (int i = 0; i < num_values_; i++) {
    SOPLOGLN(F("values_[%d] pub id: %d"), i, values_[i]->publish_id());
  }

  for (int i = 0; i < num_functions_; i++) {
    SOPLOGLN(F("function_[%d] id_1003: %d"), i, functions_[i]->id_1003());
    SOPLOGLN(F("function_[%d] id_2004: %d"), i, functions_[i]->id_2004());
  }
}

void Thing::TestPublish() {
  while (1) {
    publish(QOS_FLAG, id_1001_, "id_1001_ test", strlen("id_1001_ test"));
    publish(QOS_FLAG, id_2001_, "id_2001_ test", strlen("id_2001_ test"));
    publish(QOS_FLAG, id_2002_, "id_2002_ test", strlen("id_2002_ test"));
    publish(QOS_FLAG, id_2003_, "id_2003_ test", strlen("id_2003_ test"));
    publish(QOS_FLAG, id_2010_, "id_2010_ test", strlen("id_2010_ test"));
    publish(QOS_FLAG, id_2011_, "id_2011_ test", strlen("id_2011_ test"));
    publish(QOS_FLAG, id_2012_, "id_2012_ test", strlen("id_2012_ test"));
    publish(QOS_FLAG, id_2013_, "id_2013_ test", strlen("id_2013_ test"));
    publish(QOS_FLAG, id_2014_, "id_2014_ test", strlen("id_2014_ test"));
    publish(QOS_FLAG, id_2015_, "id_2015_ test", strlen("id_2015_ test"));
    publish(QOS_FLAG, id_2016_, "id_2016_ test", strlen("id_2016_ test"));

    for (int i = 0; i < num_values_; i++) {
      publish(QOS_FLAG, values_[i]->publish_id(), "values_ test",
              strlen("values_ test"));
    }

    for (int i = 0; i < num_functions_; i++) {
      SOPLOGLN(F("function_[%d] id_1003: %d"), i, functions_[i]->id_1003());
      SOPLOGLN(F("function_[%d] id_2004: %d"), i, functions_[i]->id_2004());
    }

    delay(2000);
  }
}

void Thing::Loop(int pub_period) {
  bool time_passed = false;
  bool changed = false;

  // Wait for Response about Value
  SendAliveMessage();
  ReadZbeeIfAvailable();
  for (uint8_t i = 0; i < num_values_; i++) {
    time_passed = compareTimeStamp(values_[i]);
    if (time_passed) {
      changed = values_[i]->GetPublishJson(publish_buffer);
      if (changed) {
        SOPLOGLN(F("pub_id: %d, buffer: %s"), values_[i]->publish_id(),
                 publish_buffer);
        publish(QOS_FLAG, values_[i]->publish_id(), publish_buffer,
                strlen(publish_buffer));
      }
    }
    ReadZbeeIfAvailable();
  }
}

//----------------------------------------
// Private Functions
//----------------------------------------

void Thing::Register() {
  for (int i = 0; i < num_values_; i++) {
    values_[i]->GetInformation(publish_buffer);
    publish(QOS_FLAG, id_2010_, publish_buffer, strlen(publish_buffer));
    for (int j = 0; j < values_[i]->getTagNum(); j++) {
      publish(QOS_FLAG, id_2011_, values_[i]->getTag(j)->GetName(),
              strlen(values_[i]->getTag(j)->GetName()));
    }
  }

  for (int i = 0; i < num_functions_; i++) {
    functions_[i]->GetInformation(publish_buffer);
    publish(QOS_FLAG, id_2012_, publish_buffer, strlen(publish_buffer));
    for (int j = 0; j < functions_[i]->getTagNum(); j++) {
      publish(QOS_FLAG, id_2013_, functions_[i]->getTag(j)->GetName(),
              strlen(functions_[i]->getTag(j)->GetName()));
    }

    for (int j = 0; j < functions_[i]->getArgumentNum(); j++) {
      functions_[i]->getArgument(j)->GetInformation(publish_buffer);
      publish(QOS_FLAG, id_2014_, publish_buffer, strlen(publish_buffer));
    }
  }

  snprintf(publish_buffer, MAX_BUFFER_SIZE, "%d", alive_cycle_);
  publish(QOS_FLAG, id_2015_, publish_buffer, strlen(publish_buffer));

  while (!middleware_registered_) {
    publish(QOS_FLAG, id_2016_, "{}", strlen("{}"));
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
  }
}

void Thing::SetSerial(Stream& serial) { zbee_.setSerial(serial); }

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

  client_id_ = (char*)malloc(
      sizeof(char) * (strlen(class_name_) + strlen(temp_mac_address) + 2));
  MEM_ALLOC_CHECK(client_id_);

  sprintf(client_id_, "%s_%s", class_name_, temp_mac_address);
};

void Thing::SetClassName(char* class_name) { class_name_ = class_name; }

bool Thing::compareTimeStamp(Value* t) {
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
  char* pszDummy = (char*)"AliveCycleReached";

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
    SOPLOGLN(F("[DEBUG] Send Alive"));
    // SOPLOGLN(diff_time);

    publish(QOS_FLAG, id_2003_, pszDummy, strlen(pszDummy));
    last_sent_time = curr_time;
  }
  return;
}

void Thing::SendAliveMessageNoCond() {
  char* pszDummy = (char*)"AliveNoCond";

  publish(QOS_FLAG, id_2003_, pszDummy, strlen(pszDummy));
}

void Thing::SendInitialValueNoCond() {
  for (uint8_t i = 0; i < num_values_; i++) {
    // Value to Json String
    values_[i]->GetPublishJson(publish_buffer);
    // Publish values
    publish(QOS_FLAG, values_[i]->publish_id(), publish_buffer,
            strlen(publish_buffer));
  }
}

//////////////////////////////////////////////////////////////////////////

void Thing::ReadZbeeTimeout(int timeout) {
  while (zbee_.readPacket(timeout)) {
    if (zbee_.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      ZBTxStatusResponse txStatus;
      zbee_.getResponse().getZBTxStatusResponse(txStatus);
      if (txStatus.isSuccess()) {
        // SOPLOGLN(F("[SUCCESS] Zigbee send success... wait response"));
      } else {
        // SOPLOGLN(F("[ERROR] Zigbee send fail."));
      }
    } else if (zbee_.getResponse().getApiId() == ZB_RX_RESPONSE) {
      zbee_.getResponse().getZBRxResponse(zbee_rx_);
      // SOPLOGLN(F("[SUCCESS] Zigbee receive success... parse stream"));

      ParseMQTTSNStream((char*)zbee_rx_.getData(), zbee_rx_.getDataLength());
      timeout = ESCAPE_ZBEE_TIMEOUT;  // exit instantly after receiving a packet
                                      // from gateway
    } else if (zbee_.getResponse().isError()) {
      // SOPLOGLN(F("[ERROR] ZigBee response error."));
    } else {
      // SOPLOGLN(F("[ERROR] Unexpected response."));
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
        if (txStatus.isSuccess()) {
          // SOPLOGLN(F("[SUCCESS] Zigbee Send Success... waiting for
          // response"));
        } else {
          // SOPLOGLN(F("[ERROR] Zigbee Send Fail."));
        }
      } else if (zbee_.getResponse().getApiId() == ZB_RX_RESPONSE) {
        zbee_.getResponse().getZBRxResponse(zbee_rx_);
        SOPLOGLN(F("[DEBUG] Zigbee Receive Success"));
        ParseMQTTSNStream((char*)zbee_rx_.getData(), zbee_rx_.getDataLength());
      } else if (zbee_.getResponse().isError()) {
        // SOPLOGLN(F("[ERROR] ZigBee Response Error."));
      } else {
        // SOPLOGLN(F("[WARNING] Unexpected Response."));
      }
    } else {
      // SOPLOGLN(F("[LED DEBUG] zbee_.getResponse().isAvailable() is false"));
      break;
    }
  }
}

bool Thing::GatewayReady() { return gateway_ready_; }

bool Thing::GatewayConnected() { return gateway_connected_; }

void Thing::ParseMQTTSNStream(char* buf, uint16_t len) {
  message_header* response_message = (message_header*)message_buffer_;

  if (len > MAX_BUFFER_SIZE) {
    SOPLOGLN(F("[DEBUG] packet length is larger than MAX_BUFFER_SIZE: %d"),
             len);
    // return;
  }

  SOPLOGLN(F("xbee recv : "));
  SOPLOGLN(F(message_buffer_));
  memcpy(message_buffer_, (const void*)buf, len);

  switch (response_message->type) {
    case ADVERTISE:
      if (!gateway_response_wait_ || (protocal_response_wait_ != ADVERTISE))
        return;
      advertiseHandler((msg_advertise*)message_buffer_);
      break;

    case GWINFO:
      if (!gateway_response_wait_ || protocal_response_wait_ != GWINFO) return;
      gwinfoHandler((msg_gwinfo*)message_buffer_);
      break;

    case CONNACK:
      if (!gateway_response_wait_ || protocal_response_wait_ != CONNACK) return;
      connackHandler((msg_connack*)message_buffer_);
      break;

    case REGACK:
      if (!gateway_response_wait_ || protocal_response_wait_ != REGACK) {
        SOPLOGLN(F(
            "[ERROR] in !gateway_response_wait_ || protocal_response_wait_ != "
            "REGACK"));
        return;
      }

      regackHandler((msg_regack*)message_buffer_);
      break;

    case PUBLISH:
      if (publish_process_) {
        SOPLOGLN(
            F("[ERROR] Ignore PUBLISH message for waiting previous publish "
              "message"));
        return;
      }
      publishHandler((msg_publish*)message_buffer_);
      break;

    case SUBACK:
      if (!gateway_response_wait_ || protocal_response_wait_ != SUBACK) return;
      subackHandler((msg_suback*)message_buffer_);
      break;

    case PINGREQ:
      pingreqHandler();
      break;

    case PINGRESP:
      if (!gateway_response_wait_ || protocal_response_wait_ != PINGRESP)
        return;
      pingrespHandler();
      break;

    case DISCONNECT:
      disconnectHandler((msg_disconnect*)message_buffer_);
      break;

#ifdef USE_QOS2
    case PUBREC:
      pubrecHandler((msg_pubqos2*)message_buffer_);
      break;

    case PUBREL:
      pubrelHandler((msg_pubqos2*)message_buffer_);
      break;

    case PUBCOMP:
      pubcompHandler((msg_pubqos2*)message_buffer_);
      break;
#endif

    default:
      SOPLOGLN(F("[ERROR] Not supported type."));
      return;
  }

  gateway_response_wait_ = false;
}

void Thing::broadcast() {
  message_header* hdr = reinterpret_cast<message_header*>(message_buffer_);
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
  // SOPLOGLN(F("Send packet!"));
  zbee_.send(zbee_tx_);
}

void Thing::unicast() {
  if (!gateway_ready_) {
    broadcast();
    return;
  }

  message_header* hdr = reinterpret_cast<message_header*>(message_buffer_);

  zbee_tx_ = ZBTxRequest(gateway_address_64_, message_buffer_, hdr->length);
  sendPacket();

  if (!gateway_response_wait_) {
    SOPLOG(F("[DEBUG] No gateway response\n"));
  }
}

void Thing::advertiseHandler(const msg_advertise* msg) {
  if (!gateway_ready_) {
    gateway_id_ = msg->gw_id;
    gateway_ready_ = true;
  }
}

void Thing::gwinfoHandler(const msg_gwinfo* msg) {
  if (!gateway_ready_) {
    gateway_address_64_ = zbee_rx_.getRemoteAddress64();
    gateway_address_16_ = zbee_rx_.getRemoteAddress16();
    gateway_id_ = msg->gw_id;
    gateway_ready_ = true;
  }
}

void Thing::connackHandler(const msg_connack* msg) {
  gateway_connected_ = true;
  if (connect_handler_ != NULL) connect_handler_();
}

void Thing::regackHandler(const msg_regack* msg) {
  if (msg->return_code == 0 && bswap(msg->message_id) == message_id_) {
    registered_id_ = bswap(msg->topic_id);
  }
}

void Thing::disconnectHandler(const msg_disconnect* msg) {
  if (gateway_connected_ && disconnect_handler_ != NULL) disconnect_handler_();
  gateway_connected_ = false;
  gateway_ready_ = false;
  middleware_registered_ = false;
}

void Thing::publishHandler(const msg_publish* msg) {
  SOPLOGLN("publishHandler");
  StaticJsonDocument<200> receive_doc;
  StaticJsonDocument<200> publish_doc;
  DeserializationError error;

  int success = -1;
  char* pTokPtr = NULL;
  char* t_name = NULL;
  char* t_args = NULL;

  return_code_t ret = REJECTED_INVALID_TOPIC_ID;
  uint16_t topic_id = bswap(msg->topic_id);

  if (topic_id == id_1001_) {
    SOPLOGLN("topic_id == id_1001_");
    snprintf(receive_buffer, MAX_BUFFER_SIZE, "%s", msg->data);

    error = deserializeJson(receive_doc, receive_buffer);
    if (error) {
      SOPLOG(F("deserializeJson() failed: "));
      SOPLOGLN(error.f_str());
      return;
    }

    SOPLOGLN("receive_doc: %d", receive_doc["error"]);

    if (receive_doc["error"] == 0) {
      middleware_registered_ = true;
    } else if (receive_doc["error"] == -4) {
      SOPLOGLN(F("Duplicated Thing error message from middleware."));
      middleware_registered_ = true;
    }

#ifdef USE_QOS2
    publish_process_ = true;
    if (msg->flags & FLAG_QOS_2) {
      ret = ACCEPTED;
      pubrec(msg);
    }
    publish_process_ = false;
#endif
    return;
  }

  for (int i = 0; i < num_functions_; i++) {
    if (topic_id == functions_[i]->id_1003()) {
      publish_process_ = true;
      // safe cpy
      strncpy(receive_buffer, msg->data, MAX_BUFFER_SIZE);

#ifdef USE_QOS2
      if (msg->flags & FLAG_QOS_2) {
        ret = ACCEPTED;
        pubrec(msg);
      }
#endif

      publish_process_ = false;

      t_name = strtok_r(receive_buffer, ":", &pTokPtr);  // dd:dd:sds
      t_args = strtok_r(NULL, "#", &pTokPtr);

      switch (functions_[i]->getReturnType()) {
        case VOID:
        case INTEGER:
        case DOUBLE:
        case BOOL:
          functions_[i]->Execute(t_args, &success);
          break;
        default:
          success = -7;
          break;  // cannot be occured
      }

      snprintf(publish_buffer, MAX_BUFFER_SIZE,
               "{\"scenario\" : \"%s\" , \"error\" : %d}", t_name, success);
      // snprintf(buffer, MAX_BUFFER_SIZE + 60,
      //          "{\"scenario\" : \"%s\" , \"error\" : %d , \"return_type\" : "
      //          "\"%s\" , \"return_value\" : \"%s\" }",
      //          t_name, success, "", "");
      SOPLOG(F("buffer : "));
      SOPLOGLN(publish_buffer);
      publish(QOS_FLAG, functions_[i]->id_2004(), publish_buffer,
              strlen(publish_buffer));

      return;
    }
  }
}

void Thing::subackHandler(const msg_suback* msg) {}

void Thing::searchgw(const uint8_t radius) {
  msg_searchgw* msg = reinterpret_cast<msg_searchgw*>(message_buffer_);

  msg->length = sizeof(msg_searchgw);
  msg->type = SEARCHGW;
  msg->radius = radius;

  broadcast();
  gateway_response_wait_ = true;
  protocal_response_wait_ = GWINFO;
}

void Thing::connect(const uint8_t flags, const uint16_t duration,
                    const char* client_id_) {
  msg_connect* msg = reinterpret_cast<msg_connect*>(message_buffer_);

  msg->length = sizeof(msg_connect) + strlen(client_id_);
  msg->type = CONNECT;
  msg->flags = flags;
  msg->protocol_id = PROTOCOL_ID;
  msg->duration = bswap(duration);
  strcpy(msg->client_id, client_id_);

  unicast();
  gateway_connected_ = false;
  gateway_response_wait_ = true;
  protocal_response_wait_ = CONNACK;
}

void Thing::disconnect(const uint16_t duration) {
  msg_disconnect* msg = reinterpret_cast<msg_disconnect*>(message_buffer_);

  msg->length = sizeof(message_header);
  msg->type = DISCONNECT;

  if (duration > 0) {
    msg->length += sizeof(msg_disconnect);
    msg->duration = bswap(duration);
  }

  unicast();
  gateway_response_wait_ = true;
}

bool Thing::registerTopic(const char* name) {
  if (!gateway_response_wait_) {
    message_id_++;

    registered_id_ = -1;
    msg_register* msg = reinterpret_cast<msg_register*>(message_buffer_);

    msg->length = sizeof(msg_register) + strlen(name);
    msg->type = REGISTER;
    msg->topic_id = 0;
    msg->message_id = bswap(message_id_);
    strcpy(msg->topic_name, name);

    unicast();
    gateway_response_wait_ = true;
    protocal_response_wait_ = REGACK;
    return true;
  } else {
    SOPLOGLN(F("gateway_response_wait_ is false error!"));
  }

  return false;
}

void Thing::publish(const uint8_t flags, const uint16_t topicId,
                    const void* data, const uint8_t data_len) {
  // Author: thsvkd
  // in QoS 0, message_id_ have to set to 0
  message_id_ = 0;

  msg_publish* msg = reinterpret_cast<msg_publish*>(message_buffer_);

  msg->length = sizeof(msg_publish) + data_len;
  msg->type = PUBLISH;
  msg->flags = flags;
  msg->topic_id = bswap(topicId);
  msg->message_id = bswap(message_id_);
  memcpy(msg->data, data, data_len);

  unicast();

  if ((flags & QOS_MASK) == FLAG_QOS_2) {
    gateway_response_wait_ = true;
    protocal_response_wait_ = PUBREC;
    while (true) {
      ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
      if (protocal_response_wait_ == PUBREC) {
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

void Thing::subscribe(const uint8_t flags, const uint16_t topicId) {
  message_id_++;

  msg_subscribe* msg = reinterpret_cast<msg_subscribe*>(message_buffer_);

  msg->length = sizeof(msg_subscribe);
  msg->type = SUBSCRIBE;
  // msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_PREDEFINED_ID;
  msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_NAME;
  msg->message_id = bswap(message_id_);
  msg->topic_id = bswap(topicId);

  unicast();

  if ((flags & QOS_MASK) == FLAG_QOS_1 || (flags & QOS_MASK) == FLAG_QOS_2) {
    gateway_response_wait_ = true;
    protocal_response_wait_ = SUBACK;
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

void Thing::print_message_buffer_(void* buf, int length) {
  // print message_buffer_ BYTE *buf to *(buf + length)
  char* tmp = (char*)buf;
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
void Thing::subscribe(const uint8_t flags, const char* name) {
  message_id_++;

  msg_subscribe* msg = reinterpret_cast<msg_subscribe*>(message_buffer_);

  msg->length = sizeof(msg_subscribe) + strlen(name) - sizeof(uint16_t);
  msg->type = SUBSCRIBE;
  // msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_PREDEFINED_ID;
  msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_NAME;
  msg->message_id = bswap(message_id_);
  msg->topic_id = 0;
  strcpy(msg->topic_name, name);

  unicast();

  if ((flags & QOS_MASK) == FLAG_QOS_1 || (flags & QOS_MASK) == FLAG_QOS_2) {
    gateway_response_wait_ = true;
    protocal_response_wait_ = SUBACK;
  }
}

void Thing::pingresp(int flag) {
  message_header* msg = reinterpret_cast<message_header*>(message_buffer_);
  msg->length = sizeof(message_header);
  msg->type = PINGRESP;

  if (flag == 0)
    unicast();
  else
    broadcast();
}

void Thing::unsubscribe(const uint8_t flags, const char* name) {
  message_id_++;

  msg_unsubscribe* msg = reinterpret_cast<msg_unsubscribe*>(message_buffer_);

  msg->length = sizeof(msg_unsubscribe);
  msg->type = UNSUBSCRIBE;
  // msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_PREDEFINED_ID;
  msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_NAME;
  msg->message_id = bswap(message_id_);
  msg->topic_id = 0;
  strcpy(msg->topic_name, name);

  unicast();

  if ((flags & QOS_MASK) == FLAG_QOS_1 || (flags & QOS_MASK) == FLAG_QOS_2) {
    gateway_response_wait_ = true;
    protocal_response_wait_ = UNSUBACK;
  }
}

void Thing::unsubscribe(const uint8_t flags, const uint16_t topicId) {
  message_id_++;

  msg_unsubscribe* msg = reinterpret_cast<msg_unsubscribe*>(message_buffer_);

  msg->length = sizeof(msg_unsubscribe);
  msg->type = UNSUBSCRIBE;
  // msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_PREDEFINED_ID;
  msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_NAME;
  msg->message_id = bswap(message_id_);
  msg->topic_id = bswap(topicId);

  unicast();

  if ((flags & QOS_MASK) == FLAG_QOS_1 || (flags & QOS_MASK) == FLAG_QOS_2) {
    gateway_response_wait_ = true;
    protocal_response_wait_ = UNSUBACK;
  }
}

void Thing::pingreq() {
  msg_pingreq* msg = reinterpret_cast<msg_pingreq*>(message_buffer_);
  msg->length = sizeof(msg_pingreq) + strlen(client_id_);
  msg->type = PINGREQ;
  strcpy(msg->client_id, client_id_);

  unicast();
  last_ping_ = gateway_response_wait_ = true;
  protocal_response_wait_ = PINGRESP;
}

void Thing::pingreqHandler() {
  if (zbee_rx_.getRemoteAddress16() == gateway_address_16_)
    pingresp(0);
  else
    pingresp(1);
}

void Thing::pingrespHandler() {}

#ifdef USE_QOS2
void Thing::pubrec(const msg_publish* recv_msg) {
  gateway_response_wait_ = true;
  protocal_response_wait_ = PUBREL;
  while (protocal_response_wait_ == PUBREL) {
    msg_pubqos2* msg = reinterpret_cast<msg_pubqos2*>(message_buffer_);
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

void Thing::pubrel(const msg_pubqos2* recv_msg) {
  gateway_response_wait_ = true;
  protocal_response_wait_ = PUBCOMP;
  while (protocal_response_wait_ == PUBCOMP) {
    msg_pubqos2* msg = reinterpret_cast<msg_pubqos2*>(message_buffer_);
    msg->length = sizeof(msg_pubqos2);
    msg->type = PUBREL;
    msg->message_id = recv_msg->message_id;
    unicast();
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
  }
}

void Thing::pubcomp(const msg_pubqos2* recv_msg) {
  msg_pubqos2* msg = reinterpret_cast<msg_pubqos2*>(message_buffer_);
  msg->length = sizeof(msg_pubqos2);
  msg->type = PUBCOMP;
  msg->message_id = recv_msg->message_id;

  unicast();
}

void Thing::pubrecHandler(const msg_pubqos2* msg) { pubrel(msg); }

void Thing::pubrelHandler(const msg_pubqos2* msg) {
  gateway_response_wait_ = false;
  protocal_response_wait_ = PUBLISH;
  pubcomp(msg);
}

void Thing::pubcompHandler(const msg_pubqos2* msg) {
  gateway_response_wait_ = false;
  protocal_response_wait_ = PUBLISH;
}

#endif
