#ifndef SMALL_THING_FUNCTION_H_
#define SMALL_THING_FUNCTION_H_

#include "argument.h"
#include "common.h"
#include "tag.h"

class Function {
 public:
  Function(const char* name, VoidFunction func);
  Function(const char* name, IntegerFunction func);
  Function(const char* name, DoubleFunction func);
  Function(const char* name, BoolFunction func);

  Function(const char* name, VoidArgumentFunction func, int nArguments);
  Function(const char* name, IntegerArgumentFunction func, int nArguments);
  Function(const char* name, DoubleArgumentFunction func, int nArguments);
  Function(const char* name, BoolArgumentFunction func, int nArguments);

  ~Function();

  void* function();
  void AddArgument(Argument& argument);
  void AddTag(const char* tag_name);
  void AddTag(Tag& function_tag);

  void SetName(const char* name);
  char* GetName();

  Argument* getArgument(int idx) { return Arguments_[idx]; }
  int getArgumentNum() { return num_argument_; }

  Tag* getTag(int idx) { return FunctionTags_[idx]; }
  int getTagNum() { return num_tag_; }

  void GetInformation(char* buffer);

  int GetArgumentOrderAndTypeByName(const char* name, int* order, int* type);
  void Execute(char* args, int* success) const;

  uint16_t set_id_1003(uint16_t id_1003);
  uint16_t id_1003();

  uint16_t set_id_2004(uint16_t id_2004);
  uint16_t id_2004();

  SoPType getReturnType(void);  // for function return type

 private:
  void set_function(VoidFunction func);
  void set_function(IntegerFunction func);
  void set_function(DoubleFunction func);
  void set_function(BoolFunction func);

  void set_function(VoidArgumentFunction func);
  void set_function(IntegerArgumentFunction func);
  void set_function(DoubleArgumentFunction func);
  void set_function(BoolArgumentFunction func);

  void Initialize();

  char* name_;
  char name2_[256];

  void* function_;
  void* return_value_;

  int num_argument_;
  Argument** Arguments_;

  Tag** FunctionTags_;
  int num_tag_;

  uint16_t id_1003_;
  uint16_t id_2004_;

  SoPType return_type_;
};

typedef struct _SUserFunctionData {
  Function* pFunction;
  JsonObject* pJsonArgumentArray;
} SUserFunctionData;

#endif  // SMALL_THING_FUNCTION_H_
