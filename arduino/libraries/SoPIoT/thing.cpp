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
      zbee_() {
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

void Thing::XbeeSetup() {
  XbeeAtCommand((uint8_t*)"ID");
  SOPLOGLN(F("ID : "));
  PrintXbeePacket((char*)zbee_atcommand_result_);
  // delay(500);

  XbeeAtCommand((uint8_t*)"OP");
  SOPLOGLN(F("OP : "));
  PrintXbeePacket((char*)zbee_atcommand_result_);
  // delay(500);

  uint8_t id = 0x01;
  XbeeAtCommand((uint8_t*)"ID", &id, sizeof(id));
  SOPLOGLN(F("ID : "));
  PrintXbeePacket((char*)zbee_atcommand_result_);
  // delay(500);
}

void Thing::Setup() {
  SOPLOGLN(F("==== Small Thing start ===="));
  SOPLOGLN(F("Start to get clientID"));
  GenerateClientId();
  SOPLOGLN(F("ClientID : %s"), client_id_);

  SOPLOGLN(F("==== Setup SEARCHGW ===="));

  while (!GatewayReady()) {
    Searchgw(SEARCH_RADIUS);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
  }
  SOPLOGLN(F("Gateway ID : %d"), gateway_id_);

  SOPLOGLN(F("==== Setup CONNECT ===="));
  while (!GatewayConnected()) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, "%s", client_id_);
    Connect(0, 50, publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
  }
  SOPLOGLN(F("CONNECT success"));

  // Start REGISTER & SUBSCRIBE
  SOPLOGLN(F("==== Setup REGISTER & SUBSCRIBE ===="));
  registered_id_ = UINT16_MAX;
  while (registered_id_ == UINT16_MAX) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, MT1001, client_id_);  // id_1001_
    RegisterTopic(publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    id_1001_ = registered_id_;
  }
  SOPLOGLN(F("REGISTER Topic %s, ID : %d"), publish_buffer, id_1001_);

  Subscribe(QOS_FLAG, publish_buffer);
  ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
  SOPLOGLN(F("SUBSCRIBE Topic %s"), publish_buffer);

  // Author: thsvkd
  // Remove PingReq part

  registered_id_ = UINT16_MAX;
  while (registered_id_ == UINT16_MAX) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, TM2001, client_id_);  // id_2001_
    RegisterTopic(publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    id_2001_ = registered_id_;
  }
  SOPLOGLN(F("REGISTER Topic %s, ID : %d"), publish_buffer, id_2001_);

  registered_id_ = UINT16_MAX;
  while (registered_id_ == UINT16_MAX) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, TM2002, client_id_);  // id_2002_
    RegisterTopic(publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    id_2002_ = registered_id_;
  }
  SOPLOGLN(F("REGISTER Topic %s, ID : %d"), publish_buffer, id_2002_);

  // Publish(QOS_FLAG, registered_id_, "{}", 3);
  // SOPLOGLN(F("Send UNREGISTER for clean start"));

  registered_id_ = UINT16_MAX;
  while (registered_id_ == UINT16_MAX) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, TM2003, client_id_);  // id_2003_
    RegisterTopic(publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    id_2003_ = registered_id_;
  }
  SOPLOGLN(F("REGISTER Topic %s, ID : %d"), publish_buffer, id_2003_);

  SOPLOGLN(F("==== Setup REGISTER *New* Topics for Small Thing ===="));

  registered_id_ = UINT16_MAX;
  while (registered_id_ == UINT16_MAX) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, TM2010, client_id_);  // id_2010_
    RegisterTopic(publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    id_2010_ = registered_id_;
  }
  SOPLOGLN(F("REGISTER Topic %s, ID : %d"), publish_buffer, id_2010_);

  registered_id_ = UINT16_MAX;
  while (registered_id_ == UINT16_MAX) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, TM2011, client_id_);  // id_2011_
    RegisterTopic(publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    id_2011_ = registered_id_;
    SOPLOGLN(F("id_2011_ : %d"), id_2011_);
  }
  SOPLOGLN(F("REGISTER Topic %s, ID : %d"), publish_buffer, id_2011_);

  registered_id_ = UINT16_MAX;
  while (registered_id_ == UINT16_MAX) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, TM2012, client_id_);  // id_2012_
    RegisterTopic(publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    id_2012_ = registered_id_;
  }
  SOPLOGLN(F("REGISTER Topic %s, ID : %d"), publish_buffer, id_2012_);

  registered_id_ = UINT16_MAX;
  while (registered_id_ == UINT16_MAX) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, TM2013, client_id_);  // id_2013_
    RegisterTopic(publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    id_2013_ = registered_id_;
  }
  SOPLOGLN(F("REGISTER Topic %s, ID : %d"), publish_buffer, id_2013_);

  registered_id_ = UINT16_MAX;
  while (registered_id_ == UINT16_MAX) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, TM2014, client_id_);  // id_2014_
    RegisterTopic(publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    id_2014_ = registered_id_;
  }
  SOPLOGLN(F("REGISTER Topic %s, ID : %d"), publish_buffer, id_2014_);

  registered_id_ = UINT16_MAX;
  while (registered_id_ == UINT16_MAX) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, TM2015, client_id_);  // id_2015_
    RegisterTopic(publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    id_2015_ = registered_id_;
  }
  SOPLOGLN(F("REGISTER Topic %s, ID : %d"), publish_buffer, id_2015_);

  registered_id_ = UINT16_MAX;
  while (registered_id_ == UINT16_MAX) {
    snprintf(publish_buffer, MAX_BUFFER_SIZE, TM2016, client_id_);  // id_2016_
    RegisterTopic(publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    id_2016_ = registered_id_;
  }
  SOPLOGLN(F("REGISTER Topic %s, ID : %d"), publish_buffer, id_2016_);

  // Start Value
  SOPLOGLN(F("==== Setup REGISTER Value Topics ===="));
  for (int i = 0; i < num_values_; i++) {
    registered_id_ = UINT16_MAX;
    while (values_[i]->SetPublishID(registered_id_) == UINT16_MAX) {
      snprintf(publish_buffer, MAX_BUFFER_SIZE, COMMON0000, client_id_,
               values_[i]->GetName());
      RegisterTopic(publish_buffer);
      ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    }
    SOPLOGLN(F("REGISTER Topic %s, ID : %d"), publish_buffer,
             values_[i]->GetPublishID());
    values_[i]->AddTag(client_id_);
  }

  // Start Function
  SOPLOGLN(F("==== Setup REGISTER Function Topics ===="));
  for (int i = 0; i < num_functions_; i++) {
    registered_id_ = UINT16_MAX;
    while ((functions_[i]->SetID1003(registered_id_)) == UINT16_MAX) {
      snprintf(publish_buffer, MAX_BUFFER_SIZE, MT1003,
               functions_[i]->GetName(), client_id_);
      RegisterTopic(publish_buffer);
      ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    }
    SOPLOGLN(F("REGISTER Topic %s, ID : %d"), publish_buffer,
             functions_[i]->GetID1003());

    Subscribe(QOS_FLAG, publish_buffer);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    SOPLOGLN(F("SUBSCRIBE Topic %s"), publish_buffer);

    registered_id_ = UINT16_MAX;
    while ((functions_[i]->SetID2004(registered_id_)) == UINT16_MAX) {
      snprintf(publish_buffer, MAX_BUFFER_SIZE, TM2004,
               functions_[i]->GetName(), client_id_);
      RegisterTopic(publish_buffer);
      ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
    }
    SOPLOGLN(F("REGISTER Topic %s, ID : %d"), publish_buffer,
             functions_[i]->GetID2004());
    functions_[i]->AddTag(client_id_);
  }

  PrintTopicID();
  // TestPublish();

  RegisterToMIddleware();

  SendAliveMessageNoCond();
  SendInitialValueNoCond();
  SOPLOGLN(F("==== Registeration to the Middleware Finish ===="));
}

