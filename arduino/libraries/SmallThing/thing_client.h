/**
 * @file	thing_client.h
 * @brief class Value, Function, Client included
 * @author Donghyun Kang, kangdongh@iris.snu.ac.kr
 * @date 2016-04-01
 * @version 1.0.0
 * @section changelog Change Log
 * 2016/04/01 the first reconstruction applied
 * @section license_section License
 * TODO
 */

#ifndef _CAPITAL_ST_THING_CLIENT_H_
#define _CAPITAL_ST_THING_CLIENT_H_

#include <Arduino.h>
#include <Stream.h>
#include <XBee.h>
#include "thing_defines.h"

#define CAPITAL_MAX_VALUE 10
#define CAPITAL_MAX_FUNCTION 5
#define CAPITAL_MAX_ATTRIBUTE 5
#define CAPITAL_MAX_NAME_LENGTH 20
#define CAPITAL_MAX_BUFFER_SIZE 60

/**
 * @brief A class for handling various "value" types.
 */
/**
 * @details A programmer can add Value to the Supervisor by using this class.
 * int | double | bool f(void) - only 3 types can be handled
 */
class Value
{
public:
	/** @} */
	/**
	 * @name complete Constructors
	 * @brief while using 3 Constructors below, it can be added to the Client without any function calls
	 * @param name means the name of the value, sleep_ms_interval means sleep interval of each value in milliseconds. 
	 * @{
	 */
	/**
	 * constructor with name, integer value, min, max
	 */
	Value(const char *name, IntegerValue value, int min, int max, int sleep_ms_interval);
	/**
	* constructor with name, char* value, min, max
	*/
	Value(const char *name, StringValue value, int min, int max, int sleep_ms_interval);
	/**
	 * constructor with name, double value, min, max
	 */
	Value(const char *name, DoubleValue value, double min, double max, int sleep_ms_interval);
	/**
	 * constructor with name, bool value. bool value does not need to set min and max.
	 */
	Value(const char *name, BoolValue value, int sleep_ms_interval);
	/** @} */
	/**
	 * A destructor
	 */
	~Value();

	/**
	 * get name's pointer of the value
	 */
	char *name();

	void *value();
	bool value_changed(void *cur);

	/**
	 * internal function to set sleep interval of each values/ 
	 */
	void set_sleep_interval(const int sleep_ms_interval);

	/**
	 * internal function to get sleep interval of each values/ 
	 */
	int get_sleep_ms_interval();

	/**
	 * internal function to set last sent time of each value to publish its value periodically / 
	 */
	void set_last_sent_time();

	/**
	 * internal function to get last sent time of each value to publish its value periodically / 
	 */
	unsigned long get_last_sent_time();
	/**
	 * Get a string that contains information of the value
	 * @param buffer output buffer 
	 */
	void GetInformation(char *buffer);

	/**
	 * set a publish id(common00) that registered to the gateway
	 * returns applied publish_id
	 */
	uint16_t set_publish_id(uint16_t publish_id);
	/**
	 * get a publish id that registered to the gateway
	 */
	uint16_t publish_id();

	bool capVal2str(char *buffer);

	CapType value_classifier(void);

	char *user_string_buffer_;

private:
	/**
	* @name set value
	* @{
	* set function to get the value
	*/
	/**
	* set name of the value
	*/
	void set_name(const char *name);
	/**
	* set integer value
	*/
	void set_value(IntegerValue value);
	/**
	* set string value
	*/
	void set_value(StringValue value);
	/**
	* set double value
	*/
	void set_value(DoubleValue value);
	/**
	* set bool value
	*/
	void set_value(BoolValue value);

	/**
	* @name set minmax
	* set the minimum value or maximum of the value
	* @{
	*/
	/**
	* set the minimum value of the integer value or double value
	*/
	void set_min(const int min);
	/**
	* set the minium value of the double value (it does not allowed to the integer value)
	*/
	void set_min(const double min);
	/**
	* set the maximum value of the integer value or double value
	*/
	void set_max(const int max);
	/**
	* set the maximum value of the double value (it does not allowed to the integer value)
	*/
	void set_max(const double max);
	void Initialize();
	uint16_t publish_id_;
	const char *name_;
	void *value_;
	void *min_;
	void *max_;
	void *prev_;
	int sleep_ms_interval_;
	unsigned long last_sent_time_;
	CapType value_classifier_;
};

