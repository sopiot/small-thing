#ifndef SMALL_THING_COMMON_H_
#define SMALL_THING_COMMON_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Printers.h>
#include <Stream.h>
#include <XBee.h>
#include <stdarg.h>

//----------------------------------------
// Author: ikess
// This codes are
// From mqttsn.h by John Donovan (20210107 ikess guessed.)
// ref:
// https://bitbucket.org/MerseyViking/mqtt-sn-arduino/src/master/src/mqttsn.h
//----------------------------------------

#define PROTOCOL_ID 0x01

#define FLAG_DUP 0x80
#define FLAG_QOS_0 0x00
#define FLAG_QOS_1 0x20
#define FLAG_QOS_2 0x40
#define FLAG_QOS_M1 0x60
#define FLAG_RETAIN 0x10
#define FLAG_WILL 0x08
#define FLAG_CLEAN 0x04
#define FLAG_TOPIC_NAME 0x00
#define FLAG_TOPIC_PREDEFINED_ID 0x01
#define FLAG_TOPIC_SHORT_NAME 0x02

#define QOS_MASK (FLAG_QOS_0 | FLAG_QOS_1 | FLAG_QOS_2 | FLAG_QOS_M1)
#define TOPIC_MASK \
  (FLAG_TOPIC_NAME | FLAG_TOPIC_PREDEFINED_ID | FLAG_TOPIC_SHORT_NAME)

// Recommended values for timers and counters. All timers are in seconds.
#define T_ADV 960
#define N_ADV 3
#define T_SEARCH_GW 5
#define T_GW_INFO 5
#define T_WAIT 360
#define T_RETRY 15
#define N_RETRY 5

#define SOPLOG_LIMIT 256

enum return_code_t {
  ACCEPTED,
  REJECTED_CONGESTION,
  REJECTED_INVALID_TOPIC_ID,
  REJECTED_NOT_SUPPORTED
};

// 0x03, 0x11, 0x19, 0x1E~0xFD, 0xFF is reserved
enum message_type {
  ADVERTISE,
  SEARCHGW,
  GWINFO,
  CONNECT = 0x04,  // for 0x03 reserved
  CONNACK,
  WILLTOPICREQ,
  WILLTOPIC,
  WILLMSGREQ,
  WILLMSG,
  REGISTER,
  REGACK,
  PUBLISH,
  PUBACK,
  PUBCOMP,
  PUBREC,
  PUBREL,
  SUBSCRIBE = 0x12,  // for 0x11 reserved
  SUBACK,
  UNSUBSCRIBE,
  UNSUBACK,
  PINGREQ,
  PINGRESP,
  DISCONNECT,
  WILLTOPICUPD = 0x1a,  // for 0x19 reserved
  WILLTOPICRESP,
  WILLMSGUPD,
  WILLMSGRESP,
};

//----------------------------------------
// MQTT-SN Basic Protocols
//----------------------------------------

// Author: thsvkd
// on ARM board, structure is divided(packed) by 4 bytes
// to sync with AVR board, set structure packing size to 1 byte.
#pragma pack(push, 1)

struct message_header {
  uint8_t length;
  uint8_t type;
};

struct msg_advertise : public message_header {
  uint8_t gw_id;
  uint16_t duration;
};

struct msg_searchgw : public message_header {
  uint8_t radius;
};

struct msg_gwinfo : public message_header {
  uint8_t gw_id;
  char gw_add[0];
};

struct msg_connect : public message_header {
  uint8_t flags;
  uint8_t protocol_id;
  uint16_t duration;
  char client_id[0];
};

struct msg_connack : public message_header {
  return_code_t return_code;
};

struct msg_willtopic : public message_header {
  uint8_t flags;
  char will_topic[0];
};

struct msg_willmsg : public message_header {
  char willmsg[0];
};

struct msg_register : public message_header {
  uint16_t topic_id;
  uint16_t message_id;
  char topic_name[0];
};

struct msg_regack : public message_header {
  uint16_t topic_id;
  uint16_t message_id;
  uint8_t return_code;
};

struct msg_publish : public message_header {
  uint8_t flags;
  uint16_t topic_id;
  uint16_t message_id;
  char data[0];
};

struct msg_puback : public message_header {
  uint16_t topic_id;
  uint16_t message_id;
  uint8_t return_code;
};

struct msg_pubqos2 : public message_header {
  uint16_t message_id;
};

struct msg_subscribe : public message_header {
  uint8_t flags;
  uint16_t message_id;
  union {
    char topic_name[0];
    uint16_t topic_id;
  };
};

struct msg_suback : public message_header {
  uint8_t flags;
  uint16_t topic_id;
  uint16_t message_id;
  uint8_t return_code;
};

struct msg_unsubscribe : public message_header {
  uint8_t flags;
  uint16_t message_id;
  union {
    char topic_name[0];
    uint16_t topic_id;
  };
};

struct msg_unsuback : public message_header {
  uint16_t message_id;
};

struct msg_pingreq : public message_header {
  char client_id[0];
};

struct msg_disconnect : public message_header {
  uint16_t duration;
};

