#include "thing_client.h"

// for ARM board support
char *dtostrf_cap(double val, signed char width, unsigned char prec, char *sout)
{
#if defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_MBED)
	asm(".global _printf_float");

	char fmt[20];
	sprintf(fmt, "%%%d.%df", width, prec);
	sprintf(sout, fmt, val);
	return sout;
#else
	return dtostrf(val, width, prec, sout);
#endif
}

//  class Value
void Value::Initialize()
{
	name_ = NULL;
	value_ = NULL;
	min_ = NULL;
	max_ = NULL;
	prev_ = NULL;
	user_string_buffer_ = NULL;
	sleep_ms_interval_ = 0;
	last_sent_time_ = 0;
	value_classifier_ = UNDEFINED;
}

Value::Value(const char *name, BoolValue value, int sleep_ms_interval)
{
	Initialize();
	set_name(name);
	set_value(value);
	set_max(1);
	set_min(0);
	set_sleep_interval(sleep_ms_interval);
}

Value::Value(const char *name, IntegerValue value, int min, int max, int sleep_ms_interval)
{
	Initialize();
	set_name(name);
	set_value(value);
	set_min(min);
	set_max(max);
	set_sleep_interval(sleep_ms_interval);
}

Value::Value(const char *name, StringValue value, int min, int max, int sleep_ms_interval)
{
	Initialize();
	set_name(name);
	set_value(value);
	set_min(min);
	set_max(max);
	set_sleep_interval(sleep_ms_interval);
}

Value::Value(const char *name, DoubleValue value, double min, double max, int sleep_ms_interval)
{
	Initialize();
	set_name(name);
	set_value(value);
	set_min(min);
	set_max(max);
	set_sleep_interval(sleep_ms_interval);
}

Value::~Value()
{
}

char *Value::name()
{
	return (char *)name_;
}

void Value::set_name(const char *name)
{
	name_ = strdup(name);
}

void *Value::value(void)
{
	return value_;
}

void Value::set_value(StringValue value)
{
	value_ = (void *)value;
	value_classifier_ = STRING;
}

void Value::set_value(IntegerValue value)
{
	value_ = (void *)value;
	prev_ = malloc(sizeof(int));
	*(int *)prev_ = 0;
	value_classifier_ = INTEGER;
}

void Value::set_value(DoubleValue value)
{
	value_ = (void *)value;
	prev_ = malloc(sizeof(double));
	*(double *)prev_ = 0;
	value_classifier_ = DOUBLE;
}

void Value::set_value(BoolValue value)
{
	value_ = (void *)value;
	prev_ = malloc(sizeof(int));
	*(int *)prev_ = -1;
	value_classifier_ = BOOL;
}

bool Value::value_changed(void *cur)
{
	bool changed = false;
	switch (value_classifier_)
	{
	case STRING:
		if (strncmp((char *)cur, (char *)prev_, *(int *)max_) != 0)
			changed = true;
		memcpy(prev_, cur, *(int *)max_);
		break;
	case INTEGER:
	case BOOL:
		if (*(int *)prev_ == *(int *)cur)
		{
			changed = false;
		}
		else
		{
			changed = true;
		}
		memcpy(prev_, cur, sizeof(int));
		break;
	case DOUBLE:
		if (DOUBLE_IS_APPROX_EQUAL(*(double *)prev_, *(double *)cur))
		{
			changed = false;
		}
		else
		{
			changed = true;
		}
		memcpy(prev_, cur, sizeof(double));
		break;
	default:
		// error!
		break;
	}
	return changed;
}

void Value::set_min(const int min)
{
	if (!min_)
		min_ = malloc(sizeof(int));
	*(int *)min_ = min;
}

void Value::set_min(const double min)
{
	if (!min_)
		min_ = malloc(sizeof(double));
	*(double *)min_ = (double)min;
}

void Value::set_max(const int max)
{
	if (!max_)
		max_ = malloc(sizeof(int));
	*(int *)max_ = max;
	if (value_classifier_ == STRING)
	{
		user_string_buffer_ = (char *)malloc((max + 1) * sizeof(char));
		prev_ = malloc((max + 1) * sizeof(char));
	}
}

void Value::set_max(const double max)
{
	if (!max_)
		max_ = malloc(sizeof(double));
	*(double *)max_ = max;
}

void Value::set_sleep_interval(const int sleep_ms_interval)
{
	sleep_ms_interval_ = sleep_ms_interval;
}

int Value::get_sleep_ms_interval()
{
	return sleep_ms_interval_;
}
void Value::set_last_sent_time()
{
	last_sent_time_ = millis();
}

CapType Value::value_classifier()
{
	return value_classifier_;
}

uint16_t Value::set_publish_id(uint16_t publish_id)
{
	return (publish_id_ = publish_id);
}

uint16_t Value::publish_id()
{
	return publish_id_;
}

void Value::GetInformation(char *buffer)
{
	switch (value_classifier_)
	{
	case BOOL:
		snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, "%s\tbool\t%d\t%d", name_, *(int *)min_, *(int *)max_);
		break;
	case INTEGER:
		snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, "%s\tint\t%d\t%d", name_, *(int *)min_, *(int *)max_);
		break;
	case STRING:
		snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, "%s\tstring\t%d\t%d", name_, *(int *)min_, *(int *)max_);
		break;
	case DOUBLE:
	{
		char min_temp[10];
		char max_temp[10];
		dtostrf_cap(*(double *)min_, 8, 2, min_temp);
		dtostrf_cap(*(double *)max_, 8, 2, max_temp);
		snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, "%s\tdouble\t%s\t%s", name_, min_temp, max_temp);
		break;
	}
	default:
		// error!
		break;
	}
}

unsigned long Value::get_last_sent_time()
{
	return last_sent_time_;
}

bool Value::capVal2str(char *buffer)
{
	void *val;
	int nval;
	char dval;
	char *ptsval;
	int len = 0;
	switch (value_classifier_)
	{
	case INTEGER:
	{
		nval = ((IntegerValue)value_)();
		len = snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, "{\"type\" : \"int\" , \"value\" : %d}", nval);
		val = &nval;
		break;
	}
	case DOUBLE:
	{
		char val_temp[10];
		dval = ((DoubleValue)value_)();
		dtostrf_cap(dval, 8, 2, val_temp);
		len = snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, "{\"type\" : \"double\" , \"value\" : %s}", val_temp);
		val = &dval;
		break;
	}
	case BOOL:
	{
		nval = ((BoolValue)value_)();
		/*
			if (*(int*)val == 1) {
				len = sprintf(buffer, "{\"type\" : \"bool\" , \"value\" : 1}");
			}
			else {
				len = sprintf(buffer, "{\"type\" : \"bool\" , \"value\" : 0}");
			}*/
		len = snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, "{\"type\" : \"bool\" , \"value\" : %d}", nval);
		val = &nval;
		break;
	}
	case STRING:
	{
		ptsval = ((StringValue)value_)(user_string_buffer_, *(int *)max_);
		if (ptsval == NULL)
		{
			CPDBG(F("Fatal Error is occured on capVal2str!!\n"));
			return false;
		}
		len = snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, "{\"type\" : \"string\" , \"value\" : \"%s\"}", ptsval);
		val = ptsval;
		break;
	}
	case VOID:
	case UNDEFINED:
		CPDBG(F("CriticalError - not allowed value type"));
		break;
	}

	if (len < 0)
	{
		CPDBG(F("Fatal Error is occured on capVal2str!!\n"));
		return false;
	}

	return value_changed(val);
}

