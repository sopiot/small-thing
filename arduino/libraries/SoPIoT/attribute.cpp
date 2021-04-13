#include "attribute.h"

#include "utils.h"

void Attribute::Initialize() {
  name_ = NULL;
  string_value_ = NULL;
  real_value_ = 0;
  attribute_type_ = UNDEFINED;
}

Attribute::~Attribute() {
  if (name_) free(name_);
  if (string_value_) free(string_value_);
}

Attribute::Attribute(const char* name, double attribute_value,
                     CapType attribute_type) {
  Initialize();
  attribute_type_ = attribute_type;
  set_attribute_value(attribute_value);
  set_name(name);
}

Attribute::Attribute(const char* name, char* attribute_value,
                     CapType attribute_type) {
  Initialize();
  attribute_type_ = attribute_type;
  set_attribute_value(attribute_value);
  set_name(name);
}

char* Attribute::name() { return name_; }

void Attribute::set_name(const char* name) {
  name_ = strdup(name);
  MEM_ALLOC_CHECK(name_);
}

void Attribute::set_attribute_value(const char* attribute_value) {
  string_value_ = strdup(attribute_value);
  MEM_ALLOC_CHECK(string_value_);
}

void Attribute::set_attribute_value(double attribute_value) {
  real_value_ = attribute_value;
}

CapType Attribute::attribute_type(void) { return attribute_type_; }

void Attribute::GetInformation(char* buffer) {
  switch (attribute_type_) {
    case BOOL: {
      char min_temp[10];
      safe_dtostrf(real_value_, 8, 2, min_temp);
      snprintf(buffer, MAX_BUFFER_SIZE, "%s\tbool\t%s", name_, min_temp);
      break;
    }
    case INTEGER: {
      char min_temp[10];
      safe_dtostrf(real_value_, 8, 2, min_temp);
      snprintf(buffer, MAX_BUFFER_SIZE, "%s\tint\t%s", name_, min_temp);
      break;
    }
    case DOUBLE: {
      char min_temp[10];
      safe_dtostrf(real_value_, 8, 2, min_temp);
      snprintf(buffer, MAX_BUFFER_SIZE, "%s\tdouble\t%s", name_, min_temp);
      break;
    }
    case STRING:
      snprintf(buffer, MAX_BUFFER_SIZE, "%s\tstring\t%s", name_, string_value_);
      break;
    default:
      // error!
      break;
  }
}