void Thing::PrintTags() {
  for (int i = 0; i < num_values_; i++) {
    SOPLOGLN(F("tag num: %d"), values_[i]->GetTagNum());
    for (int j = 0; j < values_[i]->GetTagNum(); j++) {
      SOPLOGLN(F("values_[%d] tag[%d]: %s"), i, j,
               values_[i]->GetTag(j)->GetName());
    }
  }

  for (int i = 0; i < num_functions_; i++) {
    SOPLOGLN(F("tag num: %d"), functions_[i]->GetTagNum());
    for (int j = 0; j < functions_[i]->GetTagNum(); j++) {
      SOPLOGLN(F("functions_[%d] tag[%d]: %s"), i, j,
               functions_[i]->GetTag(j)->GetName());
    }
  }
}

void Thing::PrintTopicID() {
  SOPLOGLN(F("Topic \"MT/REGACK/%s\": %d"), client_id_, id_1001_);
  SOPLOGLN(F("Topic \"TM/REGISTER/%s\": %d"), client_id_, id_2001_);
  SOPLOGLN(F("Topic \"TM/UNREGISTER/%s\": %d"), client_id_, id_2002_);
  SOPLOGLN(F("Topic \"TM/ALIVE/%s\": %d"), client_id_, id_2003_);
  SOPLOGLN(F("Topic \"TM/SN/REGISTER/VALUE/%s\": %d"), client_id_, id_2010_);
  SOPLOGLN(F("Topic \"TM/SN/REGISTER/VALUETAG/%s\": %d"), client_id_, id_2011_);
  SOPLOGLN(F("Topic \"TM/SN/REGISTER/FUNCTION/%s\": %d"), client_id_, id_2012_);
  SOPLOGLN(F("Topic \"TM/SN/REGISTER/FUNCTIONTAG/%s\": %d"), client_id_,
           id_2013_);
  SOPLOGLN(F("Topic \"TM/SN/REGISTER/ARGUMENT/%s\": %d"), client_id_, id_2014_);
  SOPLOGLN(F("Topic \"TM/SN/REGISTER/ALIVECYCLE/%s\": %d"), client_id_,
           id_2015_);
  SOPLOGLN(F("Topic \"TM/SN/REGISTER/FINISH/%s\": %d"), client_id_, id_2016_);

  for (int i = 0; i < num_values_; i++) {
    SOPLOGLN(F("Topic \"%s/%s\" %d"), client_id_, values_[i]->GetName(),
             values_[i]->GetPublishID());
  }

  for (int i = 0; i < num_functions_; i++) {
    SOPLOGLN(F("Topic \"MT/SN/%s/%s\": %d"), functions_[i]->GetName(),
             client_id_, functions_[i]->GetID1003());
    SOPLOGLN(F("Topic \"TM/RESULT/FUNCTION/%s/%s\": %d"),
             functions_[i]->GetName(), client_id_, functions_[i]->GetID2004());
  }
}

