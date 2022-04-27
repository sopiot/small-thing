#ifndef COMMON_H_
#define COMMON_H_

#include <Arduino.h>
#include <stdarg.h>
#define SOP_DEBUG
#ifdef SOP_DEBUG

#define SOPLOG_LIMIT 32

static void SOPLOG(char *fmt, ...) {
  char buf[SOPLOG_LIMIT];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, SOPLOG_LIMIT, fmt, args);
  va_end(args);
  Serial.print(buf);
}

static void SOPLOG(const __FlashStringHelper *fmt, ...) {
  char buf[SOPLOG_LIMIT];
  va_list args;
  va_start(args, fmt);
#ifdef __AVR__
  // progmem for AVR
  vsnprintf_P(buf, sizeof(buf), (const char *)fmt, args);
#else
  // for the rest of the world
  vsnprintf(buf, sizeof(buf), (const char *)fmt, args);
#endif
  va_end(args);
  Serial.print(buf);
}

static void SOPLOGLN(char *fmt, ...) {
  char buf[SOPLOG_LIMIT];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, SOPLOG_LIMIT, fmt, args);
  va_end(args);
  Serial.println(buf);
}

static void SOPLOGLN(const __FlashStringHelper *fmt, ...) {
  char buf[SOPLOG_LIMIT];
  va_list args;
  va_start(args, fmt);
#ifdef __AVR__
  // progmem for AVR
  vsnprintf_P(buf, sizeof(buf), (const char *)fmt, args);
#else
  // for the rest of the world
  vsnprintf(buf, sizeof(buf), (const char *)fmt, args);
#endif
  va_end(args);
  Serial.println(buf);
}

#else
#define SOPLOG(fmt, ...)
#define SOPLOGLN(fmt, ...)
#endif

// // dtostrf works differently on ARM board.
// // TODO(thsvkd): fix it with the best practice
// char *safe_dtostrf(double val, signed char width, unsigned char prec,
//                    char *sout) {
// #if defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_SAM) || \
//     defined(ARDUINO_ARCH_MBED)
//   asm(".global _printf_float");

//   char fmt[20];
//   sprintf(fmt, "%%%d.%df", width, prec);
//   sprintf(sout, fmt, val);
//   return sout;
// #else
//   return dtostrf(val, width, prec, sout);
// #endif
// }

#endif  // COMMON_H_