//class attribute
void Attribute::Initialize()
{
	name_ = NULL;
	string_value_ = NULL;
	real_value_ = 0;
	attribute_type_ = UNDEFINED;
}

Attribute::~Attribute()
{
}

Attribute::Attribute(const char *name, double attribute_value, CapType attribute_type)
{
	Initialize();
	attribute_type_ = attribute_type;
	set_attribute_value(attribute_value);
	set_name(name);
}

Attribute::Attribute(const char *name, char *attribute_value, CapType attribute_type)
{
	Initialize();
	attribute_type_ = attribute_type;
	set_attribute_value(attribute_value);
	set_name(name);
}

char *Attribute::name()
{
	return name_;
}

void Attribute::set_name(const char *name)
{
	name_ = strdup(name);
}

void Attribute::set_attribute_value(const char *attribute_value)
{
	string_value_ = strdup(attribute_value);
}

void Attribute::set_attribute_value(double attribute_value)
{
	real_value_ = attribute_value;
}

CapType Attribute::attribute_type(void)
{
	return attribute_type_;
}

void Attribute::GetInformation(char *buffer)
{
	switch (attribute_type_)
	{
	case BOOL:
	{
		char min_temp[10];
		dtostrf_cap(real_value_, 8, 2, min_temp);
		snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, "%s\tbool\t%s", name_, min_temp);
		break;
	}
	case INTEGER:
	{
		char min_temp[10];
		dtostrf_cap(real_value_, 8, 2, min_temp);
		snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, "%s\tint\t%s", name_, min_temp);
		break;
	}
	case DOUBLE:
	{
		char min_temp[10];
		dtostrf_cap(real_value_, 8, 2, min_temp);
		snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, "%s\tdouble\t%s", name_, min_temp);
		break;
	}
	case STRING:
		snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, "%s\tstring\t%s", name_, string_value_);
		break;
	default:
		// error!
		break;
	}
}

// Argument
void Argument::Initialize()
{
	arg_type_ = UNDEFINED;
	name_ = NULL;
	value_ = NULL;
	min_ = NULL;
	max_ = NULL;
}

Argument::Argument(const char *name, int min, int max, CapType arg_type)
{
	Initialize();
	arg_type_ = arg_type;
	set_min(min);
	set_max(max);
	set_name(name);
}

Argument::Argument(const char *name, double min, double max, CapType arg_type)
{
	Initialize();
	arg_type_ = arg_type;
	set_min(min);
	set_max(max);
	set_name(name);
}

Argument::~Argument()
{
}

int Argument::get_order()
{
	return order_;
}

void Argument::set_order(const int order)
{
	order_ = (int)order;
}

char *Argument::name()
{
	return name_;
}

void *Argument::value()
{
	return value_;
}

void Argument::set_name(const char *name)
{
	name_ = strdup(name);
	switch (arg_type_)
	{
	case BOOL:
	case INTEGER:
		value_ = malloc(sizeof(int));
		break;
	case STRING:
		value_ = malloc(sizeof(char) * (*(int *)max_ + 1));
		break;
	case DOUBLE:
		value_ = malloc(sizeof(double));
		break;
	default:
		break;
	}
}

void Argument::set_min(const int min)
{
	if (arg_type_ == INTEGER || arg_type_ == STRING || arg_type_ == BOOL)
	{
		int *i_min = (int *)malloc(sizeof(int));
		*i_min = min;
		min_ = (void *)i_min;
	}
	else if (arg_type_ == DOUBLE)
	{
		double *d_min = (double *)malloc(sizeof(double));
		*d_min = (double)min;
		min_ = (void *)d_min;
	}
}

void Argument::set_min(const double min)
{
	if (arg_type_ == DOUBLE)
	{
		double *d_min = (double *)malloc(sizeof(double));
		*d_min = min;
		min_ = (void *)d_min;
	}
	else
	{
		CPDBG(F("the type of minimum argument is not suitable\n"));
	}
}

void Argument::set_max(const int max)
{
	if (arg_type_ == INTEGER || arg_type_ == STRING || arg_type_ == BOOL)
	{
		int *i_max = (int *)malloc(sizeof(int));
		*i_max = max;
		max_ = (void *)i_max;
	}
	else if (arg_type_ == DOUBLE)
	{
		double *d_max = (double *)malloc(sizeof(double));
		*d_max = (double)max;
		max_ = (void *)d_max;
	}
}

void Argument::set_max(const double max)
{
	if (arg_type_ == DOUBLE)
	{
		double *d_max = (double *)malloc(sizeof(double));
		*d_max = max;
		max_ = (void *)d_max;
	}
	else
	{
		CPDBG(F("the type of maximum argument is not suitable\n"));
	}
}

bool Argument::Check_valid_and_set(char *val)
{
	bool flag = true;
	switch (arg_type_)
	{
	case BOOL:
	case INTEGER:
	{
		int nval = atoi(val);
		if (nval < *(int *)min_ || nval > *(int *)max_)
			flag = false;
		else
			*(int *)value_ = nval;
		break;
	}
	case DOUBLE:
	{
		double dval = atof(val);
		if (dval < *(double *)min_ || dval > *(double *)max_)
			flag = false;
		else
			*(double *)value_ = dval;
		break;
	}
	case STRING:
	{
		int len = strlen(val);
		if (len < *(int *)min_ || len > *(int *)max_)
			flag = false;
		else
			strncpy((char *)value_, val, len + 1);
		break;
	}
	default:
		break;
	}
	return flag;
}

void Argument::GetInformation(char *buffer)
{
	switch (arg_type_)
	{
	case BOOL:
		snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, "%s\tbool\t%d\t%d\t", name_, *(int *)min_, *(int *)max_);
		break;
	case INTEGER:
		snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, "%s\tint\t%d\t%d\t", name_, *(int *)min_, *(int *)max_);
		break;
	case STRING:
		snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, "%s\tstring\t%d\t%d\t", name_, *(int *)min_, *(int *)max_);
		break;
	case DOUBLE:
	{
		char min_temp[10];
		char max_temp[10];
		dtostrf_cap(*(double *)min_, 8, 2, min_temp);
		dtostrf_cap(*(double *)max_, 8, 2, max_temp);
		snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, "%s\tdouble\t%s\t%s\t", name_, min_temp, max_temp);
		break;
	}
	default:
		// error!
		break;
	}
}

CapType Argument::arg_type(void)
{
	return arg_type_;
}

// class Function
void Function::Initialize()
{
	name_ = NULL;
	function_ = NULL;
	nmaxArguments_ = 0;
	ncurArguments_ = 0;
	ncurFunctionAttributes_ = 0;
	ptsArguments_ = NULL;
	function_classifier_ = UNDEFINED;
}

