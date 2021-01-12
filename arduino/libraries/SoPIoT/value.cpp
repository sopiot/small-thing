#include "value.h"

#include "utils.h"

void Value::Initialize() {
  name_ = NULL;
  value_ = NULL;
  min_ = NULL;
  max_ = NULL;
  prev_ = NULL;
  user_string_buffer_ = NULL;
  sleep_ms_interval_ = 0;
  last_sent_time_ = 0;
  value_classifier_ = UNDEFINED;
}

Value::Value(const char *name, BoolValue value, int sleep_ms_interval) {
  Initialize();
  set_name(name);
  set_value(value);
  set_max(1);
  set_min(0);
  set_sleep_interval(sleep_ms_interval);
}

Value::Value(const char *name, IntegerValue value, int min, int max,
             int sleep_ms_interval) {
  Initialize();
  set_name(name);
  set_value(value);
  set_min(min);
  set_max(max);
  set_sleep_interval(sleep_ms_interval);
}

Value::Value(const char *name, StringValue value, int min, int max,
             int sleep_ms_interval) {
  Initialize();
  set_name(name);
  set_value(value);
  set_min(min);
  set_max(max);
  set_sleep_interval(sleep_ms_interval);
}

Value::Value(const char *name, DoubleValue value, double min, double max,
             int sleep_ms_interval) {
  Initialize();
  set_name(name);
  set_value(value);
  set_min(min);
  set_max(max);
  set_sleep_interval(sleep_ms_interval);
}

Value::~Value() {
  if (name_) free(name_);
  if (value_) free(value_);
  if (min_) free(min_);
  if (max_) free(max_);
  if (prev_) free(prev_);
  if (user_string_buffer_) free(user_string_buffer_);
}

char *Value::name() { return (char *)name_; }

void Value::set_name(const char *name) {
  name_ = strdup(name);
  MEM_ALLOC_CHECK(name_);
}

void *Value::value(void) { return value_; }

void Value::set_value(StringValue value) {
  value_ = (void *)value;
  value_classifier_ = STRING;
}

void Value::set_value(IntegerValue value) {
  value_ = (void *)value;

  if (prev_ != NULL) {
    free(prev_);
  }
  prev_ = malloc(sizeof(int));
  MEM_ALLOC_CHECK(prev_);

  *(int *)prev_ = 0;

  value_classifier_ = INTEGER;
}

void Value::set_value(DoubleValue value) {
  value_ = (void *)value;

  if (prev_ != NULL) {
    free(prev_);
  }
  prev_ = malloc(sizeof(double));
  MEM_ALLOC_CHECK(prev_);

  *(double *)prev_ = 0;

  value_classifier_ = DOUBLE;
}

void Value::set_value(BoolValue value) {
  value_ = (void *)value;

  if (prev_ != NULL) {
    free(prev_);
  }
  prev_ = malloc(sizeof(int));
  MEM_ALLOC_CHECK(prev_);

  *(int *)prev_ = -1;

  value_classifier_ = BOOL;
}

bool Value::value_changed(void *cur) {
  bool changed = false;
  switch (value_classifier_) {
    case STRING:
      if (strncmp((char *)cur, (char *)prev_, *(int *)max_) != 0) {
        changed = true;
      }

      if (prev_ != NULL) {
        free(prev_);
      }
      memcpy(prev_, cur, *(int *)max_);

      break;
    case INTEGER:
    case BOOL:
      if (*(int *)prev_ != *(int *)cur) {
        changed = true;
      }

      if (prev_ != NULL) {
        free(prev_);
      }
      memcpy(prev_, cur, sizeof(int));

      break;
    case DOUBLE:
      if (DOUBLE_IS_APPROX_EQUAL(*(double *)prev_, *(double *)cur)) {
        changed = true;
      }

      if (prev_ != NULL) {
        free(prev_);
      }
      memcpy(prev_, cur, sizeof(double));

      break;
    default:
      // error!
      break;
  }
  return changed;
}

void Value::set_min(const int min) {
  if (!min_) min_ = malloc(sizeof(int));
  *(int *)min_ = min;
}