void Thing::PrintXbeePacket(char* buf) {
  SOPLOG(F("Xbee packet : "));
  for (int i = 0; i < MAX_BUFFER_SIZE; i++) {
    SOPLOG(F("%02X "), buf[i]);
  }
  SOPLOGLN(F(""));
}

void Thing::PrintXbeePacket(char* buf, int length) {
  SOPLOG(F("Xbee packet : "));
  for (int i = 0; i < length; i++) {
    SOPLOG(F("%02X "), buf[i]);
  }
  SOPLOGLN(F(""));
}

void Thing::TestPublish() {
  while (1) {
    Publish(QOS_FLAG, id_1001_, "id_1001_ test", strlen("id_1001_ test"));
    Publish(QOS_FLAG, id_2001_, "id_2001_ test", strlen("id_2001_ test"));
    Publish(QOS_FLAG, id_2002_, "id_2002_ test", strlen("id_2002_ test"));
    Publish(QOS_FLAG, id_2003_, "id_2003_ test", strlen("id_2003_ test"));
    Publish(QOS_FLAG, id_2010_, "id_2010_ test", strlen("id_2010_ test"));
    Publish(QOS_FLAG, id_2011_, "id_2011_ test", strlen("id_2011_ test"));
    Publish(QOS_FLAG, id_2012_, "id_2012_ test", strlen("id_2012_ test"));
    Publish(QOS_FLAG, id_2013_, "id_2013_ test", strlen("id_2013_ test"));
    Publish(QOS_FLAG, id_2014_, "id_2014_ test", strlen("id_2014_ test"));
    Publish(QOS_FLAG, id_2015_, "id_2015_ test", strlen("id_2015_ test"));
    Publish(QOS_FLAG, id_2016_, "id_2016_ test", strlen("id_2016_ test"));

    for (int i = 0; i < num_values_; i++) {
      Publish(QOS_FLAG, values_[i]->GetPublishID(), "values_ test",
              strlen("values_ test"));
    }

    for (int i = 0; i < num_functions_; i++) {
      SOPLOGLN(F("callback_function_[%d] id_1003: %d"), i,
               functions_[i]->GetID1003());
      SOPLOGLN(F("callback_function_[%d] id_2004: %d"), i,
               functions_[i]->GetID2004());
    }

    // delay(2000);
  }
}

void Thing::XbeeOff() {
  // pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  delay(10);
}
void Thing::XbeeOn() {
  // pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  delay(10);
}

// TODO: pub_period is needed?
void Thing::Loop(int pub_period) {
  bool time_passed = false;
  bool changed = false;

  // Wait for Response about Value
  SendAliveMessage();
  if (!(num_values_ < 2 && num_functions_ == 0)) {
    ReadZbeeIfAvailable();
  }

  for (uint8_t i = 0; i < num_values_; i++) {
    time_passed = CompareTimeStamp(values_[i]);
    if (time_passed) {
      changed = values_[i]->GetValuePublishJson(publish_buffer);
      if (changed) {
        SOPLOGLN(F("pub_id: %d, buffer: %s"), values_[i]->GetPublishID(),
                 publish_buffer);
        Publish(QOS_FLAG, values_[i]->GetPublishID(), publish_buffer,
                strlen(publish_buffer));
        if ((num_values_ < 2 && num_functions_ == 0)) {
          int cycle = values_[0]->GetPublishCycle();
          SOPLOGLN(F("sleep about %d ms"), cycle);
          // SOPLOGLN(F("1time: %d us"), (unsigned int)micros());
          XbeeOff();
          // SOPLOGLN(F("2time: %d us"), (unsigned int)micros());
#if !(defined(ARDUINO_ARCH_MBED) || defined(ARDUINO_NANO_RP2040_CONNECT) || \
      defined(AVR_MEGA2560))
          sleep_time_ms += SoPSleep(cycle);
#endif
          // SOPLOGLN(F("3time: %d us"), (unsigned int)micros());
          XbeeOn();
          // SOPLOGLN(F("4time: %d us"), (unsigned int)micros());
        }
      }
    }
    if (!(num_values_ < 2 && num_functions_ == 0)) {
      ReadZbeeIfAvailable();
    }
  }
}

