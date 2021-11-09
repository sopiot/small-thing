#include "utils.h"
#include "value.h"

void Value::Initialize() {
  name_ = NULL;
  callback_function_ = NULL;
  min_ = NULL;
  max_ = NULL;
  user_string_buffer_ = NULL;

  publish_cycle_ = 0;
  last_sent_time_ = 0;
  value_type_ = UNDEFINED;
}

Value::Value(const char* name, BoolValue value, int sleep_ms_interval) {
  Initialize();
  SetName(name);
  SetValue(value);
  SetMax(1);
  SetMin(0);
  SetPublishCycle(sleep_ms_interval);
}

Value::Value(const char* name, IntegerValue value, int min, int max,
             int sleep_ms_interval) {
  Initialize();
  SetName(name);
  SetValue(value);
  SetMin(min);
  SetMax(max);
  SetPublishCycle(sleep_ms_interval);
}

Value::Value(const char* name, StringValue value, int min, int max,
             int sleep_ms_interval) {
  Initialize();
  SetName(name);
  SetValue(value);
  SetMin(min);
  SetMax(max);
  SetPublishCycle(sleep_ms_interval);
}

Value::Value(const char* name, DoubleValue value, double min, double max,
             int sleep_ms_interval) {
  Initialize();
  SetName(name);
  SetValue(value);
  SetMin(min);
  SetMax(max);
  SetPublishCycle(sleep_ms_interval);
}

Value::~Value() {
  if (name_) free(name_);
  if (callback_function_) free(callback_function_);
  if (min_) free(min_);
  if (max_) free(max_);
  if (user_string_buffer_) free(user_string_buffer_);
}

void Value::AddTag(const char* tag_name) {
  Tag* tag = new Tag(tag_name);
  AddTag(*tag);
}

void Value::AddTag(Tag& value_tag) {
  value_tags_ = (Tag**)realloc(value_tags_, sizeof(Tag*) * (num_tag_ + 1));
  value_tags_[num_tag_] = &value_tag;
  MEM_ALLOC_CHECK(value_tags_[num_tag_]);
  num_tag_++;
}

char* Value::GetName() { return (char*)name_; }

void Value::SetName(const char* name) {
  name_ = strdup(name);
  MEM_ALLOC_CHECK(name_);
}

void* Value::GetCallbackFunction() { return callback_function_; }

void Value::SetValue(StringValue value) {
  callback_function_ = (void*)value;
  value_type_ = STRING;
}

void Value::SetValue(IntegerValue value) {
  callback_function_ = (void*)value;
  value_type_ = INTEGER;
}

void Value::SetValue(DoubleValue value) {
  callback_function_ = (void*)value;
  value_type_ = DOUBLE;
}

void Value::SetValue(BoolValue value) {
  callback_function_ = (void*)value;
  value_type_ = BOOL;
}

bool Value::ValueChanged(union NewValuePool new_value) {
  bool changed = false;

  switch (value_type_) {
    case STRING:
      if (strncmp(new_value.string_value, prev_value_.string_value,
                  *(int*)max_) != 0) {
        changed = true;
        memcpy(prev_value_.string_value, new_value.string_value, *(int*)max_);
      }
      break;
    case INTEGER:
    case BOOL:
      if (prev_value_.int_value != new_value.int_value) {
        changed = true;
        prev_value_.int_value = new_value.int_value;
      }
      break;
    case DOUBLE:
      if (!DOUBLE_IS_APPROX_EQUAL(prev_value_.double_value,
                                  new_value.double_value)) {
        changed = true;
        prev_value_.double_value = new_value.double_value;
      }
      break;
    default:
      SOPLOGLN(F("Value type error! type : %d"), value_type_);
      break;
  }

  return changed;
}

void Value::SetMin(const int min) {
  if (!min_) min_ = malloc(sizeof(int));
  *(int*)min_ = min;
}

void Value::SetMin(const double min) {
  if (!min_) min_ = malloc(sizeof(double));
  *(double*)min_ = (double)min;
}

void Value::SetMax(const int max) {
  if (!max_) max_ = malloc(sizeof(int));
  *(int*)max_ = max;

  if (value_type_ == STRING) {
    user_string_buffer_ = (char*)malloc((max + 1) * sizeof(char));
    MEM_ALLOC_CHECK(user_string_buffer_);
  }
}

