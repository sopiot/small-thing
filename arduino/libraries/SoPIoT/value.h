#ifndef SMALL_THING_VALUE_H_
#define SMALL_THING_VALUE_H_

#include "tag.h"

class Value {
 public:
  Value(const char* name, IntegerValue value, int min, int max,
        int sleep_ms_interval);
  Value(const char* name, StringValue value, int min, int max,
        int sleep_ms_interval);
  Value(const char* name, DoubleValue value, double min, double max,
        int sleep_ms_interval);
  Value(const char* name, BoolValue value, int sleep_ms_interval);
  ~Value();

  void AddTag(const char* tag_name);
  void AddTag(Tag& value_tag);

  char* GetName();
  void* GetCallbackFunction();
  bool GetValueIfChanged(void* cur);

  void SetPublishCycle(const int sleep_ms_interval);
  int GetPublishCycle();

  void SetLastSentTime();
  unsigned long GetLastSentTime();

  Tag* GetTag(int idx) { return value_tags_[idx]; }
  int GetTagNum() { return num_tag_; }

  void GetRegisterPublishData(char* buffer);
  bool GetValuePublishJson(char* buffer);

  SoPType GetValueType(void);

  uint16_t SetPublishID(uint16_t publish_id);
  uint16_t GetPublishID();

 private:
  void SetName(const char* name);

  void SetValue(IntegerValue value);
  void SetValue(StringValue value);
  void SetValue(DoubleValue value);
  void SetValue(BoolValue value);

  void SetMin(const int min);
  void SetMin(const double min);
  void SetMax(const int max);
  void SetMax(const double max);

  void Initialize();

  uint16_t publish_id_;

  char* name_;
  void* callback_function_;
  void* min_;
  void* max_;
  void* prev_value_;
  void* new_value_;

  Tag** value_tags_;
  int num_tag_;

  char* user_string_buffer_;
  int publish_cycle_;
  unsigned long last_sent_time_;

  SoPType value_type_;
};

#endif  // SMALL_THING_VALUE_H_