Function::Function(const char *name, VoidFunction func, int nArguments, int nFunctionAttributes)
{
	Initialize();
	set_name(name);
	set_function(func);
	nmaxArguments_ = nArguments;
	if (nArguments > 0)
		ptsArguments_ = (Argument **)malloc(sizeof(Argument *) * nArguments);
	if (nFunctionAttributes > 0)
		ptsFunctionAttributes_ = (Attribute **)malloc(sizeof(Attribute *) * nFunctionAttributes);
}

Function::~Function()
{
}

void Function::Add_argument(Argument &argument)
{
	//set order of argument to distinguish when receiving function request from middleware
	if (ncurArguments_ >= nmaxArguments_)
	{
		CPDBG(F("the number of arguments is exceeded"));
		return;
	}
	ptsArguments_[ncurArguments_] = &argument;
	argument.set_order(ncurArguments_++);
}

void Function::Add_functionattribute(Attribute &function_attribute)
{
	ptsFunctionAttributes_[ncurFunctionAttributes_] = &function_attribute;
	ncurFunctionAttributes_++;
}

char *Function::name()
{
	return (char *)name_;
}

void Function::set_name(const char *name)
{
	name_ = strdup(name);
}

void Function::set_function(VoidFunction func)
{
	function_ = (void *)func;
	function_classifier_ = VOID;
}

void Function::Execute(char *args, int *success) const
{
	int i;
	//	char *pTokPtr = NULL;
	char *target = NULL;
	if (!function_ || function_classifier_ != VOID)
	{
		*success = -1;
		return;
	}
	for (i = 0; i < ncurArguments_; i++)
	{
		target = strsep(&args, "\t");
		if (target == NULL)
		{
			*success = -7; //insucfficient arguments
			return;
		}
		if (!ptsArguments_[i]->Check_valid_and_set(target))
		{
			*success = -7; //insucfficient arguments
			return;
		}
	}

	VoidFunction executer = (VoidFunction)function_;

	executer((void *)this);
	*success = 0;
}

uint16_t Function::set_id_1003(uint16_t id_1003)
{
	return (id_1003_ = id_1003);
}

uint16_t Function::id_1003()
{
	return id_1003_;
}

uint16_t Function::set_id_2004_deprecated(uint16_t id_2004)
{
	return (id_2004_deprecated_ = id_2004);
}

uint16_t Function::id_2004_deprecated()
{
	return id_2004_deprecated_;
}

uint16_t Function::set_id_2004(uint16_t id_2004)
{
	return (id_2004_ = id_2004);
}

uint16_t Function::id_2004()
{
	return id_2004_;
}

CapType Function::function_classifier()
{
	return function_classifier_;
}

void Function::GetInformation(char *buffer)
{
	//	int index = 0;
	int i, len;
	switch (function_classifier_)
	{
	case INTEGER:
	case DOUBLE:
	case BOOL:
	case VOID:
		//index =
		snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, "%s\t%d", name_, ncurArguments_);
		break;
	default: // error!
		CPDBG(F("Function::GetInformation -> ERROR!"));
		break;
	}
	CPDBG(F("Get Function Information!"));
	//CPDBG(index);
}

//class ThingClient
ThingClient::ThingClient() : waiting_for_response_(true),
							 response_wait_for_(ADVERTISE),
							 connected_(false),
							 message_id_(0),
							 gateway_id_(0),
							 valid_(0),
							 response_retries_(0),
							 registered_(0),
							 registered_id_(-1),
							 zbee_(),
							 in_process_(false)
{
	num_values_ = 0;
	num_functions_ = 0;
}

ThingClient::~ThingClient()
{
	free(client_id_);
	free(class_name_);
}

//Default alive_cycle is 60
ThingClient::ThingClient(char *class_name, Stream &serial)
{
	ThingClient(class_name, 60, serial);
}

ThingClient::ThingClient(char *class_name, int alive_cycle, Stream &serial) : waiting_for_response_(true),
																			  response_wait_for_(ADVERTISE),
																			  connected_(false),
																			  message_id_(0),
																			  gateway_id_(0),
																			  valid_(0),
																			  response_retries_(0),
																			  registered_(0),
																			  registered_id_(-1),
																			  zbee_()
{
	zbee_.setSerial(serial);
	this->class_name_ = class_name;
	this->alive_cycle_ = alive_cycle;
	response_timer_ = millis();
	memset(message_buffer_, 0, CAPITAL_MAX_BUFFER_SIZE);

	num_values_ = 0;
	num_functions_ = 0;
}

void ThingClient::set_serial(Stream &serial)
{
	zbee_.setSerial(serial);
}

void ThingClient::init_client_id()
{
	//int len = 0;
	//char temp_mac_address[17] = "";
	//int nLen = 0;

	//get_mac_address();

	// TODO:
	// FIX ME!!!!!!!!!
	client_id_ = (char *)malloc(sizeof(char) * (strlen(class_name_) + 1));
	snprintf(client_id_, strlen(class_name_) + 1, "%s", class_name_);
	// client_id_ = (char *)malloc(sizeof(char) * (strlen(class_name_) + strlen(temp_mac_address) + 2));
	// client_id_ = (char *)malloc(sizeof(char) * (100));

	// for (int i = 0; i < 8; i++)
	// {
	// 	len += sprintf(temp_mac_address + len, "%.2X", (unsigned char)mac_address[i]);
	// }

	//sprintf(client_id_, "%s_%s", class_name_, temp_mac_address);

	//nLen = strlen(client_id_);
	//CPDBG();
}

//TODO: thsvkd complete this!!!
void ThingClient::set_xbee_ID(int id)
{
	//Serial High
	uint8_t shCmd[] = {'S', 'H'};
	//Serial Low
	uint8_t slCmd[] = {'S', 'L'};
	AtCommandRequest atRequestSH = AtCommandRequest(shCmd);
	AtCommandRequest atRequestSL = AtCommandRequest(slCmd);
	AtCommandResponse atResponse = AtCommandResponse();
}

void ThingClient::get_mac_address()
{
	//Serial High
	uint8_t shCmd[] = {'S', 'H'};
	//Serial Low
	uint8_t slCmd[] = {'S', 'L'};
	AtCommandRequest atRequestSH = AtCommandRequest(shCmd);
	AtCommandRequest atRequestSL = AtCommandRequest(slCmd);
	AtCommandResponse atResponse = AtCommandResponse();

	while (1)
	{
		zbee_.send(atRequestSH);

		if (zbee_.readPacket(5000))
		{
			if (zbee_.getResponse().getApiId() == AT_COMMAND_RESPONSE)
			{
				zbee_.getResponse().getAtCommandResponse(atResponse);
				if (atResponse.isOk())
				{
					for (int i = 0; i < atResponse.getValueLength(); i++)
					{
						mac_address[i] = atResponse.getValue()[i];
					}
					break;
				}
			}
		}
		else
		{
			CPDBG(F("READ ERROR!!"));
			delay(1000);
		}
	}

	delay(1000);
	while (1)
	{
		zbee_.send(atRequestSL);

		if (zbee_.readPacket(5000))
		{
			if (zbee_.getResponse().getApiId() == AT_COMMAND_RESPONSE)
			{
				zbee_.getResponse().getAtCommandResponse(atResponse);
				if (atResponse.isOk())
				{
					for (int i = 0; i < atResponse.getValueLength(); i++)
					{
						mac_address[i + 4] = atResponse.getValue()[i];
					}
					break;
				}
			}
		}
		else
		{
			CPDBG(F("READ ERROR!!"));
			delay(1000);
		}
	}
}