void Value::SetMax(const double max) {
  if (!max_) max_ = malloc(sizeof(double));
  *(double*)max_ = max;
}

void Value::SetPublishCycle(const int sleep_ms_interval) {
  publish_cycle_ = sleep_ms_interval;
}

int Value::GetPublishCycle() { return publish_cycle_; }

void Value::SetLastSentTime() { last_sent_time_ = millis(); }

SoPType Value::GetValueType() { return value_type_; }

void Value::GetRegisterPublishData(char* buffer) {
  switch (value_type_) {
    case INTEGER:
      snprintf(buffer, MAX_BUFFER_SIZE, "%s#int#%d#%d", name_, *(int*)min_,
               *(int*)max_);
      break;
    case DOUBLE: {
      char min_temp[10];
      char max_temp[10];
      Safe_dtostrf(*(double*)min_, 8, 2, min_temp);
      Safe_dtostrf(*(double*)max_, 8, 2, max_temp);
      snprintf(buffer, MAX_BUFFER_SIZE, "%s#double#%s#%s", name_, min_temp,
               max_temp);
      break;
    }
    case BOOL:
      snprintf(buffer, MAX_BUFFER_SIZE, "%s#bool#%d#%d", name_, *(int*)min_,
               *(int*)max_);
      break;
    case STRING:
      snprintf(buffer, MAX_BUFFER_SIZE, "%s#string#%d#%d", name_, *(int*)min_,
               *(int*)max_);
      break;
    default:
      SOPLOGLN("UNDEFINED Value type... error!");
      break;
  }
}

unsigned long Value::GetLastSentTime() { return last_sent_time_; }

bool Value::GetValuePublishJson(char* buffer) {
  void* val;
  int nval;
  char dval;
  char* ptsval;
  int buffer_size = 0;
  switch (value_type_) {
    case INTEGER: {
      new_value_.int_value = ((IntegerValue)callback_function_)();
      buffer_size = snprintf(buffer, MAX_BUFFER_SIZE,
                             "{\"type\" : \"int\" , \"value\" : %d}\n",
                             new_value_.int_value);
      break;
    }
    case DOUBLE: {
      new_value_.double_value = ((DoubleValue)callback_function_)();
      buffer_size = snprintf(buffer, MAX_BUFFER_SIZE,
                             "{\"type\" : \"double\" , \"value\" : %fl}\n",
                             new_value_.double_value);
      break;
    }
    case BOOL: {
      new_value_.bool_value = ((BoolValue)callback_function_)();
      buffer_size = snprintf(buffer, MAX_BUFFER_SIZE,
                             "{\"type\" : \"bool\" , \"value\" : %d}\n",
                             new_value_.bool_value);
      break;
    }
    case STRING: {
      new_value_.string_value =
          ((StringValue)callback_function_)(user_string_buffer_, *(int*)max_);
      if (new_value_.string_value == NULL) {
        SOPLOGLN(F("Fatal Error is occured on GetValuePublishJson!!\n"));
        return false;
      }
      buffer_size = snprintf(buffer, MAX_BUFFER_SIZE,
                             "{\"type\" : \"string\" , \"value\" : \"%s\"}\n",
                             new_value_.string_value);
      break;
    }
    case VOID:
    case UNDEFINED:
      SOPLOGLN(F("[ERROR] not allowed value type"));
      break;
  }

  if (buffer_size < 0) {
    SOPLOGLN(
        F("[ERROR] Fatal Error is occured on GetValuePublishJson!! "
          "buffer_size == "
          "0\n"));
    return false;
  }

  SOPLOGLN(F("new_value_.int_value: %d"), new_value_.int_value);
  // SOPLOGLN(F("new_value_.double_value: %d"), new_value_.double_value);
  // SOPLOGLN(F("new_value_.bool_value: %d"), new_value_.bool_value);
  // SOPLOGLN(F("new_value_.string_value: %d"), new_value_.string_value);
  SOPLOGLN(F("prev_value_: %d"), prev_value_);
  return ValueChanged(new_value_);
}

uint16_t Value::SetPublishID(uint16_t publish_id) {
  return (publish_id_ = publish_id);
}

uint16_t Value::GetPublishID() { return publish_id_; }