//----------------------------------------
// Private Functions
//----------------------------------------

void Thing::RegisterToMIddleware() {
  for (int i = 0; i < num_values_; i++) {
    values_[i]->GetRegisterPublishData(publish_buffer);
    Publish(QOS_FLAG, id_2010_, publish_buffer, strlen(publish_buffer));
    SOPLOGLN(F("Send Value %s : %s"), values_[i]->GetName(), publish_buffer);
    delay(50);

    for (int j = 0; j < values_[i]->GetTagNum(); j++) {
      values_[i]->GetTag(j)->GetRegisterPublishData(publish_buffer);
      Publish(QOS_FLAG, id_2011_, publish_buffer, strlen(publish_buffer));
      SOPLOGLN(F("Send ValueTag %s : %s"), values_[i]->GetTag(j)->GetName(),
               publish_buffer);
      delay(50);
    }
  }

  SOPLOGLN(F("num_functions_ : %d"), num_functions_);
  for (int i = 0; i < num_functions_; i++) {
    functions_[i]->GetRegisterPublishData(publish_buffer);
    Publish(QOS_FLAG, id_2012_, publish_buffer, strlen(publish_buffer));
    SOPLOGLN(F("Send Function %s : %s"), functions_[i]->GetName(),
             publish_buffer);
    delay(50);

    for (int j = 0; j < functions_[i]->GetTagNum(); j++) {
      functions_[i]->GetTag(j)->GetRegisterPublishData(publish_buffer);
      Publish(QOS_FLAG, id_2013_, publish_buffer, strlen(publish_buffer));
      SOPLOGLN(F("Send FunctionTag %s : %s"),
               functions_[i]->GetTag(j)->GetName(), publish_buffer);
      delay(50);
    }

    for (int j = 0; j < functions_[i]->GetArgumentNum(); j++) {
      functions_[i]->GetArgument(j)->GetRegisterPublishData(publish_buffer);
      Publish(QOS_FLAG, id_2014_, publish_buffer, strlen(publish_buffer));
      SOPLOGLN(F("Send Argument %s : %s"),
               functions_[i]->GetArgument(j)->GetName(), publish_buffer);
      delay(50);
    }
  }

  delay(500);
  snprintf(publish_buffer, MAX_BUFFER_SIZE, "%d", alive_cycle_);
  Publish(QOS_FLAG, id_2015_, publish_buffer, strlen(publish_buffer));
  SOPLOGLN(F("Send AliveCycle : %s"), publish_buffer);
  delay(50);

  while (!middleware_registered_) {
    Publish(QOS_FLAG, id_2016_, "{}", strlen("{}"));
    SOPLOGLN(F("Send Finish : %s"), publish_buffer);
    delay(50);
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
  }
}

void Thing::SetSerial(Stream& serial) { zbee_.setSerial(serial); }

uint8_t* Thing::XbeeAtCommand(uint8_t* cmd) {
  AtCommandRequest atRequest = AtCommandRequest(cmd);
  AtCommandResponse atResponse = AtCommandResponse();

  while (1) {
    zbee_.send(atRequest);

    if (zbee_.readPacket(READ_ZBEE_TIMEOUT)) {
      if (zbee_.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
        zbee_.getResponse().getAtCommandResponse(atResponse);
        if (atResponse.isOk()) {
          memcpy(zbee_atcommand_result_, atResponse.getValue(),
                 atResponse.getValueLength());
          zbee_atcommand_result_[atResponse.getValueLength()] = '\0';
          return zbee_atcommand_result_;
        }
      }
    } else {
      SOPLOGLN(F("Serial High READ ERROR!!"));
      delay(250);
    }
  }
}

uint8_t* Thing::XbeeAtCommand(uint8_t* cmd, uint8_t* cmdValue,
                              uint8_t cmdValueLength) {
  AtCommandRequest atRequest = AtCommandRequest(cmd, cmdValue, cmdValueLength);
  AtCommandResponse atResponse = AtCommandResponse();

  while (1) {
    zbee_.send(atRequest);

    if (zbee_.readPacket(READ_ZBEE_TIMEOUT)) {
      if (zbee_.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
        zbee_.getResponse().getAtCommandResponse(atResponse);
        if (atResponse.isOk()) {
          memcpy(zbee_atcommand_result_, atResponse.getValue(),
                 atResponse.getValueLength());
          zbee_atcommand_result_[atResponse.getValueLength()] = '\0';
          return zbee_atcommand_result_;
        }
      }
    } else {
      SOPLOGLN(F("Serial High READ ERROR!!"));
      delay(250);
    }
  }
}

