#include "argument.h"
#include "utils.h"

void Argument::Initialize() {
  arg_type_ = UNDEFINED;
  name_ = NULL;
  value_ = NULL;
  min_ = NULL;
  max_ = NULL;
}

Argument::Argument(const char* name, int min, int max, CapType arg_type) {
  Initialize();
  arg_type_ = arg_type;
  set_min(min);
  set_max(max);
  set_name(name);
}

Argument::Argument(const char* name, double min, double max, CapType arg_type) {
  Initialize();
  arg_type_ = arg_type;
  set_min(min);
  set_max(max);
  set_name(name);
}

Argument::~Argument() {
  if (name_) free(name_);
  if (value_) free(value_);
  if (min_) free(min_);
  if (max_) free(max_);
}

int Argument::get_order() { return order_; }

void Argument::set_order(const int order) { order_ = (int)order; }

char* Argument::name() { return name_; }

void* Argument::value() { return value_; }

void Argument::set_name(const char* name) {
  name_ = strdup(name);
  MEM_ALLOC_CHECK(name_);

  switch (arg_type_) {
    case BOOL:
    case INTEGER:
      value_ = malloc(sizeof(int));
      MEM_ALLOC_CHECK(value_);
      break;
    case STRING:
      value_ = malloc(sizeof(char) * (*(int*)max_ + 1));
      MEM_ALLOC_CHECK(value_);
      break;
    case DOUBLE:
      value_ = malloc(sizeof(double));
      MEM_ALLOC_CHECK(value_);
      break;
    default:
      break;
  }
}

void Argument::set_min(const int min) {
  if (arg_type_ == INTEGER || arg_type_ == STRING || arg_type_ == BOOL) {
    int* i_min = (int*)malloc(sizeof(int));
    MEM_ALLOC_CHECK(i_min);
    *i_min = min;

    if (min_) free(min_);
    min_ = (void*)i_min;
  } else if (arg_type_ == DOUBLE) {
    double* d_min = (double*)malloc(sizeof(double));
    MEM_ALLOC_CHECK(d_min);
    *d_min = (double)min;

    if (min_) free(min_);
    min_ = (void*)d_min;
  }
}

void Argument::set_min(const double min) {
  if (arg_type_ == DOUBLE) {
    double* d_min = (double*)malloc(sizeof(double));
    MEM_ALLOC_CHECK(d_min);
    *d_min = min;

    if (min_) free(min_);
    min_ = (void*)d_min;
  } else {
    SOPLOGLN(F("[ERROR] the type of min is not suitable"));
  }
}

void Argument::set_max(const int max) {
  if (arg_type_ == INTEGER || arg_type_ == STRING || arg_type_ == BOOL) {
    int* i_max = (int*)malloc(sizeof(int));
    MEM_ALLOC_CHECK(i_max);
    *i_max = max;

    if (max_) free(max_);
    max_ = (void*)i_max;
  } else if (arg_type_ == DOUBLE) {
    double* d_max = (double*)malloc(sizeof(double));
    MEM_ALLOC_CHECK(d_max);
    *d_max = (double)max;

    if (max_) free(max_);
    max_ = (void*)d_max;
  }
}

void Argument::set_max(const double max) {
  if (arg_type_ == DOUBLE) {
    double* d_max = (double*)malloc(sizeof(double));
    MEM_ALLOC_CHECK(d_max);
    *d_max = max;

    if (max_) free(max_);
    max_ = (void*)d_max;
  } else {
    SOPLOGLN(F("[ERROR] the type of max is not suitable"));
  }
}

bool Argument::SetArgumentIfValid(char* val) {
  bool flag = true;
  switch (arg_type_) {
    case BOOL:
    case INTEGER: {
      int nval = atoi(val);
      if (nval < *(int*)min_ || nval > *(int*)max_)
        flag = false;
      else
        *(int*)value_ = nval;
      break;
    }
    case DOUBLE: {
      double dval = atof(val);
      if (dval < *(double*)min_ || dval > *(double*)max_)
        flag = false;
      else
        *(double*)value_ = dval;
      break;
    }
    case STRING: {
      int len = strlen(val);
      if (len < *(int*)min_ || len > *(int*)max_)
        flag = false;
      else
        strncpy((char*)value_, val, len + 1);
      break;
    }
    default:
      break;
  }
  return flag;
}

void Argument::GetInformation(char* buffer) {
  switch (arg_type_) {
    case BOOL:
      snprintf(buffer, MAX_BUFFER_SIZE, "%s\tbool\t%d\t%d\t", name_,
               *(int*)min_, *(int*)max_);
      break;
    case INTEGER:
      snprintf(buffer, MAX_BUFFER_SIZE, "%s\tint\t%d\t%d\t", name_, *(int*)min_,
               *(int*)max_);
      break;
    case STRING:
      snprintf(buffer, MAX_BUFFER_SIZE, "%s\tstring\t%d\t%d\t", name_,
               *(int*)min_, *(int*)max_);
      break;
    case DOUBLE: {
      char min_temp[10];
      char max_temp[10];
      safe_dtostrf(*(double*)min_, 8, 2, min_temp);
      safe_dtostrf(*(double*)max_, 8, 2, max_temp);
      snprintf(buffer, MAX_BUFFER_SIZE, "%s\tdouble\t%s\t%s\t", name_, min_temp,
               max_temp);
      break;
    }
    default:
      // error!
      break;
  }
}

CapType Argument::arg_type(void) { return arg_type_; }