struct msg_willtopicresp : public message_header {
  uint8_t return_code;
};

struct msg_willmsgresp : public message_header {
  uint8_t return_code;
};

#pragma pack(pop)

//----------------------------------------
// SoPIoT Defines
//----------------------------------------

// Config
#define MAX_VALUE_NUM 20
#define MAX_FUNCTION_NUM 20
#define MAX_NAME_LENGTH 20
#define MAX_BUFFER_SIZE 60

// SoPIoT protocols (See specification documentation)
#define COMMON0000 "%s/%s"

// TODO: Update protocol!!!
// MW --> Thing
#define MT1001 "MT/RESULT/REGISTER/%s"
#define MT1002 "MT/PINGREQ/%s"
#define MT1003 "MT/SN/%s/%s"
// #define Alive_trig "EM/REFRESH/#"

// Thing --> MW
#define TM2001 "TM/REGISTER/%s"
#define TM2002 "TM/UNREGISTER/%s"
#define TM2003 "TM/ALIVE/%s"

#define TM2010 "TM/SN/REGISTER/VALUE/%s"
#define TM2011 "TM/SN/REGISTER/VALUETAG/%s"
#define TM2012 "TM/SN/REGISTER/FUNCTION/%s"
#define TM2013 "TM/SN/REGISTER/FUNCTIONTAG/%s"
#define TM2014 "TM/SN/REGISTER/ARGUMENT/%s"
#define TM2015 "TM/SN/REGISTER/ALIVECYCLE/%s"
#define TM2016 "TM/SN/REGISTER/FINISH/%s"

#define TM2004 "TM/RESULT/EXECUTE/%s/%s"

// Double data type comparison
#define DOUBLE_EPSILON (0.0000001)
#define DOUBLE_IS_APPROX_EQUAL(a, b) (fabs((a) - (b)) <= DOUBLE_EPSILON)

// QoS 1
// #define QOS 1
// #define USE_QOS1
// QoS 2
// #define QOS 2
// #define USE_QOS2

#ifdef USE_QOS2
#define QOS_FLAG FLAG_QOS_2
#elif defined(USE_QOS1)
#define QOS_FLAG FLAG_QOS_1
#else
#define QOS_FLAG FLAG_QOS_0
#endif

// Author: thsvkd
// Depending arduino board,
// Serial variable is different.
// Some are Serial, the others are Serial1
// TODO: fix it with the best practice
#if (defined(ARDUINO_ARCH_SAMD) && !defined(ARDUINO_SAMD_ZERO)) || \
    (defined(ARDUINO_ARCH_SAM) && !defined(ARDUINO_SAM_DUE)) ||    \
    defined(ARDUINO_ARCH_MBED) || defined(__AVR_ATmega32U4__) ||   \
    defined(ARDUINO_AVR_PROMICRO)
#define USE_SERIAL_ONE
#endif

#ifdef USE_SERIAL_ONE
#define SafeSerial Serial1
#else
#define SafeSerial Serial
#endif

typedef enum _soptype {
  UNDEFINED = 0, /** < represents UNDEFINED */
  INTEGER,       /** < represents IntegerFunction or IntegerValue */
  DOUBLE,        /** < represents DoubleFunction or DoubleValue */
  BOOL,          /** < represents BoolFunction or BoolValue */
  STRING,        /** < represents Character array Value > */
  BINARY,        /** <represents Binary Value> */
  VOID,          /** < represents VoidFunction */
} SoPType;

typedef enum _sopdevreg {
  VALUE = 0,
  VALUE_TAG,
  FUNCTION,
  FUNCTION_TAG,
  ARGUMENT,
  DURATION,
  FINISH,
} SoPRegStatus;

typedef enum _sopstate {
  UNREGISTERED = 0,
  REGISTERED,
} SoPState;

typedef void (*VoidFunction)(void);
typedef int (*IntegerFunction)(void);
typedef double (*DoubleFunction)(void);
typedef bool (*BoolFunction)(void);

typedef void (*VoidArgumentFunction)(void *);
typedef int (*IntegerArgumentFunction)(void *);
typedef double (*DoubleArgumentFunction)(void *);
typedef bool (*BoolArgumentFunction)(void *);

typedef int (*IntegerValue)(void);
typedef double (*DoubleValue)(void);
typedef bool (*BoolValue)(void);
typedef char *(*StringValue)(char *, int);

#define SOP_DEBUG
#ifdef SOP_DEBUG

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

#define MEM_ALLOC_CHECK(var)           \
  if (var == NULL) {                   \
    SOPLOGLN("[ERROR] malloc failed"); \
  }

#define CHECK_STRING_LENGTH(file, line, total_length, data)              \
  if (total_length - 1 > MAX_BUFFER_SIZE) {                              \
    while (true) {                                                       \
      SOPLOGLN(F("[%s]-%d: string was too long... -> %s: total length: " \
                 "%d, string length: %d"),                               \
               file, line, data, total_length, strlen(data));            \
      delay(1000);                                                       \
    }                                                                    \
  }

#endif  // SMALL_THING_COMMON_H_