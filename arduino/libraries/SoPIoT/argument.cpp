#include "argument.h"
#include "utils.h"

void Argument::Initialize() {
  argument_type_ = UNDEFINED;
  name_ = NULL;
  value_ = NULL;
  min_ = NULL;
  max_ = NULL;
}

Argument::Argument() { Initialize(); }

Argument::Argument(const char* name, int min, int max, SoPType argument_type) {
  Initialize();
  argument_type_ = argument_type;
  SetMin(min);
  SetMax(max);
  SetName(name);
}

Argument::Argument(const char* name, double min, double max,
                   SoPType argument_type) {
  Initialize();
  argument_type_ = argument_type;
  SetMin(min);
  SetMax(max);
  SetName(name);
}

Argument::~Argument() {
  if (name_) free(name_);
  if (value_) free(value_);
  if (min_) free(min_);
  if (max_) free(max_);
}

int Argument::GetOrder() { return order_; }

void* Argument::GetMin() { return min_; }

void* Argument::GetMax() { return max_; }

void Argument::SetOrder(const int order) { order_ = (int)order; }

char* Argument::GetName() { return name_; }

void* Argument::GetValue() { return value_; }

void Argument::SetName(const char* name) {
  name_ = strdup(name);
  MEM_ALLOC_CHECK(name_);

  switch (argument_type_) {
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

void Argument::SetMin(const int min) {
  if (argument_type_ == INTEGER || argument_type_ == STRING ||
      argument_type_ == BOOL) {
    int* i_min = (int*)malloc(sizeof(int));
    MEM_ALLOC_CHECK(i_min);
    *i_min = min;

    if (min_) free(min_);
    min_ = (void*)i_min;
  } else if (argument_type_ == DOUBLE) {
    double* d_min = (double*)malloc(sizeof(double));
    MEM_ALLOC_CHECK(d_min);
    *d_min = (double)min;

    if (min_) free(min_);
    min_ = (void*)d_min;
  }
}

void Argument::SetMin(const double min) {
  if (argument_type_ == DOUBLE) {
    double* d_min = (double*)malloc(sizeof(double));
    MEM_ALLOC_CHECK(d_min);
    *d_min = min;

    if (min_) free(min_);
    min_ = (void*)d_min;
  } else {
    SOPLOGLN(F("[ERROR] the type of min is not suitable"));
  }
}

void Argument::SetMax(const int max) {
  if (argument_type_ == INTEGER || argument_type_ == STRING ||
      argument_type_ == BOOL) {
    int* i_max = (int*)malloc(sizeof(int));
    MEM_ALLOC_CHECK(i_max);
    *i_max = max;

    if (max_) free(max_);
    max_ = (void*)i_max;
  } else if (argument_type_ == DOUBLE) {
    double* d_max = (double*)malloc(sizeof(double));
    MEM_ALLOC_CHECK(d_max);
    *d_max = (double)max;

    if (max_) free(max_);
    max_ = (void*)d_max;
  }
}

void Argument::SetMax(const double max) {
  if (argument_type_ == DOUBLE) {
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
  switch (argument_type_) {
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

void Argument::GetRegisterPublishData(char* buffer) {
  switch (argument_type_) {
    case BOOL:
      snprintf(buffer, MAX_BUFFER_SIZE, "%s#bool#%d#%d", name_, *(int*)min_,
               *(int*)max_);
      break;
    case INTEGER:
      snprintf(buffer, MAX_BUFFER_SIZE, "%s#int#%d#%d", name_, *(int*)min_,
               *(int*)max_);
      break;
    case STRING:
      snprintf(buffer, MAX_BUFFER_SIZE, "%s#string#%d#%d", name_, *(int*)min_,
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
    default:
      // error!
      break;
  }
}

SoPType Argument::GetArgumentType(void) { return argument_type_; }