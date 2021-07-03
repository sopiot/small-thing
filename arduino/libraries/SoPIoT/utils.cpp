#include "argument.h"
#include "function.h"
#include "utils.h"

int GetStringArgumentByName(void* pData, const char* name, char** out) {
  char* data = (char*)GetArgumentByName(pData, name);

  if (data == NULL) {
    return -1;
  }

  *out = data;
  return 0;
}

int GetIntArgumentByName(void* pData, const char* name, int* out) {
  int* data = (int*)GetArgumentByName(pData, name);
  if (data == NULL) {
    return -1;
  }

  *out = *data;
  return 0;
}

int GetDoubleArgumentByName(void* pData, const char* name, double* out) {
  double* data = (double*)GetArgumentByName(pData, name);

  if (data == NULL) {
    return -1;
  }

  *out = *data;
  return 0;
}

int GetBoolArgumentByName(void* pData, const char* name, int* out) {
  bool* data = (bool*)GetArgumentByName(pData, name);
  if (data == NULL) {
    return -1;
  }

  *out = *data;
  return 0;
}

void* GetArgumentByName(void* pData, const char* name) {
  Function* function = (Function*)pData;
  int idx = 0;
  int nargs = function->GetArgumentNum();
  Argument* ptrarg;
  for (idx = 0; idx < nargs; idx++) {
    ptrarg = function->GetArgument(idx);
    if (strcmp(name, ptrarg->GetName()) == 0) {
      return ptrarg->GetValue();
    }
  }
  if (idx == nargs) return NULL;  // not found
}

// dtostrf works differently on ARM board.
// TODO(thsvkd): fix it with the best practice
char* Safe_dtostrf(double val, signed char width, unsigned char prec,
                   char* sout) {
#if defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_SAM) || \
    defined(ARDUINO_ARCH_MBED)
  asm(".global _printf_float");

  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
#else
  return dtostrf(val, width, prec, sout);
#endif
}