void Thing::GetMacAddress() {
  while (1) {
    memcpy(mac_address_ + 4, XbeeAtCommand((uint8_t*)"SL"), 4);
    delay(50);
    memcpy(mac_address_, XbeeAtCommand((uint8_t*)"SH"), 4);
    delay(50);

    if (memcmp(mac_address_ + 4, xbee_low_address, 4) != 0 &&
        memcmp(mac_address_, mac_address_ + 4, 4) != 0)
      break;
  }
}

uint8_t* Thing::GetPanID() {
  XbeeAtCommand((uint8_t*)"OP");
  return zbee_atcommand_result_;
}

void Thing::SetPanID(uint8_t* id) {
  XbeeAtCommand((uint8_t*)"ID", id, sizeof(id));

  while (memcmp(GetPanID(), id, sizeof(id)) != 0) {
    SOPLOGLN(F("GetPanID : "));
    PrintXbeePacket((char*)zbee_atcommand_result_);
    delay(50);
  }

  SOPLOGLN(F("SetPanID success!"));
}

void Thing::GenerateClientId() {
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

bool Thing::CompareTimeStamp(Value* t) {
  bool time_passed = false;
  unsigned long curr_time = 0;
  unsigned long diff_time = 0;
  unsigned long last_sent_time = 0;

  last_sent_time = t->GetLastSentTime() + sleep_time_ms;

  // set true for the initial case
  if (last_sent_time == 0) {
    t->SetLastSentTime();
    return true;
  }

  curr_time = millis();

  diff_time = curr_time - last_sent_time;

  // millis() goes back to zero after approximately 50 days
  if ((diff_time < 0) || (diff_time >= (unsigned long)t->GetPublishCycle())) {
    t->SetLastSentTime();
    time_passed = true;
  } else {
    time_passed = false;
  }

  return time_passed;
}

// Publish(QOS_FLAG, id_2003_, NULL, 0);
void Thing::SendAliveMessage() {
  static unsigned long curr_time = 0;
  static unsigned long last_sent_time = 0;
  unsigned long diff_time = 0;
  char* pszDummy = (char*)"AliveCycleReached";

  curr_time = millis() + sleep_time_ms;

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

    Publish(QOS_FLAG, id_2003_, pszDummy, strlen(pszDummy));
    last_sent_time = curr_time;
  }
  return;
}

void Thing::SendAliveMessageNoCond() {
  char* pszDummy = (char*)"AliveNoCond";

  SOPLOGLN(F("init alive"));
  Publish(QOS_FLAG, id_2003_, pszDummy, strlen(pszDummy));
}

void Thing::SendInitialValueNoCond() {
  for (uint8_t i = 0; i < num_values_; i++) {
    // Value to Json String
    values_[i]->GetValuePublishJson(publish_buffer);
    // Publish values
    SOPLOGLN(F("init Value --> pub_id: %d, buffer: %s"),
             values_[i]->GetPublishID(), publish_buffer);
    Publish(QOS_FLAG, values_[i]->GetPublishID(), publish_buffer,
            strlen(publish_buffer));
  }
}

//////////////////////////////////////////////////////////////////////////