class Attribute
{
public:
	/**
	* @name complete Constructors
	* @brief while using 3 Constructors below, it can be added to the Client without any function calls
	* @param name means the name of the attribute
	* @{
	*/

	/**
	* constructor with name and real value.
	*/
	Attribute(const char *name, double attribute_value, CapType attribute_type);
	/**
	* constructor with name and string value.
	*/
	Attribute(const char *name, char *attribute_value, CapType attribute_type);

	/** @} */
	/**
	* A destructor
	*/
	~Attribute();

	/**
	* set name of the attribute
	*/
	void set_name(const char *name);
	/**
	* get name's pointer of the attribute
	*/
	char *name();

	/**
	* set string or binary value of the attribute
	*/
	void set_attribute_value(const char *attribute_value);

	/**
	* set real value of the attribute
	*/
	void set_attribute_value(double attribute_value);

	/**
	* get json_object* to register to the middleware, it will not be used in user level
	*/
	void GetInformation(char *buffer);

	/**
	* get type status of the attribute, it also not be used in user level
	*/
	CapType attribute_type(void);

private:
	/**
	* initailizing function
	*/
	void Initialize();
	double real_value_;
	char *string_value_;
	char *name_;			 /** < A name of the value*/
	CapType attribute_type_; /** < represents type of the value */
};

class Argument
{
public:
	/**
	* constructor for int or char array or bool argument
	* @param name means the name of the Argument
	*/
	Argument(const char *name, int min, int max, CapType arg_type);
	/**
	* constructor for double-argument function
	* @param name means the name of the function
	*/
	Argument(const char *name, double min, double max, CapType arg_type);

	/** @} */
	/**
	* A destructor
	*/
	~Argument();

	/**
	* set name of the function
	*/
	void set_name(const char *name);
	/**
	* get name's pointer of the function
	*/
	char *name();
	void *value();

	/**
	* @}
	* @name set minmax
	* set the minimum value or maximum of the value
	* @{
	*/

	/**
	* get the order of the argument
	*/
	int get_order();
	/**
	* set the order of the argument
	*/
	void set_order(const int order);

	/**
	* get type status of the function, it also not be used in user level
	*/
	CapType arg_type(void);

	void GetInformation(char *buf);

	bool Check_valid_and_set(char *val);

private:
	/**
	* set the minimum value of the argument of integer function or double function
	*/
	void set_min(const int min);
	/**
	* set the minimum value of the argument of the double function
	*/
	void set_min(const double min);
	/**
	* set the maximum value of the argument of integer function or double function
	*/
	void set_max(const int max);
	/**
	* set the maximum value of the argument of double function
	*/
	void set_max(const double max);

	/** @}
	*
	* initailizing function
	*/
	void Initialize();
	char *name_; /** < A name of the argument */
	void *value_;
	void *min_;
	void *max_;
	CapType arg_type_; /** < represents type of the argument */
	int order_;		   /** < represents order of argument */
};

/**
 * @brief A class for handling various "function" types.
 * @details A programmer can add Functions to the Supervisor by using this class.
 * void f(void | int | double | bool) - only 4 types can be handled
 */
class Function
{
public:
	/**
	 * @name incomplete Constructors
	 * @brief Name and function must be set. 
	 *        Also, if the argument type of the function is integer, double, or const char*, 
	 *        min and max must be set.
	 *        (In the case of const char*, 
	 *         min and max indicates minimum length and maximum length of the array.)
	 * @param (*function) means the function pointer
	 * @param min means the minimum argument value of the function
	 * @param max means the maximum argument value of the function
	 * @{
	 */

	/* only one type of constructor is allowed*/
	Function(const char *name, VoidFunction func, int nArguments, int nFunctionAttributes);

	/** @} */
	/**
	 * A destructor
	 */
	~Function();

	/**
	* add a function-dependent value
	*/

	void Add_argument(Argument &argument);

	void Add_functionattribute(Attribute &function_attribute);

	/**
	 * set name of the function
	 */
	void set_name(const char *name);
	/**
	 * get name's pointer of the function
	 */
	char *name();

	/**
	 * @name set function
	 * set function pointer 
	 * @{
	 */
	int ncurArguments() { return ncurArguments_; }
	Argument *getIdxArgument(int idx) { return ptsArguments_[idx]; }

