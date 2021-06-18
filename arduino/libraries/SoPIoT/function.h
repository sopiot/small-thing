#ifndef SMALL_THING_FUNCTION_H_
#define SMALL_THING_FUNCTION_H_

#include "argument.h"
#include "tag.h"
#include "common.h"

class Function {
 public:
  Function(const char* name, VoidFunction func, int nArguments,
           int nFunctionTags);
  ~Function();

  void AddArgument(Argument& argument);
  void AddTag(const char *tag_name);
  void AddTag(Tag& function_tag);

  void set_name(const char* name);
  char* name();

  int nmaxArguments() { return nmaxArguments_; }
  int ncurArguments() { return ncurArguments_; }
  Argument* getArgument(int idx) { return ptsArguments_[idx]; }

  int nmaxFunctionTags() { return nmaxFunctionTags_; }
  int ncurFunctionTags() { return ncurFunctionTags_; }
  Tag* getTag(int idx) { return ptsFunctionTags_[idx]; }

  void GetInformation(char* buffer);
  void Execute(char* args, int* success) const;

  uint16_t set_id_1003(uint16_t id_1003);
  uint16_t id_1003();

  uint16_t set_id_2004(uint16_t id_2004);
  uint16_t id_2004();

  SoPType function_classifier(void);  // for function return type

 private:
  void set_function(VoidFunction func);
  void Initialize();

  char* name_;
  void* function_;
  
  int nmaxArguments_;
  int ncurArguments_;
  Argument** ptsArguments_;

  int nmaxFunctionTags_;
  int ncurFunctionTags_;
  Tag** ptsFunctionTags_;

  uint16_t id_1003_;
  uint16_t id_2004_;

  SoPType function_classifier_;
};

#endif  // SMALL_THING_FUNCTION_H_