void Thing::ReadZbeeTimeout(int timeout) {
  while (zbee_.readPacket(timeout)) {
    if (zbee_.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      ZBTxStatusResponse txStatus;
      zbee_.getResponse().getZBTxStatusResponse(txStatus);
      if (!txStatus.isSuccess()) {
        SOPLOGLN(F("[ERROR] Zigbee Send Fail."));
      }
    } else if (zbee_.getResponse().getApiId() == ZB_RX_RESPONSE) {
      // SOPLOGLN(F("Zigbee receive success!"));
      zbee_.getResponse().getZBRxResponse(zbee_rx_);
      // PrintXbeePacket((char*)zbee_rx_.getData());
      ParseMQTTSNStream((char*)zbee_rx_.getData(), zbee_rx_.getDataLength());
      break;
    } else if (zbee_.getResponse().isError()) {
      SOPLOGLN(F("[ERROR] ZigBee response error."));
    } else {
      SOPLOGLN(F("[ERROR] Unexpected response."));
      char* response = (char*)zbee_.getResponse().getApiId();
      PrintXbeePacket(response);
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

  memset(message_buffer_, 0, MAX_BUFFER_SIZE);
  memcpy(message_buffer_, (const void*)buf, MAX_BUFFER_SIZE);
  SOPLOGLN(F("response_message->type : %d"), response_message->type);
  SOPLOGLN(F("gateway_response_wait_ : %d"), gateway_response_wait_);

  switch (response_message->type) {
    case ADVERTISE:
      if (!gateway_response_wait_) return;
      AdvertiseHandler((msg_advertise*)message_buffer_);
      break;

    case GWINFO:
      if (!gateway_response_wait_) return;
      GwinfoHandler((msg_gwinfo*)message_buffer_);
      break;

    case CONNACK:
      if (!gateway_response_wait_) return;
      ConnackHandler((msg_connack*)message_buffer_);
      break;

    case REGACK:
      if (!gateway_response_wait_) return;
      RegackHandler((msg_regack*)message_buffer_);
      break;

    case PUBLISH:
      if (!gateway_response_wait_) return;
      PublishHandler((msg_publish*)message_buffer_);
      break;

    case SUBACK:
      if (!gateway_response_wait_) return;
      SubackHandler((msg_suback*)message_buffer_);
      break;

    case PINGREQ:
      PingreqHandler();
      break;

    case PINGRESP:
      if (!gateway_response_wait_) return;
      PingrespHandler();
      break;

    case DISCONNECT:
      DisconnectHandler((msg_disconnect*)message_buffer_);
      break;

#ifdef USE_QOS2
    case PUBREC:
      PubrecHandler((msg_pubqos2*)message_buffer_);
      break;

    case PUBREL:
      PubrelHandler((msg_pubqos2*)message_buffer_);
      break;

    case PUBCOMP:
      PubcompHandler((msg_pubqos2*)message_buffer_);
      break;
#endif

    default:
      SOPLOGLN(F("[ERROR] Unvaild Response_message type! "
                 "response_message->type : %d"),
               response_message->type);
      return;
  }
}

void Thing::Broadcast() {
  message_header* hdr = reinterpret_cast<message_header*>(message_buffer_);
  uint16_t addr16 = ZB_BROADCAST_ADDRESS;
  XBeeAddress64 addr64 = XBeeAddress64(0, 0xffff);

  // TODO(ikess): check what is diffrence between ZB_TX_UNICAST,
  // ZB_TX_BROADCAST??
  zbee_tx_ = ZBTxRequest(addr64, addr16, SEARCH_RADIUS, ZB_TX_UNICAST,
                         message_buffer_, hdr->length, DEFAULT_FRAME_ID);
  // zbee_tx_ = ZBTxRequest(addr64, message_buffer_, hdr->length);

  SendPacket();
}

void Thing::SendPacket() {
  // SOPLOGLN(F("Send packet!"));
  // PrintXbeePacket((char*)message_buffer_);
  zbee_.send(zbee_tx_);
}

void Thing::Unicast() {
  if (!gateway_ready_) {
    Broadcast();
    return;
  }

  message_header* hdr = reinterpret_cast<message_header*>(message_buffer_);

  zbee_tx_ = ZBTxRequest(gateway_address_64_, message_buffer_, hdr->length);
  SendPacket();
}

void Thing::AdvertiseHandler(const msg_advertise* msg) {
  if (!gateway_ready_) {
    gateway_id_ = msg->gw_id;
    gateway_ready_ = true;
    gateway_response_wait_ = false;
  }
}

void Thing::GwinfoHandler(const msg_gwinfo* msg) {
  if (!gateway_ready_) {
    gateway_address_64_ = zbee_rx_.getRemoteAddress64();
    gateway_address_16_ = zbee_rx_.getRemoteAddress16();
    gateway_id_ = msg->gw_id;
    gateway_ready_ = true;
    gateway_response_wait_ = false;
  }
}

void Thing::ConnackHandler(const msg_connack* msg) {
  gateway_connected_ = true;
  gateway_response_wait_ = false;
  if (connect_handler_ != NULL) connect_handler_();
}

void Thing::RegackHandler(const msg_regack* msg) {
  if (msg->return_code == 0 && bswap(msg->message_id) == message_id_) {
    registered_id_ = bswap(msg->topic_id);
    gateway_response_wait_ = false;
  } else {
    SOPLOGLN(F("Message_id was not matched. msg->return_code: %d , "
               "bswap(msg->message_id): %d, message_id_: %d"),
             msg->return_code, bswap(msg->message_id), message_id_);
    gateway_response_wait_ = true;
  }
}

void Thing::DisconnectHandler(const msg_disconnect* msg) {
  if (gateway_connected_ && disconnect_handler_ != NULL) disconnect_handler_();
  gateway_connected_ = false;
  gateway_ready_ = false;
  middleware_registered_ = false;
  gateway_response_wait_ = false;
}

void Thing::PublishHandler(const msg_publish* msg) {
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

  // when RAGACK
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
  }
  // when Function execute packet
  else {
    for (int i = 0; i < num_functions_; i++) {
      if (topic_id == functions_[i]->GetID1003()) {
        strncpy(receive_buffer, msg->data, MAX_BUFFER_SIZE);
        t_name = strtok_r(receive_buffer, ":", &pTokPtr);
        t_args = strtok_r(NULL, "#", &pTokPtr);

        switch (functions_[i]->GetReturnType()) {
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
        Publish(QOS_FLAG, functions_[i]->GetID2004(), publish_buffer,
                strlen(publish_buffer));
      }
    }
  }
  gateway_response_wait_ = true;
}

void Thing::SubackHandler(const msg_suback* msg) {}

void Thing::Searchgw(const uint8_t radius) {
  msg_searchgw* msg = reinterpret_cast<msg_searchgw*>(message_buffer_);
  memset(message_buffer_, 0, MAX_BUFFER_SIZE);

  msg->length = sizeof(msg_searchgw);
  msg->type = SEARCHGW;
  msg->radius = radius;

  Broadcast();
  gateway_response_wait_ = true;
}

void Thing::Connect(const uint8_t flags, const uint16_t duration,
                    const char* client_id_) {
  msg_connect* msg = reinterpret_cast<msg_connect*>(message_buffer_);
  memset(message_buffer_, 0, MAX_BUFFER_SIZE);

  msg->length = sizeof(msg_connect) + strlen(client_id_);
  msg->type = CONNECT;
  msg->flags = flags;
  msg->protocol_id = PROTOCOL_ID;
  msg->duration = bswap(duration);

  CHECK_STRING_LENGTH(__FILE__, __LINE__, msg->length, client_id_);
  strncpy(msg->client_id, client_id_, strlen(client_id_));

  Unicast();
  gateway_connected_ = false;
  gateway_response_wait_ = true;
}

void Thing::Disconnect(const uint16_t duration) {
  msg_disconnect* msg = reinterpret_cast<msg_disconnect*>(message_buffer_);
  memset(message_buffer_, 0, MAX_BUFFER_SIZE);

  msg->length = sizeof(message_header);
  msg->type = DISCONNECT;

  if (duration > 0) {
    msg->length += sizeof(msg_disconnect);
    msg->duration = bswap(duration);
  }

  Unicast();
  gateway_response_wait_ = true;
}

void Thing::RegisterTopic(const char* name) {
  // pass the function if message_id was not matched
  if (!gateway_response_wait_) {
    message_id_++;

    msg_register* msg = reinterpret_cast<msg_register*>(message_buffer_);
    memset(message_buffer_, 0, MAX_BUFFER_SIZE * sizeof(uint8_t));

    msg->length = sizeof(msg_register) + strlen(name);
    msg->type = REGISTER;
    msg->topic_id = 0;
    msg->message_id = bswap(message_id_);

    CHECK_STRING_LENGTH(__FILE__, __LINE__, msg->length, name);
    strncpy(msg->topic_name, name, strlen(name));

    Unicast();
    gateway_response_wait_ = true;
    // SOPLOGLN(F("wait for message_id_ : %d"), message_id_);
  } else {
    // SOPLOGLN(F("wait for message_id_ : %d"), message_id_);
  }
}

void Thing::Publish(const uint8_t flags, const uint16_t topicId,
                    const void* data, const uint8_t data_len) {
  // Author: thsvkd
  // in QoS 0, message_id_ have to set to 0
  SOPLOGLN(F("publish"));
  message_id_ = 0;

  msg_publish* msg = reinterpret_cast<msg_publish*>(message_buffer_);
  memset(message_buffer_, 0, MAX_BUFFER_SIZE);

  msg->length = sizeof(msg_publish) + data_len;
  msg->type = PUBLISH;
  msg->flags = flags;
  msg->topic_id = bswap(topicId);
  msg->message_id = bswap(message_id_);
  memcpy(msg->data, data, data_len);

  Unicast();

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
        Unicast();
      } else
        break;
    }
  }
  gateway_response_wait_ = true;
}

