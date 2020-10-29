/**
 * @file thing_defines.h
 * @brief define macros, default enums and types
 */

#ifndef CAPITAL_ARDUINO_THING_DEFINES_H_
#define CAPITAL_ARDUINO_THING_DEFINES_H_

#define COMMON0000 "%s/%s"
#define MT1001 "MT/REGACK/%s"
#define MT1002 "MT/PINGREQ/%s"
#define MT1003 "MT/%s/%s"

#define TM2001 "TM/REGISTER/%s"
#define TM2002 "TM/UNREGISTER/%s"
#define TM2003 "TM/ALIVE/%s"
#define TM2004_DEPRECATED "TM/RESULT/FUNCTION/%s"
#define TM2004 "TM/RESULT/FUNCTION/%s/%s"

#define QOS 2
#define TIMEOUT 10000L

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
#define TOPIC_MASK (FLAG_TOPIC_NAME | FLAG_TOPIC_PREDEFINED_ID | FLAG_TOPIC_SHORT_NAME)

// Recommended values for timers and counters. All timers are in seconds.
#define T_ADV 960
#define N_ADV 3
#define T_SEARCH_GW 5
#define T_GW_INFO 5
#define T_WAIT 360
#define T_RETRY 15
#define N_RETRY 5
#define CAP_DEBUG
#ifdef CAP_DEBUG
#define CPDBG(...) Serial.println(__VA_ARGS__)
#else
#define CPDBG(...)
#endif

#define DOUBLE_EPSILON (0.0000001)
#define DOUBLE_IS_APPROX_EQUAL(a, b) (fabs((a) - (b)) <= DOUBLE_EPSILON)

#define USE_QOS2

#ifdef USE_QOS2
#define QOS_FLAG FLAG_QOS_2
#else
#define QOS_FLAG 0
#endif

typedef enum _captype
{
    UNDEFINED = 0,
    VOID,
    INTEGER,
    DOUBLE,
    BOOL,
    STRING,
} CapType;

typedef enum _capdevreg
{
    VALUE = 0,
    ATTRIBUTE,
    FUNCTION,
    ARGUMENT,
    //DOWHAN ADDEED
    FUNCTION_ATTRIBUTE,
    //END
    DURATION,
    FINISH,
} CapRegStatus;

typedef enum _capstate
{
    UNREGISTERED = 0,
    REGISTERED,
} CapState;

typedef void (*VoidFunction)(void *);
typedef int (*IntegerFunction)(void *);
typedef double (*DoubleFunction)(void *);
typedef bool (*BoolFunction)(void *);

typedef int (*IntegerValue)(void);
typedef double (*DoubleValue)(void);
typedef bool (*BoolValue)(void);
typedef char *(*StringValue)(char *, int);

enum return_code_t
{
    ACCEPTED,
    REJECTED_CONGESTION,
    REJECTED_INVALID_TOPIC_ID,
    REJECTED_NOT_SUPPORTED
};

enum message_type
{
    ADVERTISE,
    SEARCHGW,
    GWINFO,
    CONNECT = 0x04,
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
    SUBSCRIBE = 0x12,
    SUBACK,
    UNSUBSCRIBE,
    UNSUBACK,
    PINGREQ,
    PINGRESP,
    DISCONNECT,
    WILLTOPICUPD = 0x1a,
    WILLTOPICRESP,
    WILLMSGUPD,
    WILLMSGRESP,
    DEVREG,
    DEVREGACK
};

/*********************************************************
At ARM board, struct packing structure is divide by 4 byte
so you have to use pragma(1) like that
**********************************************************/
#if defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_SAM)
#pragma pack(push, 1)
#endif
struct message_header
{
    uint8_t length;
    uint8_t type;
};

struct msg_advertise : public message_header
{
    uint8_t gw_id;
    uint16_t duration;
};

struct msg_searchgw : public message_header
{
    uint8_t radius;
};

struct msg_gwinfo : public message_header
{
    uint8_t gw_id;
    char gw_add[0];
};

struct msg_connect : public message_header
{
    uint8_t flags;
    uint8_t protocol_id;
    uint16_t duration;
    char client_id[0];
};

struct msg_connack : public message_header
{
    return_code_t return_code;
};

struct msg_willtopic : public message_header
{
    uint8_t flags;
    char will_topic[0];
};

struct msg_willmsg : public message_header
{
    char willmsg[0];
};

struct msg_register : public message_header
{
    uint16_t topic_id;
    uint16_t message_id;
    char topic_name[0];
};

struct msg_regack : public message_header
{
    uint16_t topic_id;
    uint16_t message_id;
    uint8_t return_code;
};

struct msg_publish : public message_header
{
    uint8_t flags;
    uint16_t topic_id;
    uint16_t message_id;
    char data[0];
};

struct msg_puback : public message_header
{
    uint16_t topic_id;
    uint16_t message_id;
    uint8_t return_code;
};

struct msg_pubqos2 : public message_header
{
    uint16_t message_id;
};

struct msg_subscribe : public message_header
{
    uint8_t flags;
    uint16_t message_id;
    union
    {
        char topic_name[0];
        uint16_t topic_id;
    };
};

struct msg_suback : public message_header
{
    uint8_t flags;
    uint16_t topic_id;
    uint16_t message_id;
    uint8_t return_code;
};

//

struct msg_devreg : public message_header
{
    uint16_t pub_id;
    uint16_t message_id;
    uint8_t status;
    char data[0];
};

// status -> 0 : continue 1 : last

struct msg_devregack : public message_header
{
    uint8_t flags;
    uint16_t message_id;
    uint8_t return_code;
};

// added

struct msg_unsubscribe : public message_header
{
    uint8_t flags;
    uint16_t message_id;
    union
    {
        char topic_name[0];
        uint16_t topic_id;
    };
};

struct msg_unsuback : public message_header
{
    uint16_t message_id;
};

struct msg_pingreq : public message_header
{
    char client_id[0];
};

struct msg_disconnect : public message_header
{
    uint16_t duration;
};

struct msg_willtopicresp : public message_header
{
    uint8_t return_code;
};

struct msg_willmsgresp : public message_header
{
    uint8_t return_code;
};
#if defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_SAM)
#pragma pack(pop)
#endif

#endif
