#include "utils.h"

#include "argument.h"
#include "function.h"

int GetStringArgumentByName(void* pData, const char* name, char** ppszOut) {
  char* pszData = (char*)GetArgumentByName(pData, name);

  if (pszData == NULL) {
    return -1;
  }

  *ppszOut = pszData;
  return 0;
}

int GetIntArgumentByName(void* pData, const char* name, int* pnOut) {
  int* pnData = (int*)GetArgumentByName(pData, name);
  if (pnData == NULL) {
    return -1;
  }
  *pnOut = *pnData;
  return 0;
}

int GetDoubleArgumentByName(void* pData, const char* name, double* pdbOut) {
  double* pdbData = (double*)GetArgumentByName(pData, name);

  if (pdbData == NULL) {
    return -1;
  }

  *pdbOut = *pdbData;
  return 0;
}

void* GetArgumentByName(void* pData, const char* name) {
  Function* function = (Function*)pData;
  int idx = 0;
  int nargs = function->ncurArguments();
  Argument* ptrarg;
  for (idx = 0; idx < nargs; idx++) {
    ptrarg = function->getIdxArgument(idx);
    if (strcmp(name, ptrarg->name()) == 0) {
      return ptrarg->value();
    }
  }
  if (idx == nargs) return NULL;  // not found
}

// dtostrf works differently on ARM board.
// TODO(thsvkd): fix it with the best practice
char* safe_dtostrf(double val, signed char width, unsigned char prec,
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