void Thing::Subscribe(const uint8_t flags, const uint16_t topicId) {
  message_id_++;

  msg_subscribe* msg = reinterpret_cast<msg_subscribe*>(message_buffer_);
  memset(message_buffer_, 0, MAX_BUFFER_SIZE);

  msg->length = sizeof(msg_subscribe);
  msg->type = SUBSCRIBE;
  // msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_PREDEFINED_ID;
  msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_NAME;
  msg->message_id = bswap(message_id_);
  msg->topic_id = bswap(topicId);

  Unicast();

  if ((flags & QOS_MASK) == FLAG_QOS_1 || (flags & QOS_MASK) == FLAG_QOS_2) {
    gateway_response_wait_ = true;
    protocal_response_wait_ = SUBACK;
  }
}

void Thing::Subscribe(const uint8_t flags, const char* name) {
  message_id_++;

  msg_subscribe* msg = reinterpret_cast<msg_subscribe*>(message_buffer_);
  memset(message_buffer_, 0, MAX_BUFFER_SIZE);

  msg->length = sizeof(msg_subscribe) + strlen(name) - sizeof(uint16_t);
  msg->type = SUBSCRIBE;
  // msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_PREDEFINED_ID;
  msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_NAME;
  msg->message_id = bswap(message_id_);
  msg->topic_id = 0;

  CHECK_STRING_LENGTH(__FILE__, __LINE__, msg->length, name);
  strncpy(msg->topic_name, name, strlen(name));

  Unicast();

  if ((flags & QOS_MASK) == FLAG_QOS_1 || (flags & QOS_MASK) == FLAG_QOS_2) {
    gateway_response_wait_ = true;
    protocal_response_wait_ = SUBACK;
  }
}