void ThingClient::set_class_name(char *class_name)
{
	class_name_ = class_name;
}

void ThingClient::Add(Value &v)
{
	if (num_values_ < CAPITAL_MAX_VALUE)
	{
		values_[num_values_++] = &v;
	}
	else
	{
		CPDBG(F("It cannot be added (more than limit)\n"));
	}
}

void ThingClient::Add(Function &f)
{
	if (num_functions_ < CAPITAL_MAX_FUNCTION)
	{
		functions_[num_functions_++] = &f;
	}
	else
	{
		CPDBG(F("It cannot be added (more than limit)\n"));
	}
}

void ThingClient::Add(Attribute &a)
{
	if (num_attributes_ < CAPITAL_MAX_ATTRIBUTE)
	{
		attributes_[num_attributes_++] = &a;
	}
	else
	{
		CPDBG(F("It cannot be added (more than limit)\n"));
	}
}

void ThingClient::Setting()
{
	init_client_id();

	CPDBG(F("SEARCHGW"));
	checkSerial();
	while (!valid())
	{
		searchgw(100);
		checkSerial();
	}

	//CONNECT, CONNACK
	CPDBG(F("CONNECT"));
	while (!connected())
	{
		snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, "%s", client_id_);
		connect(0, 50, buffer);
		checkSerial();
	}

	//REGISTER, REGACK
	CPDBG(F("REGISTER"));
	do
	{
		snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, MT1001, client_id_); // id_1001_
		registerTopic(buffer);
		checkSerial();
	} while ((id_1001_ = registered_id_) == (UINT16_MAX));

	CPDBG(F("SUBSCRIBE"));
	subscribe(QOS_FLAG, buffer);
	checkSerial();

	registered_id_ = UINT16_MAX;
	CPDBG(F("REGISTER"));
	do
	{
		snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, MT1002, client_id_); // id1105
		registerTopic(buffer);
		checkSerial();
	} while ((id_1002_ = registered_id_) == (UINT16_MAX));

	CPDBG(F("SUBSCRIBE"));
	subscribe(QOS_FLAG, buffer);
	checkSerial();

	registered_id_ = UINT16_MAX;
	CPDBG(F("REGISTER"));
	do
	{
		snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, TM2001, client_id_); // id_2001_
		registerTopic(buffer);
		checkSerial();
	} while ((id_2001_ = registered_id_) == (UINT16_MAX));

	registered_id_ = UINT16_MAX;
	CPDBG(F("REGISTER"));
	do
	{
		snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, TM2002, client_id_); // id_2002_
		registerTopic(buffer);
		checkSerial();
	} while ((id_2002_ = registered_id_) == (UINT16_MAX));

	registered_id_ = UINT16_MAX;
	CPDBG(F("REGISTER"));
	do
	{
		snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, TM2003, client_id_); // id_2003_
		registerTopic(buffer);
		checkSerial();
	} while ((id_2003_ = registered_id_) == (UINT16_MAX));

	CPDBG(F("REGISTER VALUE"));
	for (int i = 0; i < num_values_; i++)
	{
		registered_id_ = UINT16_MAX;
		do
		{
			snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, COMMON0000, client_id_, values_[i]->name());
			registerTopic(buffer);
			checkSerial();

		} while ((values_[i]->set_publish_id(registered_id_)) == (UINT16_MAX));
	}

	CPDBG(F("REGISTER FUNCTION"));
	for (int i = 0; i < num_functions_; i++)
	{
		registered_id_ = UINT16_MAX;
		do
		{
			snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, MT1003, functions_[i]->name(), client_id_); // id1003
			registerTopic(buffer);
			checkSerial();
		} while ((functions_[i]->set_id_1003(registered_id_)) == (UINT16_MAX));

		subscribe(QOS_FLAG, buffer);
		checkSerial();

		registered_id_ = UINT16_MAX;
		memset(buffer, 0, CAPITAL_MAX_BUFFER_SIZE);
		do
		{
			snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, TM2004_DEPRECATED, client_id_); // id2004_deprecated
			registerTopic(buffer);
			checkSerial();
		} while ((functions_[i]->set_id_2004_deprecated(registered_id_)) == (UINT16_MAX));

		registered_id_ = UINT16_MAX;
		do
		{
			snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, TM2004, functions_[i]->name(), client_id_); // id2004
			registerTopic(buffer);
			checkSerial();
		} while ((functions_[i]->set_id_2004(registered_id_)) == (UINT16_MAX));
	}

	CPDBG(F("Registering & Subscribe_topic finished\n"));
	CPDBG(F("DEVREG start"));
	devreg();
	sendAliveMessageRightNow();
	CPDBG(F("***********Registering device to MIDDLEWARE finished***********"));
}

bool ThingClient::compareTimeStamp(Value *t)
{
	bool time_passed = false;
	unsigned long curr_time = 0;
	unsigned long diff_time = 0;
	unsigned long last_sent_time = 0;

	last_sent_time = t->get_last_sent_time();

	//set true for the initial case
	if (last_sent_time == 0)
	{
		t->set_last_sent_time();
		return true;
	}

	curr_time = millis();

	diff_time = curr_time - last_sent_time;

	//millis() goes back to zero after approximately 50 days
	if ((diff_time < 0) || (diff_time >= (unsigned long)t->get_sleep_ms_interval()))
	{
		t->set_last_sent_time();
		time_passed = true;
	}
	else
	{
		time_passed = false;
	}

	return time_passed;
}

//publish(QOS_FLAG, id_2003_, NULL, 0);
void ThingClient::sendAliveMessage()
{
	static unsigned long curr_time = 0;
	static unsigned long last_sent_time = 0;
	unsigned long diff_time = 0;
	char *pszDummy = (char *)"dummy";

	curr_time = millis();

	//set true for the initial case
	if (last_sent_time == 0)
	{
		last_sent_time = curr_time;
		return;
	}

	diff_time = curr_time - last_sent_time;

	//millis() goes back to zero after approximately 50 days
	if ((diff_time < 0) || (diff_time >= alive_cycle_ * 1000))
	{
		CPDBG(F("diff time"));
		CPDBG(diff_time);
		publish(0, id_2003_, pszDummy, strlen(pszDummy));
		last_sent_time = curr_time;
	}
	return;
}

void ThingClient::sendAliveMessageRightNow()
{
	char *pszDummy = (char *)"dummy";

	publish(0, id_2003_, pszDummy, strlen(pszDummy));
}

void ThingClient::DoLoop(int pub_period)
{
	bool time_passed = false;
	bool changed = false;

	sendAliveMessage();

	checkSerialForValue();
	for (uint8_t i = 0; i < num_values_; i++)
	{
		CPDBG(i + 1);
		CPDBG(F("Value publish start!"));
		time_passed = compareTimeStamp(values_[i]);
		changed = values_[i]->capVal2str(buffer);

		// if (time_passed && changed)
		// {
		// 	publish(QOS_FLAG, values_[i]->publish_id(), buffer, strlen(buffer));
		// }
		if (time_passed)
		{
			publish(QOS_FLAG, values_[i]->publish_id(), buffer, strlen(buffer));
			sendAliveMessageRightNow();
		}
	}
	delay(pub_period);
}