void Value::set_min(const double min) {
  if (!min_) min_ = malloc(sizeof(double));
  *(double *)min_ = (double)min;
}

void Value::set_max(const int max) {
  if (!max_) max_ = malloc(sizeof(int));
  *(int *)max_ = max;

  if (value_classifier_ == STRING) {
    user_string_buffer_ = (char *)malloc((max + 1) * sizeof(char));
    MEM_ALLOC_CHECK(user_string_buffer_);
    prev_ = malloc((max + 1) * sizeof(char));
    MEM_ALLOC_CHECK(prev_);
  }
}

void Value::set_max(const double max) {
  if (!max_) max_ = malloc(sizeof(double));
  *(double *)max_ = max;
}

void Value::set_sleep_interval(const int sleep_ms_interval) {
  sleep_ms_interval_ = sleep_ms_interval;
}

int Value::get_sleep_ms_interval() { return sleep_ms_interval_; }
void Value::set_last_sent_time() { last_sent_time_ = millis(); }

CapType Value::value_classifier() { return value_classifier_; }

uint16_t Value::set_publish_id(uint16_t publish_id) {
  return (publish_id_ = publish_id);
}

uint16_t Value::publish_id() { return publish_id_; }

void Value::GetInformation(char *buffer) {
  switch (value_classifier_) {
    case BOOL:
      snprintf(buffer, MAX_BUFFER_SIZE, "%s\tbool\t%d\t%d", name_, *(int *)min_,
               *(int *)max_);
      break;
    case INTEGER:
      snprintf(buffer, MAX_BUFFER_SIZE, "%s\tint\t%d\t%d", name_, *(int *)min_,
               *(int *)max_);
      break;
    case STRING:
      snprintf(buffer, MAX_BUFFER_SIZE, "%s\tstring\t%d\t%d", name_,
               *(int *)min_, *(int *)max_);
      break;
    case DOUBLE: {
      char min_temp[10];
      char max_temp[10];
      safe_dtostrf(*(double *)min_, 8, 2, min_temp);
      safe_dtostrf(*(double *)max_, 8, 2, max_temp);
      snprintf(buffer, MAX_BUFFER_SIZE, "%s\tdouble\t%s\t%s", name_, min_temp,
               max_temp);
      break;
    }
    default:
      // error!
      break;
  }
}

unsigned long Value::get_last_sent_time() { return last_sent_time_; }

bool Value::capVal2str(char *buffer) {
  void *val;
  int nval;
  char dval;
  char *ptsval;
  int len = 0;
  switch (value_classifier_) {
    case INTEGER: {
      nval = ((IntegerValue)value_)();
      len = snprintf(buffer, MAX_BUFFER_SIZE,
                     "{\"type\" : \"int\" , \"value\" : %d}", nval);
      val = &nval;
      break;
    }
    case DOUBLE: {
      char val_temp[10];
      dval = ((DoubleValue)value_)();
      safe_dtostrf(dval, 8, 2, val_temp);
      len = snprintf(buffer, MAX_BUFFER_SIZE,
                     "{\"type\" : \"double\" , \"value\" : %s}", val_temp);
      val = &dval;
      break;
    }
    case BOOL: {
      nval = ((BoolValue)value_)();
      len = snprintf(buffer, MAX_BUFFER_SIZE,
                     "{\"type\" : \"bool\" , \"value\" : %d}", nval);
      val = &nval;
      break;
    }
    case STRING: {
      ptsval = ((StringValue)value_)(user_string_buffer_, *(int *)max_);
      if (ptsval == NULL) {
        SOPLOGLN(F("Fatal Error is occured on capVal2str!!\n"));
        return false;
      }
      len = snprintf(buffer, MAX_BUFFER_SIZE,
                     "{\"type\" : \"string\" , \"value\" : \"%s\"}", ptsval);
      val = ptsval;
      break;
    }
    case VOID:
    case UNDEFINED:
      SOPLOGLN(F("CriticalError - not allowed value type"));
      break;
  }

  if (len < 0) {
    SOPLOGLN(F("Fatal Error is occured on capVal2str!!\n"));
    return false;
  }

  return value_changed(val);
}