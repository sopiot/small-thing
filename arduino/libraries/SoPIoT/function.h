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

  void* GetCallbackFunction();
  void AddArgument(Argument& argument);
  void AddTag(const char* tag_name);
  void AddTag(Tag& function_tag);

  void SetName(const char* name);
  char* GetName();

  Argument* GetArgument(int idx) { return arguments_[idx]; }
  int GetArgumentNum() { return num_argument_; }

  Tag* GetTag(int idx) { return function_tags_[idx]; }
  int GetTagNum() { return num_tag_; }

  void GetPublishData(char* buffer);

  int GetArgumentOrderAndTypeByName(const char* name, int* order, int* type);
  void Execute(char* args, int* success) const;

  uint16_t SetID1003(uint16_t id_1003);
  uint16_t GetID1003();

  uint16_t SetID2004(uint16_t id_2004);
  uint16_t GetID2004();

  SoPType GetReturnType(void);  // for function return type

 private:
  void SetFunction(VoidFunction func);
  void SetFunction(IntegerFunction func);
  void SetFunction(DoubleFunction func);
  void SetFunction(BoolFunction func);

  void SetFunction(VoidArgumentFunction func);
  void SetFunction(IntegerArgumentFunction func);
  void SetFunction(DoubleArgumentFunction func);
  void SetFunction(BoolArgumentFunction func);

  void Initialize();

  char* name_;
  void* callback_function_;
  void* return_value_;

  int num_argument_;
  Argument** arguments_;

  int num_tag_;
  Tag** function_tags_;

  uint16_t id_1003_;
  uint16_t id_2004_;

  SoPType return_type_;
};

typedef struct _SUserFunctionData {
  Function* pFunction;
  JsonObject* pJsonArgumentArray;
} SUserFunctionData;

#endif  // SMALL_THING_FUNCTION_H_
