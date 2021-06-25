#ifndef SMALL_THING_VALUE_H_
#define SMALL_THING_VALUE_H_

#include "common.h"
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
  void* value();
  bool value_changed(void* cur);

  void set_sleep_interval(const int sleep_ms_interval);
  int get_sleep_ms_interval();

  void set_last_sent_time();
  unsigned long get_last_sent_time();

  Tag* getTag(int idx) { return ValueTags_[idx]; }
  int getTagNum() { return num_tag_; }

  void GetInformation(char* buffer);

  bool GetPublishJson(char* buffer);

  SoPType value_classifier(void);

  uint16_t set_publish_id(uint16_t publish_id);
  uint16_t publish_id();

 private:
  void SetName(const char* name);

  void set_value(IntegerValue value);
  void set_value(StringValue value);
  void set_value(DoubleValue value);
  void set_value(BoolValue value);

  void set_min(const int min);
  void set_min(const double min);

  void set_max(const int max);
  void set_max(const double max);

  void Initialize();

  uint16_t publish_id_;
  char* name_;
  void* value_;
  void* min_;
  void* max_;
  void* prev_;

  Tag** ValueTags_;
  int num_tag_;

  char* user_string_buffer_;
  int sleep_ms_interval_;
  unsigned long last_sent_time_;

  SoPType value_classifier_;
};

#endif  // SMALL_THING_VALUE_H_