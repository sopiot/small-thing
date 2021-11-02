#include "function.h"
#include "utils.h"

void Function::Initialize() {
  callback_function_ = NULL;
  num_argument_ = 0;
  num_tag_ = 0;
  arguments_ = NULL;
  return_type_ = UNDEFINED;
  function_tags_ = (Tag**)malloc(sizeof(Tag*));
}

Function::Function(const char* name, VoidFunction func) {
  Initialize();
  SetName(name);
  SetFunction(func);
  num_argument_ = 0;
}
Function::Function(const char* name, IntegerFunction func) {
  Initialize();
  SetName(name);
  SetFunction(func);
  num_argument_ = 0;
}
Function::Function(const char* name, DoubleFunction func) {
  Initialize();
  SetName(name);
  SetFunction(func);
  num_argument_ = 0;
}
Function::Function(const char* name, BoolFunction func) {
  Initialize();
  SetName(name);
  SetFunction(func);
  num_argument_ = 0;
}

Function::Function(const char* name, VoidArgumentFunction func,
                   int num_argument) {
  Initialize();
  SetName(name);
  SetFunction(func);
}

Function::Function(const char* name, IntegerArgumentFunction func,
                   int num_argument) {
  Initialize();
  SetName(name);
  SetFunction(func);
}

Function::Function(const char* name, DoubleArgumentFunction func,
                   int num_argument) {
  Initialize();
  SetName(name);
  SetFunction(func);
}

Function::Function(const char* name, BoolArgumentFunction func,
                   int num_argument) {
  Initialize();
  SetName(name);
  SetFunction(func);
}

void Function::SetFunction(VoidFunction func) {
  callback_function_ = (void*)func;
  return_type_ = VOID;
}

void Function::SetFunction(IntegerFunction func) {
  callback_function_ = (void*)func;
  return_type_ = INTEGER;
}

void Function::SetFunction(DoubleFunction func) {
  callback_function_ = (void*)func;
  return_type_ = DOUBLE;
}

void Function::SetFunction(BoolFunction func) {
  callback_function_ = (void*)func;
  return_type_ = BOOL;
}

void Function::SetFunction(VoidArgumentFunction func) {
  callback_function_ = (void*)func;
  return_type_ = VOID;
}

void Function::SetFunction(IntegerArgumentFunction func) {
  callback_function_ = (void*)func;
  return_type_ = INTEGER;
}

void Function::SetFunction(DoubleArgumentFunction func) {
  callback_function_ = (void*)func;
  return_type_ = DOUBLE;
}

void Function::SetFunction(BoolArgumentFunction func) {
  callback_function_ = (void*)func;
  return_type_ = BOOL;
}

Function::~Function() {
  if (name_) free(name_);
}

void* Function::GetCallbackFunction() { return callback_function_; }

void Function::AddArgument(Argument& argument) {
  arguments_ =
      (Argument**)realloc(arguments_, sizeof(Argument*) * (num_argument_ + 1));

  arguments_[num_argument_] = &argument;
  MEM_ALLOC_CHECK(arguments_[num_argument_]);
  arguments_[num_argument_]->SetOrder(num_argument_);
  num_argument_++;
}

void Function::AddTag(const char* tag_name) {
  Tag* tag = new Tag(tag_name);
  AddTag(*tag);
}

void Function::AddTag(Tag& function_tag) {
  function_tags_ =
      (Tag**)realloc(function_tags_, sizeof(Tag*) * (num_tag_ + 1));

  function_tags_[num_tag_] = &function_tag;
  MEM_ALLOC_CHECK(function_tags_[num_tag_]);
  num_tag_++;
}

char* Function::GetName() { return (char*)name_; }

void Function::SetName(const char* name) {
  strcpy(name_, name);
  MEM_ALLOC_CHECK(name_);
}

int Function::GetArgumentOrderAndTypeByName(const char* name, int* order,
                                            int* type) {
  for (int i = 0; i < num_argument_; i++) {
    if (strcmp(arguments_[i]->GetName(), name) == 0) {
      *order = arguments_[i]->GetOrder();
      *type = arguments_[i]->GetArgumentType();
      return 0;
    }
  }
  // If not found
  return -1;
}

void Function::Execute(char* args, int* success) const {
  int i;
  //	char *pTokPtr = NULL;
  char* target = NULL;
  SOPLOGLN("return_type_ : %d", return_type_);
  if (!callback_function_ || return_type_ == UNDEFINED) {
    *success = -1;
    return;
  }
  for (i = 0; i < num_argument_; i++) {
    target = strsep(&args, "#");
    if (target == NULL) {
      SOPLOGLN("insucfficient arguments");
      *success = -7;  // insucfficient arguments
      return;
    }
    if (!arguments_[i]->SetArgumentIfValid(target)) {
      SOPLOGLN("SetArgumentIfValid: insucfficient arguments");
      *success = -7;  // insucfficient arguments
      return;
    }
  }
  VoidArgumentFunction executer = (VoidArgumentFunction)callback_function_;
  SOPLOGLN(F("[DEBUG]: Execute function"));
  executer((void*)this);
  *success = 0;
}

uint16_t Function::SetID1003(uint16_t id_1003) { return (id_1003_ = id_1003); }

uint16_t Function::GetID1003() { return id_1003_; }

uint16_t Function::SetID2004(uint16_t id_2004) { return (id_2004_ = id_2004); }

uint16_t Function::GetID2004() { return id_2004_; }

SoPType Function::GetReturnType() { return return_type_; }

void Function::GetRegisterPublishData(char* buffer) {
  int i, len;
  switch (return_type_) {
    case INTEGER:
    case DOUBLE:
    case BOOL:
    case VOID:

      // snprintf(buffer, MAX_BUFFER_SIZE, "%s%s#%s#%d", name_, name2_, "void",
      //          num_argument_);
      snprintf(buffer, MAX_BUFFER_SIZE, "%s#%s#%d", name_, "void",
               num_argument_);
      break;
    default:
      SOPLOGLN(F("[ERROR] Function::GetInformation -> ERROR!"));
      break;
  }
  // SOPLOGLN(F("Get Function Information!"));
}