//////////////////////////////////////////////////////////////////////////

void ThingClient::checkSerial()
{
	int timeout = 1000;
	while (zbee_.readPacket(timeout))
	{
		if (zbee_.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE)
		{
			ZBTxStatusResponse txStatus;
			zbee_.getResponse().getZBTxStatusResponse(txStatus);
			if (txStatus.isSuccess())
				CPDBG(F("Send Success... waiting for response"));
			else
				CPDBG(F("Send Failed"));
		}
		else if (zbee_.getResponse().getApiId() == ZB_RX_RESPONSE)
		{
			CPDBG(F("Recv Success... parseStream start"));
			zbee_.getResponse().getZBRxResponse(zbee_rx_);
			parseStream((char *)zbee_rx_.getData(), zbee_rx_.getDataLength());
			timeout = 50;
		}
		else if (zbee_.getResponse().isError())
			CPDBG(F("Error"));
		else
			CPDBG(F("Whats up?"));
	}
}

void ThingClient::checkSerialForValue()
{
	bool isAvailable = false;

	zbee_.readPacket();
	if (zbee_.getResponse().isAvailable())
	{
		isAvailable = true;
	}
	else
	{
		isAvailable = false;
	}

	if (isAvailable)
	{
		if (zbee_.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE)
		{
			ZBTxStatusResponse txStatus;
			zbee_.getResponse().getZBTxStatusResponse(txStatus);
			if (txStatus.isSuccess())
				CPDBG(F("Send Success... waiting for response"));
			else
				CPDBG(F("Send Failed"));
		}
		else if (zbee_.getResponse().getApiId() == ZB_RX_RESPONSE)
		{
			zbee_.getResponse().getZBRxResponse(zbee_rx_);
			parseStream((char *)zbee_rx_.getData(), zbee_rx_.getDataLength());
		}
		else if (zbee_.getResponse().isError())
			CPDBG(F("Error"));
		else
			CPDBG(F("Whats up?"));
	}
}

void ThingClient::set_connect_handler(void (*connectHandler)())
{
	connect_handler_ = connectHandler;
}

void ThingClient::set_disconnect_handler(void (*disconnectHandler)())
{
	disconnect_handler_ = disconnectHandler;
}

bool ThingClient::valid()
{
	return valid_;
}

//not used!!
bool ThingClient::waitForResponse()
{
	if (waiting_for_response_)
	{
		// TODO: Watch out for overflow.
		if ((millis() - response_timer_) > (T_RETRY * 1000L))
		{
			response_timer_ = millis();
			CPDBG(F("response_failed\n"));
			if (response_retries_ == 0)
			{
				waiting_for_response_ = false;
				disconnectHandler(NULL);
			}
			else
			{
				unicast();
			}

			--response_retries_;
		}
	}

	return waiting_for_response_;
}

bool ThingClient::connected()
{
	return connected_;
}

uint16_t ThingClient::bswap(const uint16_t val)
{
	return (val << 8) | (val >> 8);
}

void ThingClient::parseStream(char *buf, uint16_t len)
{
	memcpy(message_buffer_, (const void *)buf, len);
	dispatch();
}

void ThingClient::dispatch()
{
	message_header *response_message = (message_header *)message_buffer_;
	CPDBG(F("DISPATCH STARTS"));
	switch (response_message->type)
	{
	case ADVERTISE:
		if (!waiting_for_response_ || (response_wait_for_ != ADVERTISE && response_wait_for_ != GWINFO))
			return;
		CPDBG(F("ADVERTISE"));
		advertiseHandler((msg_advertise *)message_buffer_);
		break;

	case GWINFO:
		if (response_wait_for_ != GWINFO)
			return;
		CPDBG(F("GW"));
		gwinfoHandler((msg_gwinfo *)message_buffer_);
		break;

	case CONNACK:
		if (!waiting_for_response_ || response_wait_for_ != CONNACK)
			return;
		CPDBG(F("CONNACK"));
		connackHandler((msg_connack *)message_buffer_);
		break;

	case REGACK:
		CPDBG(F("REGACK"));
		if (!waiting_for_response_ || response_wait_for_ != REGACK)
		{
			CPDBG(F("in !waiting_for_response_ || response_wait_for_ != REGACK"));
			return;
		}

		regackHandler((msg_regack *)message_buffer_);
		break;

	case PUBLISH:
		if (in_process_)
		{
			CPDBG(F("Ignore PUBLISH message for waiting previous publish message"));
			return;
		}
		CPDBG(F("PUBLISH"));
		publishHandler((msg_publish *)message_buffer_);
		break;

	case SUBACK:
		if (!waiting_for_response_ || response_wait_for_ != SUBACK)
			return;
		CPDBG(F("SUBACK"));
		subackHandler((msg_suback *)message_buffer_);
		break;

	case PINGREQ:
		CPDBG(F("PINGREQ"));
		pingreqHandler();
		break;

	case PINGRESP:
		if (!waiting_for_response_ || response_wait_for_ != PINGRESP)
			return;
		CPDBG(F("PINGRESP"));
		pingrespHandler();
		break;

	case DISCONNECT:
		CPDBG(F("DISCONNECT"));
		disconnectHandler((msg_disconnect *)message_buffer_);
		break;

	case DEVREG:
		CPDBG(F("DEVREG"));
		// cannot be happend
		devregHandler();
		break;

	case DEVREGACK:
		CPDBG(F("DEVREGACK"));
		//	if (!waiting_for_response_ || response_wait_for_ != DEVREGACK) return;
		devregackHandler((msg_devregack *)message_buffer_);
		break;

	case PUBREC:
		CPDBG(F("PUBREC"));
		pubrecHandler((msg_pubqos2 *)message_buffer_);
		break;
	case PUBREL:
		CPDBG(F("PUBREL"));
		pubrelHandler((msg_pubqos2 *)message_buffer_);
		break;
	case PUBCOMP:
		CPDBG(F("PUBCOMP"));
		pubcompHandler((msg_pubqos2 *)message_buffer_);
		break;
	default:
		CPDBG(F("default"));
		return;
	}

	waiting_for_response_ = false;
}

void ThingClient::broadcast()
{
	message_header *hdr = reinterpret_cast<message_header *>(message_buffer_);
	uint16_t addr16 = ZB_BROADCAST_ADDRESS;
	XBeeAddress64 addr64 = XBeeAddress64(0, 0xffff);

	//what is diffrence between ZB_TX_UNICAST, ZB_TX_BROADCAST??
	zbee_tx_ = ZBTxRequest(addr64, addr16, 10, ZB_TX_UNICAST, message_buffer_, hdr->length, DEFAULT_FRAME_ID);
	//zbee_tx_ = ZBTxRequest(addr64, message_buffer_, hdr->length);

	sendPacket();
}

void ThingClient::sendPacket()
{
	CPDBG(F("Send packet!"));
	//for DEBUGGING
	zbee_.send(zbee_tx_);
}

