#ifndef SMALL_THING_VALUE_H_
#define SMALL_THING_VALUE_H_

#include "common.h"

/**
 * @brief A class for handling various "value" types.
 */
/**
 * @details A programmer can add Value to the Supervisor by using this class.
 * int | double | bool f(void) - only 3 types can be handled
 */
class Value {
 public:
  /** @} */
  /**
   * @name complete Constructors
   * @brief while using 3 Constructors below, it can be added to the Client
   * without any function calls
   * @param name means the name of the value, sleep_ms_interval means sleep
   * interval of each value in milliseconds.
   * @{
   */
  /**
   * constructor with name, integer value, min, max
   */
  Value(const char *name, IntegerValue value, int min, int max,
        int sleep_ms_interval);
  /**
   * constructor with name, char* value, min, max
   */
  Value(const char *name, StringValue value, int min, int max,
        int sleep_ms_interval);
  /**
   * constructor with name, double value, min, max
   */
  Value(const char *name, DoubleValue value, double min, double max,
        int sleep_ms_interval);
  /**
   * constructor with name, bool value. bool value does not need to set min and
   * max.
   */
  Value(const char *name, BoolValue value, int sleep_ms_interval);
  /** @} */
  /**
   * A destructor
   */
  ~Value();

  /**
   * get name's pointer of the value
   */
  char *name();

  void *value();
  bool value_changed(void *cur);

  /**
   * internal function to set sleep interval of each values/
   */
  void set_sleep_interval(const int sleep_ms_interval);

  /**
   * internal function to get sleep interval of each values/
   */
  int get_sleep_ms_interval();

  /**
   * internal function to set last sent time of each value to publish its value
   * periodically /
   */
  void set_last_sent_time();

  /**
   * internal function to get last sent time of each value to publish its value
   * periodically /
   */
  unsigned long get_last_sent_time();
  /**
   * Get a string that contains information of the value
   * @param buffer output buffer
   */
  void GetInformation(char *buffer);

  /**
   * set a publish id(common00) that registered to the gateway
   * returns applied publish_id
   */
  uint16_t set_publish_id(uint16_t publish_id);
  /**
   * get a publish id that registered to the gateway
   */
  uint16_t publish_id();

  bool capVal2str(char *buffer);

  CapType value_classifier(void);

 private:
  /**
   * @name set value
   * @{
   * set function to get the value
   */
  /**
   * set name of the value
   */
  void set_name(const char *name);
  /**
   * set integer value
   */
  void set_value(IntegerValue value);
  /**
   * set string value
   */
  void set_value(StringValue value);
  /**
   * set double value
   */
  void set_value(DoubleValue value);
  /**
   * set bool value
   */
  void set_value(BoolValue value);

  /**
   * @name set minmax
   * set the minimum value or maximum of the value
   * @{
   */
  /**
   * set the minimum value of the integer value or double value
   */
  void set_min(const int min);
  /**
   * set the minium value of the double value (it does not allowed to the
   * integer value)
   */
  void set_min(const double min);
  /**
   * set the maximum value of the integer value or double value
   */
  void set_max(const int max);
  /**
   * set the maximum value of the double value (it does not allowed to the
   * integer value)
   */
  void set_max(const double max);
  void Initialize();
  uint16_t publish_id_;
  char *name_;
  void *value_;
  void *min_;
  void *max_;
  void *prev_;
  char *user_string_buffer_;
  int sleep_ms_interval_;
  unsigned long last_sent_time_;
  CapType value_classifier_;
};

#endif  // SMALL_THING_VALUE_H_