void Thing::Pingresp(int flag) {
  message_header* msg = reinterpret_cast<message_header*>(message_buffer_);
  msg->length = sizeof(message_header);
  msg->type = PINGRESP;

  if (flag == 0)
    Unicast();
  else
    Broadcast();
}

void Thing::Unsubscribe(const uint8_t flags, const char* name) {
  message_id_++;

  msg_unsubscribe* msg = reinterpret_cast<msg_unsubscribe*>(message_buffer_);

  msg->length = sizeof(msg_unsubscribe);
  msg->type = UNSUBSCRIBE;
  // msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_PREDEFINED_ID;
  msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_NAME;
  msg->message_id = bswap(message_id_);
  msg->topic_id = 0;

  CHECK_STRING_LENGTH(__FILE__, __LINE__, msg->length, name);
  strncpy(msg->topic_name, name, strlen(name));

  Unicast();

  if ((flags & QOS_MASK) == FLAG_QOS_1 || (flags & QOS_MASK) == FLAG_QOS_2) {
    gateway_response_wait_ = true;
    protocal_response_wait_ = UNSUBACK;
  }
}

void Thing::Unsubscribe(const uint8_t flags, const uint16_t topicId) {
  message_id_++;

  msg_unsubscribe* msg = reinterpret_cast<msg_unsubscribe*>(message_buffer_);

  msg->length = sizeof(msg_unsubscribe);
  msg->type = UNSUBSCRIBE;
  // msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_PREDEFINED_ID;
  msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_NAME;
  msg->message_id = bswap(message_id_);
  msg->topic_id = bswap(topicId);

  Unicast();

  if ((flags & QOS_MASK) == FLAG_QOS_1 || (flags & QOS_MASK) == FLAG_QOS_2) {
    gateway_response_wait_ = true;
    protocal_response_wait_ = UNSUBACK;
  }
}

void Thing::Pingreq() {
  msg_pingreq* msg = reinterpret_cast<msg_pingreq*>(message_buffer_);
  msg->length = sizeof(msg_pingreq) + strlen(client_id_);
  msg->type = PINGREQ;

  CHECK_STRING_LENGTH(__FILE__, __LINE__, msg->length, client_id_);
  strncpy(msg->client_id, client_id_, strlen(client_id_));

  Unicast();
  last_ping_ = gateway_response_wait_ = true;
  protocal_response_wait_ = PINGRESP;
}

void Thing::PingreqHandler() {
  if (zbee_rx_.getRemoteAddress16() == gateway_address_16_)
    Pingresp(0);
  else
    Pingresp(1);
}

void Thing::PingrespHandler() {}

#ifdef USE_QOS2
void Thing::Pubrec(const msg_publish* recv_msg) {
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

    Unicast();
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
  }
}

void Thing::Pubrel(const msg_pubqos2* recv_msg) {
  gateway_response_wait_ = true;
  protocal_response_wait_ = PUBCOMP;
  while (protocal_response_wait_ == PUBCOMP) {
    msg_pubqos2* msg = reinterpret_cast<msg_pubqos2*>(message_buffer_);
    msg->length = sizeof(msg_pubqos2);
    msg->type = PUBREL;
    msg->message_id = recv_msg->message_id;
    Unicast();
    ReadZbeeTimeout(READ_ZBEE_TIMEOUT);
  }
}

void Thing::Pubcomp(const msg_pubqos2* recv_msg) {
  msg_pubqos2* msg = reinterpret_cast<msg_pubqos2*>(message_buffer_);
  msg->length = sizeof(msg_pubqos2);
  msg->type = PUBCOMP;
  msg->message_id = recv_msg->message_id;

  Unicast();
}

void Thing::PubrecHandler(const msg_pubqos2* msg) { Pubrel(msg); }

void Thing::PubrelHandler(const msg_pubqos2* msg) {
  gateway_response_wait_ = false;
  protocal_response_wait_ = PUBLISH;
  Pubcomp(msg);
}

void Thing::PubcompHandler(const msg_pubqos2* msg) {
  gateway_response_wait_ = false;
  protocal_response_wait_ = PUBLISH;
}

#endif