	//Dowhan added
	Attribute *getIdxFunctionAttribute(int idx) { return ptsFunctionAttributes_[idx]; }
	//End
	int ncurFunctionAttributes() { return ncurFunctionAttributes_; }

	void GetInformation(char *buffer);
	void Execute(char *args, int *success) const;
	/**
	 * set a registered id of topic name from MT1003 (returns id_1003)
	 */
	uint16_t set_id_1003(uint16_t id_1003);
	/**
	 * get a registerd id of topic name from MT1003
	 */
	uint16_t id_1003();
	/**
	 * set a registered id of topic name from MT2004 (returns id_2004)
	 */
	uint16_t set_id_2004_deprecated(uint16_t id_2004);
	uint16_t set_id_2004(uint16_t id_2004);
	/**
	 * get a saved id_2004
	 */
	uint16_t id_2004_deprecated();
	uint16_t id_2004();

	CapType function_classifier(void); // for function return type

private:
	void set_function(VoidFunction func);

	void Initialize();

	const char *name_;
	void *function_;
	int nmaxArguments_;
	int ncurArguments_;
	Argument **ptsArguments_;
	//Dowhan added
	Attribute **ptsFunctionAttributes_;
	int ncurFunctionAttributes_;
	//End

	uint16_t id_1003_;
	uint16_t id_2004_deprecated_;
	uint16_t id_2004_;

	CapType function_classifier_;
};

/**
 * @brief A class for communication between thing and gateway. 
 */
/**
 * @details A programmer should declare this class and set id, set serial, add values and functions.
 * then call .Setting on setting tab, and Publish in loop().
 * it acts like as a bridge between thing and middleware.
 * it is very similar to the Class Client in thing_client. please refer that documentation.
 */

class ThingClient
{
public:
	ThingClient();
	~ThingClient();
	/**
	 * programmer can easily set client id and zigbee serial with this constructor.
	 */
	ThingClient(const char *class_name, int alive_cycle, Stream &serial);
	ThingClient(const char *class_name, Stream &serial);

	void Add(Value &v);
	void Add(Function &f);
	void Add(Attribute &a);
	/**
	 * set serial that connected to the zigbee output and input
	 */
	void set_serial(Stream &serial);
	/**
	 * set function that acts when arduino connects to the gateway
	 * @param connectHandler the void function that programmer wants to activate when arduino connects to the gateway.
	 */
	void set_connect_handler(void (*connectHandler)());
	/**
	 * set function that acts when arduino disconnects from the gateway
	 * @param disconnectHandler the void function that programmer wants to activate when arduino disconnects from the gateway.
	 */
	void set_disconnect_handler(void (*disconnectHandler)());

	/**
     * Initialize client id with class name and mac address
	 */
	void init_client_id();

	/**
     * get mac address of zigbee for client id
     */
	void get_mac_address();

	/**
     * set class name of client
	 */

	void set_class_name(const char *class_name);
	/**
	 * Call it after setting serial, setting client id, and Adding every function and value.
	 * Do not forget call this function after (zigbee serial).begin(9600)!
	 * it should be called in the setting() tab on Arduino IDE Environment.
	 */
	void Setting();

	/**
     *Send Alive message to middleware for every alive cycle
     */
	void sendAliveMessage();
	/**
	 * Call it in the loop() tab on Arduino IDE Environment.
	 */
	//void DoLoop();
	void DoLoop(int pub_period = 100);

protected:
	/*
	virtual void subscribeByName(const uint8_t flags, const char* topic_name) = 0;
	virtual	void unsubscribeByName(const uint8_t flags, const char* topic_name) = 0;
	
	
	virtual void willtopicreqHandler(const message_header* msg) = 0;
	virtual void willmsgreqHandler(const message_header* msg) = 0;

	virtual void registerHandler(const msg_register* msg) = 0;
	virtual void pubackHandler(const msg_puback* msg) = 0;

*/
#ifdef USE_QOS2
	void pubrecHandler(const msg_pubqos2 *msg);
	void pubrelHandler(const msg_pubqos2 *msg);
	void pubcompHandler(const msg_pubqos2 *msg);
#endif

