#ifndef SMALL_THING_UTILS_H_
#define SMALL_THING_UTILS_H_

#include "common.h"

// get argument from data by name
void* GetArgumentByName(void* pData, const char* name);

// get string argument from data by name
int GetStringArgumentByName(void* pData, const char* name, char** ppszOut);

// get integer argument from data by name
int GetIntArgumentByName(void* pData, const char* name, int* pnOut);

// get double argument from data by name
int GetDoubleArgumentByName(void* pData, const char* name, double* pdbOut);

// get double argument from data by name
int GetBoolArgumentByName(void* pData, const char* name, bool* pdbOut);

// handle mqtt connlost mesage
// TODO(ikess): check if it is needed.
// void brokerConnLost(void *context, char *cause);

// dtostrf works differently on ARM board.
char* Safe_dtostrf(double val, signed char width, unsigned char prec,
                   char* sout);

#endif