void ThingClient::unicast()
{

	if (!valid_)
	{
		broadcast();
		return;
	}

	message_header *hdr = reinterpret_cast<message_header *>(message_buffer_);

	zbee_tx_ = ZBTxRequest(gateway_address_64_, message_buffer_, hdr->length);
	sendPacket();

	if (!waiting_for_response_)
	{
		response_timer_ = millis();
		response_retries_ = N_RETRY;
	}

	/*
	int8_t hi = ((client_id_ >> 8) & 0xff);
	int8_t lo = ((client_id_ >> 0) & 0xff);
	buffer[0] = 0x7e;
	buffer[1] = hi;
	buffer[2] = lo;
	for(int i = 3; i<hdr->length+3; i++) buffer[i] = message_buffer_[i-3];
	
	Serial.write("mySerial sends message\n"));
	for(int i = 0; i< hdr->length+3; i++){CPDBG(F(" 0x")); CPDBG(F(buffer[i],HEX);}
	CPDBG(F();
	
	
	mySerial.write(buffer, hdr->length+3);
	mySerial.flush();
	*/
}

void ThingClient::advertiseHandler(const msg_advertise *msg)
{
	if (!valid_)
	{
		gateway_id_ = msg->gw_id;
		valid_ = true;
	}
}

void ThingClient::gwinfoHandler(const msg_gwinfo *msg)
{
	if (!valid_)
	{
		gateway_address_64_ = zbee_rx_.getRemoteAddress64();
		gateway_address_16_ = zbee_rx_.getRemoteAddress16();

		gateway_id_ = msg->gw_id;

		valid_ = true;
	}
}

void ThingClient::connackHandler(const msg_connack *msg)
{
	connected_ = 1;
	if (connect_handler_ != NULL)
		connect_handler_();
}

void ThingClient::regackHandler(const msg_regack *msg)
{
	CPDBG(F("in regackHandler"));
	if (msg->return_code == 0 && bswap(msg->message_id) == message_id_)
	{
		CPDBG(F("in regackHandler if"));
		registered_id_ = bswap(msg->topic_id);
	}
	CPDBG(F("in regackHandler if end"));
}

void ThingClient::disconnectHandler(const msg_disconnect *msg)
{
	if (connected_ && disconnect_handler_ != NULL)
		disconnect_handler_();
	connected_ = false;
	valid_ = false;
	registered_ = false;
}

void ThingClient::publishHandler(const msg_publish *msg)
{

	return_code_t ret = REJECTED_INVALID_TOPIC_ID;
	CPDBG(bswap(msg->topic_id));
	uint16_t topic_id = bswap(msg->topic_id);

	if (topic_id == id_1001_)
	{
		snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, "%s", msg->data);
		char errNo = buffer[0];
		CPDBG(F("Error no :"));
		CPDBG(buffer);
		if (errNo == '0')
			registered_ = !registered_;
		else if (strcmp(buffer, "-4") == 0)
		{
			CPDBG(F("Ignore duplicated error message from middleware"));
			registered_ = !registered_;
			CPDBG(registered_);
		}
		else
		{
			//do nothing
		}
		in_process_ = true;
		if (msg->flags & FLAG_QOS_2)
		{
			ret = ACCEPTED;
			pubrec(msg);
		}
		in_process_ = false;
		return;
	}
	/*
	if (topic_id == id_1002_)
	{
		publish(QOS_FLAG, id_2003_, NULL, 0);
		return ACCEPTED;
	}*/

	for (int i = 0; i < num_functions_; i++)
	{
		CPDBG(functions_[i]->id_1003());
		if (topic_id == functions_[i]->id_1003())
		{
			int success = -1;
			char *pTokPtr = NULL;
			char *t_name = NULL;
			char *t_args = NULL;
			in_process_ = true;
			strncpy(save_buffer, msg->data, CAPITAL_MAX_BUFFER_SIZE - sizeof(msg_publish)); // safe cpy
			if (msg->flags & FLAG_QOS_2)
			{
				ret = ACCEPTED;
				pubrec(msg);
			}
			in_process_ = false;
			t_name = strtok_r(save_buffer, ":", &pTokPtr);
			t_args = strtok_r(NULL, ":", &pTokPtr);
			switch (functions_[i]->function_classifier())
			{
			case VOID:
				functions_[i]->Execute(t_args, &success);
				break;
			case INTEGER:
			case DOUBLE:
			case BOOL:
			default:
				success = -7;
				break; // cannot be occured
			}
			if (t_name[0] == ' ') // No scenario name provided
			{
				snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, "{\"function\" : \"%s\" , \"error\" : %d }", functions_[i]->name(), success);
				publish(QOS_FLAG, functions_[i]->id_2004_deprecated(), buffer, strlen(buffer));
			}
			else // scenario name provided
			{
				snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, "{\"scenario\" : \"%s\" , \"error\" : %d }", t_name, success);
				publish(QOS_FLAG, functions_[i]->id_2004(), buffer, strlen(buffer));
			}

			return;
		}
	}
	in_process_ = false;
}

void ThingClient::subackHandler(const msg_suback *msg)
{
}

void ThingClient::searchgw(const uint8_t radius)
{
	msg_searchgw *msg = reinterpret_cast<msg_searchgw *>(message_buffer_);

	msg->length = sizeof(msg_searchgw);
	msg->type = SEARCHGW;
	msg->radius = radius;

	broadcast();
	waiting_for_response_ = true;
	response_wait_for_ = GWINFO;
}

void ThingClient::connect(const uint8_t flags, const uint16_t duration, const char *client_id_)
{
	msg_connect *msg = reinterpret_cast<msg_connect *>(message_buffer_);

	msg->length = sizeof(msg_connect) + strlen(client_id_);
	msg->type = CONNECT;
	msg->flags = flags;
	msg->protocol_id = PROTOCOL_ID;
	msg->duration = bswap(duration);
	strcpy(msg->client_id, client_id_);

	unicast();
	connected_ = false;
	waiting_for_response_ = true;
	response_wait_for_ = CONNACK;
}

void ThingClient::disconnect(const uint16_t duration)
{
	msg_disconnect *msg = reinterpret_cast<msg_disconnect *>(message_buffer_);

	msg->length = sizeof(message_header);
	msg->type = DISCONNECT;

	if (duration > 0)
	{
		msg->length += sizeof(msg_disconnect);
		msg->duration = bswap(duration);
	}

	unicast();
	waiting_for_response_ = true;
}

bool ThingClient::registerTopic(const char *name)
{
	CPDBG(F("in registerTopic"));
	if (!waiting_for_response_)
	{
		++message_id_;

		// Fill in the next table entry, but we only increment the counter to
		// the next topic when we get a REGACK from the broker. So don't issue
		// another REGISTER until we have resolved this one.

		registered_id_ = -1;
		msg_register *msg = reinterpret_cast<msg_register *>(message_buffer_);

		msg->length = sizeof(msg_register) + strlen(name);
		msg->type = REGISTER;
		msg->topic_id = 0;
		msg->message_id = bswap(message_id_);
		strcpy(msg->topic_name, name);

		unicast();
		waiting_for_response_ = true;
		response_wait_for_ = REGACK;
		return true;
	}
	else
	{
		CPDBG(F("waiting_for_response_ is false error!"));
	}

	return false;
}