	/*	
	virtual void unsubackHandler(const msg_unsuback* msg) = 0; // it is needed, but will not be used in this library
	
	virtual void willtopicrespHandler(const msg_willtopicresp* msg) = 0;
	virtual void willmsgrespHandler(const msg_willmsgresp* msg) = 0;
*/
	virtual void pingreqHandler();
	virtual void pingrespHandler();
	virtual void subackHandler(const msg_suback *msg);
	virtual void disconnectHandler(const msg_disconnect *msg);
	void regack(const uint16_t topicId, const uint16_t messageId, const return_code_t return_code);
	void puback(const uint16_t topicId, const uint16_t messageId, const return_code_t return_code);
	void subscribe(const uint8_t flags, const uint16_t topicId);
	void subscribe(const uint8_t flags, const char *name);
	void unsubscribe(const uint8_t flags, const uint16_t topicId);
	void unsubscribe(const uint8_t flags, const char *name);
	void devregHandler();
	void devregackHandler(const msg_devregack *msg);
	void regackHandler(const msg_regack *msg);
	void publishHandler(const msg_publish *msg);
	void advertiseHandler(const msg_advertise *msg);
	void gwinfoHandler(const msg_gwinfo *msg);
	void connackHandler(const msg_connack *msg);

private:
#ifdef USE_QOS2
	void pubrec(const msg_publish *msg);
	void pubrel(const msg_pubqos2 *msg);
	void pubcomp(const msg_pubqos2 *msg);
#endif
	/*	
	void willtopic(const uint8_t flags, const char* will_topic, const bool update = false);
	void willmsg(const void* will_msg, const uint8_t will_msg_len, const bool update = false);
*/

	bool waitForResponse();
	bool connected();
	bool valid();

	void searchgw(const uint8_t radius);
	void connect(const uint8_t flags, const uint16_t duration, const char *client_id_);
	bool registerTopic(const char *name);
	void publish(const uint8_t flags, const uint16_t topicId, const void *data, const uint8_t data_len);
	void devreg(void);

	void checkSerial();
	void checkSerialForValue();

	void pingreq();
	void pingresp(int flag);
	void disconnect(const uint16_t duration);

	void parseStream(char *buf, uint16_t len);
	void dispatch();
	uint16_t bswap(const uint16_t val);

	void unicast();
	void broadcast();
	void sendPacket();

	//f~ : function pointers

	bool compareTimeStamp(Value *t);

	void (*connect_handler_)();

	void (*disconnect_handler_)();

	//data members

	uint8_t num_values_;
	uint8_t num_functions_;
	uint8_t num_attributes_;

	Value *values_[CAPITAL_MAX_VALUE];
	Function *functions_[CAPITAL_MAX_FUNCTION];
	Attribute *attributes_[CAPITAL_MAX_ATTRIBUTE];

	uint16_t id_1001_;
	uint16_t id_1002_;
	uint16_t id_2001_;
	uint16_t id_2002_;
	uint16_t id_2003_;
	//

	char buffer[CAPITAL_MAX_BUFFER_SIZE];
	char save_buffer[CAPITAL_MAX_BUFFER_SIZE];
	uint8_t mac_address[10];

	const char *client_id_;
	const char *class_name_;
	unsigned long alive_cycle_;

	/////// MQTT-SN
	XBee zbee_;

	bool connected_;
	XBeeAddress64 gateway_address_64_;
	uint16_t gateway_address_16_;
	uint8_t gateway_id_;
	ZBTxRequest zbee_tx_;
	ZBRxResponse zbee_rx_;
	// Set to true when we're waiting for some sort of acknowledgement from the
	//server that will transition our state.
	bool waiting_for_response_;
	bool valid_;
	bool registered_;
	bool device_register_;
	bool in_process_; // for handling publish processing

	uint8_t response_wait_for_;

	uint16_t message_id_;

	uint8_t message_buffer_[CAPITAL_MAX_BUFFER_SIZE];

	uint32_t response_timer_;
	uint8_t response_retries_;

	uint16_t registered_id_;

	uint16_t last_ping_;
};

/**
*  return argument by its name
*/
void *GetArgumentByName(void *pData, const char *name);
/**
*  return argument by its name
*/
int GetStringArgumentByName(void *pData, const char *name, char **ppszOut);

/**
*  return argument by its name
*/
int GetIntArgumentByName(void *pData, const char *name, int *pnOut);

/**
*  return argument by its name
*/
int GetDoubleArgumentByName(void *pData, const char *name, double *pdbOut);

/**
* handle mqtt connlost mesage
*/
void brokerConnLost(void *context, char *cause);

#endif
