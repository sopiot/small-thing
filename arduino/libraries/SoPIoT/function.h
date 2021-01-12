#ifndef SMALL_THING_FUNCTION_H_
#define SMALL_THING_FUNCTION_H_

#include "argument.h"
#include "attribute.h"
#include "common.h"

class Function {
 public:
  Function(const char *name, VoidFunction func, int nArguments,
           int nFunctionAttributes);

  ~Function();

  void AddArgument(Argument &argument);

  void AddFunctionAttribute(Attribute &function_attribute);

  void set_name(const char *name);

  char *name();

  int ncurArguments() { return ncurArguments_; }

  Argument *getIdxArgument(int idx) { return ptsArguments_[idx]; }

  Attribute *getIdxFunctionAttribute(int idx) {
    return ptsFunctionAttributes_[idx];
  }

  int ncurFunctionAttributes() { return ncurFunctionAttributes_; }

  void GetInformation(char *buffer);

  void Execute(char *args, int *success) const;

  uint16_t set_id_1003(uint16_t id_1003);
  uint16_t id_1003();

  uint16_t set_id_2004_deprecated(uint16_t id_2004);
  uint16_t set_id_2004(uint16_t id_2004);

  uint16_t id_2004_deprecated();
  uint16_t id_2004();

  CapType function_classifier(void);  // for function return type

 private:
  void set_function(VoidFunction func);

  void Initialize();

  char *name_;
  void *function_;
  int nmaxArguments_;
  int ncurArguments_;
  Argument **ptsArguments_;
  int nMaxFunctionAttributes_;
  Attribute **ptsFunctionAttributes_;
  int ncurFunctionAttributes_;

  uint16_t id_1003_;
  uint16_t id_2004_deprecated_;
  uint16_t id_2004_;

  CapType function_classifier_;
};

#endif  // SMALL_THING_FUNCTION_H_