void ThingClient::publish(const uint8_t flags, const uint16_t topicId, const void *data, const uint8_t data_len)
{
	++message_id_;

	msg_publish *msg = reinterpret_cast<msg_publish *>(message_buffer_);

	msg->length = sizeof(msg_publish) + data_len;
	msg->type = PUBLISH;
	msg->flags = flags;
	msg->topic_id = bswap(topicId);
	msg->message_id = bswap(message_id_);
	memcpy(msg->data, data, data_len);

	unicast();

	if ((flags & QOS_MASK) == FLAG_QOS_2)
	{
		waiting_for_response_ = true;
		response_wait_for_ = PUBREC;
		while (true)
		{
			checkSerial();
			if (response_wait_for_ == PUBREC)
			{
				msg->length = sizeof(msg_publish) + data_len;
				msg->type = PUBLISH;
				msg->flags = flags;
				msg->flags |= FLAG_DUP;
				msg->topic_id = bswap(topicId);
				msg->message_id = bswap(message_id_);
				memcpy(msg->data, data, data_len);
				unicast();
			}
			else
				break;
		}
	}
}

void ThingClient::devreg()
{
	msg_devreg *msg = reinterpret_cast<msg_devreg *>(message_buffer_);
	response_wait_for_ = DEVREGACK;
	waiting_for_response_ = true;
	for (int i = 0; i < num_values_; i++)
	{
		device_register_ = false;
		while (!device_register_)
		{
			message_id_++;
			msg->type = DEVREG;
			msg->pub_id = bswap(i);
			msg->status = VALUE;
			msg->message_id = bswap(message_id_);
			values_[i]->GetInformation(buffer);
			CPDBG(buffer);
			memcpy(msg->data, buffer, strlen(buffer) + 1);
			msg->length = sizeof(msg_devreg) + strlen(buffer) + 1;
			unicast();
			checkSerial();
		}
	}
	for (int i = 0; i < num_attributes_; i++)
	{
		device_register_ = false;
		while (!device_register_)
		{
			message_id_++;
			msg->type = DEVREG;
			msg->pub_id = bswap(i);
			msg->status = ATTRIBUTE;
			msg->message_id = bswap(message_id_);
			attributes_[i]->GetInformation(buffer);
			CPDBG(buffer);
			memcpy(msg->data, buffer, strlen(buffer) + 1);
			msg->length = sizeof(msg_devreg) + strlen(buffer) + 1;
			unicast();
			checkSerial();
		}
	}
	CPDBG("num_functions_ : ");
	CPDBG(num_functions_);
	for (int i = 0; i < num_functions_; i++)
	{
		device_register_ = false;
		while (!device_register_)
		{
			message_id_++;
			msg->type = DEVREG;
			msg->pub_id = bswap(i);
			msg->status = FUNCTION;
			msg->message_id = bswap(message_id_);
			functions_[i]->GetInformation(buffer);
			CPDBG(buffer);
			memcpy(msg->data, buffer, strlen(buffer) + 1);
			msg->length = sizeof(msg_devreg) + strlen(buffer) + 1;
			unicast();
			checkSerial();
		}

		for (int j = 0; j < functions_[i]->ncurArguments(); j++)
		{
			device_register_ = false;
			while (!device_register_)
			{
				message_id_++;
				msg->type = DEVREG;
				msg->pub_id = bswap(j);
				msg->status = ARGUMENT;
				msg->message_id = bswap(message_id_);
				functions_[i]->getIdxArgument(j)->GetInformation(buffer);
				memcpy(msg->data, buffer, strlen(buffer) + 1);
				msg->length = sizeof(msg_devreg) + strlen(buffer) + 1;
				unicast();
				checkSerial();
			}
		}
		for (int j = 0; j < functions_[i]->ncurFunctionAttributes(); j++)
		{
			device_register_ = false;
			while (!device_register_)
			{
				message_id_++;
				msg->type = DEVREG;
				msg->pub_id = bswap(j);
				msg->status = FUNCTION_ATTRIBUTE;
				msg->message_id = bswap(message_id_);
				functions_[i]->getIdxFunctionAttribute(j)->GetInformation(buffer);
				memcpy(msg->data, buffer, strlen(buffer) + 1);
				msg->length = sizeof(msg_devreg) + strlen(buffer) + 1;
				unicast();
				checkSerial();
			}
		}
	}
	//set alive cycle
	device_register_ = false;
	while (!device_register_)
	{
		message_id_++;
		msg->type = DEVREG;
		msg->pub_id = 0;
		msg->status = DURATION;
		msg->message_id = bswap(message_id_);
		snprintf(buffer, CAPITAL_MAX_BUFFER_SIZE, "%ul", alive_cycle_);
		CPDBG(buffer);
		memcpy(msg->data, buffer, strlen(buffer) + 1);
		msg->length = sizeof(msg_devreg) + strlen(buffer) + 1;
		unicast();
		checkSerial();
	}
	// last
	registered_ = false;
	while (!registered_)
	{
		CPDBG("start of FINISH state");
		message_id_++;
		msg->length = sizeof(msg_devreg);
		msg->type = DEVREG;
		CPDBG(F("ID2001:"));
		CPDBG(id_2001_);
		msg->pub_id = bswap(id_2001_); //////////////////////////////
		msg->message_id = bswap(message_id_);
		msg->length = sizeof(msg_devreg);
		msg->status = FINISH;
		unicast();
		checkSerial();
		CPDBG(F("end of FINISH state"));
		CPDBG(registered_);
	}
	waiting_for_response_ = false;
	CPDBG(F("out of FINISH state"));
}

void ThingClient::devregackHandler(const msg_devregack *msg)
{
	CPDBG("devregackHandler start");
	device_register_ = true;
}

void ThingClient::devregHandler()
{
	// do nothing
}

void ThingClient::subscribe(const uint8_t flags, const uint16_t topicId)
{
	++message_id_;

	msg_subscribe *msg = reinterpret_cast<msg_subscribe *>(message_buffer_);

	msg->length = sizeof(msg_subscribe);
	msg->type = SUBSCRIBE;
	//msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_PREDEFINED_ID;
	msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_NAME;
	msg->message_id = bswap(message_id_);
	msg->topic_id = bswap(topicId);

	unicast();

	if ((flags & QOS_MASK) == FLAG_QOS_1 || (flags & QOS_MASK) == FLAG_QOS_2)
	{
		waiting_for_response_ = true;
		response_wait_for_ = SUBACK;
	}
}

//for debug
void ThingClient::print_message_buffer_(int start, int length)
{
	//print message_buffer_ BYTE message_buffer_[start] to message_buffer_[start + length]
	if (length == -1)
		length = CAPITAL_MAX_BUFFER_SIZE;
	for (int i = start; i < start + length; i++)
	{
		Serial.print(message_buffer_[i], HEX);
		Serial.print(" ");
	}
	Serial.println("");
}

void ThingClient::print_message_buffer_(void *buf, int length)
{
	//print message_buffer_ BYTE *buf to *(buf + length)
	char *tmp = (char *)buf;
	if (length == -1)
		length = CAPITAL_MAX_BUFFER_SIZE;
	for (int i = 0; i < length; i++)
	{
		Serial.print(*(tmp + i), HEX);
		Serial.print(" ");
	}
	Serial.println("");
}

void ThingClient::print_message_buffer_()
{
	//print all message_buffer_ BYTE
	for (int i = 0; i < CAPITAL_MAX_BUFFER_SIZE; i++)
	{
		Serial.print(message_buffer_[i], HEX);
		Serial.print(" ");
	}
	Serial.println("");
}
void ThingClient::subscribe(const uint8_t flags, const char *name)
{
	++message_id_;

	msg_subscribe *msg = reinterpret_cast<msg_subscribe *>(message_buffer_);

	msg->length = sizeof(msg_subscribe) + strlen(name) - sizeof(uint16_t);
	msg->type = SUBSCRIBE;
	//msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_PREDEFINED_ID;
	msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_NAME;
	msg->message_id = bswap(message_id_);
	msg->topic_id = 0;
	strcpy(msg->topic_name, name);

	unicast();

	if ((flags & QOS_MASK) == FLAG_QOS_1 || (flags & QOS_MASK) == FLAG_QOS_2)
	{
		waiting_for_response_ = true;
		response_wait_for_ = SUBACK;
	}
}

void ThingClient::pingresp(int flag)
{
	message_header *msg = reinterpret_cast<message_header *>(message_buffer_);
	msg->length = sizeof(message_header);
	msg->type = PINGRESP;

	if (flag == 0)
		unicast();
	else
		broadcast();
}

void ThingClient::unsubscribe(const uint8_t flags, const char *name)
{
	++message_id_;

	msg_unsubscribe *msg = reinterpret_cast<msg_unsubscribe *>(message_buffer_);

	msg->length = sizeof(msg_unsubscribe);
	msg->type = UNSUBSCRIBE;
	//msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_PREDEFINED_ID;
	msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_NAME;
	msg->message_id = bswap(message_id_);
	msg->topic_id = 0;
	strcpy(msg->topic_name, name);

	unicast();

	if ((flags & QOS_MASK) == FLAG_QOS_1 || (flags & QOS_MASK) == FLAG_QOS_2)
	{
		waiting_for_response_ = true;
		response_wait_for_ = UNSUBACK;
	}
}

void ThingClient::unsubscribe(const uint8_t flags, const uint16_t topicId)
{
	++message_id_;

	msg_unsubscribe *msg = reinterpret_cast<msg_unsubscribe *>(message_buffer_);

	msg->length = sizeof(msg_unsubscribe);
	msg->type = UNSUBSCRIBE;
	//msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_PREDEFINED_ID;
	msg->flags = (flags & QOS_MASK) | FLAG_TOPIC_NAME;
	msg->message_id = bswap(message_id_);
	msg->topic_id = bswap(topicId);

	unicast();

	if ((flags & QOS_MASK) == FLAG_QOS_1 || (flags & QOS_MASK) == FLAG_QOS_2)
	{
		waiting_for_response_ = true;
		response_wait_for_ = UNSUBACK;
	}
}

void ThingClient::pingreq()
{
	msg_pingreq *msg = reinterpret_cast<msg_pingreq *>(message_buffer_);
	msg->length = sizeof(msg_pingreq) + strlen(client_id_);
	msg->type = PINGREQ;
	strcpy(msg->client_id, client_id_);

	unicast();
	last_ping_ =
		waiting_for_response_ = true;
	response_wait_for_ = PINGRESP;
}

void ThingClient::pingreqHandler()
{
	if (zbee_rx_.getRemoteAddress16() == gateway_address_16_)
		pingresp(0);
	else
		pingresp(1);
}

void ThingClient::pingrespHandler()
{
}

int GetStringArgumentByName(void *pData, const char *name, char **ppszOut)
{
	char *pszData = (char *)GetArgumentByName(pData, name);

	if (pszData == NULL)
	{
		return -1;
	}

	*ppszOut = pszData;
	return 0;
}

int GetIntArgumentByName(void *pData, const char *name, int *pnOut)
{
	int *pnData = (int *)GetArgumentByName(pData, name);
	if (pnData == NULL)
	{
		return -1;
	}
	*pnOut = *pnData;
	return 0;
}

int GetDoubleArgumentByName(void *pData, const char *name, double *pdbOut)
{
	double *pdbData = (double *)GetArgumentByName(pData, name);

	if (pdbData == NULL)
	{
		return -1;
	}

	*pdbOut = *pdbData;
	return 0;
}

void *GetArgumentByName(void *pData, const char *name)
{
	Function *function = (Function *)pData;
	int idx = 0;
	int nargs = function->ncurArguments();
	Argument *ptrarg;
	for (idx = 0; idx < nargs; idx++)
	{
		ptrarg = function->getIdxArgument(idx);
		if (strcmp(name, ptrarg->name()) == 0)
		{
			return ptrarg->value();
		}
	}
	if (idx == nargs)
		return NULL; // not found
}

#ifdef USE_QOS2
void ThingClient::pubrec(const msg_publish *recv_msg)
{
	waiting_for_response_ = true;
	response_wait_for_ = PUBREL;
	while (response_wait_for_ == PUBREL)
	{
		msg_pubqos2 *msg = reinterpret_cast<msg_pubqos2 *>(message_buffer_);
		msg->length = sizeof(msg_pubqos2);
		msg->type = PUBREC;
		msg->message_id = recv_msg->message_id;

		//		CPDBG(F(msg->length);
		//		CPDBG(F(msg->type);
		//		CPDBG(F(msg->message_id);

		unicast();
		checkSerial();
	}
}

void ThingClient::pubrel(const msg_pubqos2 *recv_msg)
{

	waiting_for_response_ = true;
	response_wait_for_ = PUBCOMP;
	while (response_wait_for_ == PUBCOMP)
	{
		msg_pubqos2 *msg = reinterpret_cast<msg_pubqos2 *>(message_buffer_);
		msg->length = sizeof(msg_pubqos2);
		msg->type = PUBREL;
		msg->message_id = recv_msg->message_id;
		unicast();
		checkSerial();
	}
}

void ThingClient::pubcomp(const msg_pubqos2 *recv_msg)
{
	msg_pubqos2 *msg = reinterpret_cast<msg_pubqos2 *>(message_buffer_);
	msg->length = sizeof(msg_pubqos2);
	msg->type = PUBCOMP;
	msg->message_id = recv_msg->message_id;

	unicast();
}

void ThingClient::pubrecHandler(const msg_pubqos2 *msg)
{
	pubrel(msg);
}

void ThingClient::pubrelHandler(const msg_pubqos2 *msg)
{
	waiting_for_response_ = false;
	response_wait_for_ = PUBLISH;
	pubcomp(msg);
}

void ThingClient::pubcompHandler(const msg_pubqos2 *msg)
{
	waiting_for_response_ = false;
	response_wait_for_ = PUBLISH;
}

// thsvkd add(2021-01-06)
void dp(const char* format, ...) {
    va_list ap;
    char buf[MAX_DEBUG_LOG_SIZE];

    va_start(ap, format);
    vsprintf(buf, format, ap);
    va_end(ap);
	Serial.print(buf);
}

void dlp(const char* format, ...) {
    va_list ap;
	char buf[MAX_DEBUG_LOG_SIZE];

    va_start(ap, format);
    vsprintf(buf, format, ap);
    va_end(ap);
	Serial.println(buf);
}
#endif
