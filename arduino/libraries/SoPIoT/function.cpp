#include "function.h"
#include "utils.h"

void Function::Initialize() {
  name_ = NULL;
  function_ = NULL;
  num_argument_ = 0;
  num_tag_ = 0;
  Arguments_ = NULL;
  return_type_ = UNDEFINED;
  FunctionTags_ = (Tag**)malloc(sizeof(Tag*));
}

Function::Function(const char* name, VoidFunction func) {
  Initialize();
  SetName(name);
  set_function(func);
  num_argument_ = 0;
}
Function::Function(const char* name, IntegerFunction func) {
  Initialize();
  SetName(name);
  set_function(func);
  num_argument_ = 0;
}
Function::Function(const char* name, DoubleFunction func) {
  Initialize();
  SetName(name);
  set_function(func);
  num_argument_ = 0;
}
Function::Function(const char* name, BoolFunction func) {
  Initialize();
  SetName(name);
  set_function(func);
  num_argument_ = 0;
}

Function::Function(const char* name, VoidArgumentFunction func,
                   int num_argument) {
  Initialize();
  SetName(name);
  set_function(func);
}

Function::Function(const char* name, IntegerArgumentFunction func,
                   int num_argument) {
  Initialize();
  SetName(name);
  set_function(func);
}

Function::Function(const char* name, DoubleArgumentFunction func,
                   int num_argument) {
  Initialize();
  SetName(name);
  set_function(func);
}

Function::Function(const char* name, BoolArgumentFunction func,
                   int num_argument) {
  Initialize();
  SetName(name);
  set_function(func);
}

void Function::set_function(VoidFunction func) {
  function_ = (void*)func;
  return_type_ = VOID;
}

void Function::set_function(IntegerFunction func) {
  function_ = (void*)func;
  return_type_ = INTEGER;
}

void Function::set_function(DoubleFunction func) {
  function_ = (void*)func;
  return_type_ = DOUBLE;
}

void Function::set_function(BoolFunction func) {
  function_ = (void*)func;
  return_type_ = BOOL;
}

void Function::set_function(VoidArgumentFunction func) {
  function_ = (void*)func;
  return_type_ = VOID;
}

void Function::set_function(IntegerArgumentFunction func) {
  function_ = (void*)func;
  return_type_ = INTEGER;
}

void Function::set_function(DoubleArgumentFunction func) {
  function_ = (void*)func;
  return_type_ = DOUBLE;
}

void Function::set_function(BoolArgumentFunction func) {
  function_ = (void*)func;
  return_type_ = BOOL;
}

Function::~Function() {
  if (name_) free(name_);
}

void* Function::function() { return function_; }

void Function::AddArgument(Argument& argument) {
  // set order of argument to distinguish when receiving function request from
  // middleware

  if (num_argument_ == 0) {
    Arguments_ = (Argument**)malloc(sizeof(Argument*));
    MEM_ALLOC_CHECK(Arguments_);
  } else {
    realloc(Arguments_, sizeof(Argument*) * (num_argument_ + 1));
  }
  Arguments_[num_argument_] = new Argument();
  MEM_ALLOC_CHECK(Arguments_[num_argument_]);
  *Arguments_[num_argument_] = argument;
  Arguments_[num_argument_]->set_order(num_argument_);
  num_argument_++;
}

void Function::AddTag(const char* tag_name) {
  Tag* tag = new Tag(tag_name);
  AddTag(*tag);
}

void Function::AddTag(Tag& function_tag) {
  Tag* tag = new Tag(function_tag.GetName());
  if (num_tag_ == 0) {
    FunctionTags_ = new Tag*[1];
    MEM_ALLOC_CHECK(FunctionTags_);
  } else {
    FunctionTags_ =
        (Tag**)realloc(FunctionTags_, sizeof(Tag*) * (num_tag_ + 1));
  }
  FunctionTags_[num_tag_] = &function_tag;
  MEM_ALLOC_CHECK(FunctionTags_[num_tag_]);
  num_tag_++;
}

char* Function::GetName() { return (char*)name_; }

void Function::SetName(const char* name) {
  name_ = strdup(name);
  MEM_ALLOC_CHECK(name_);
}

int Function::GetArgumentOrderAndTypeByName(const char* name, int* order,
                                            int* type) {
  for (int i = 0; i < num_argument_; i++) {
    if (strcmp(Arguments_[i]->GetName(), name) == 0) {
      *order = Arguments_[i]->GetOrder();
      *type = Arguments_[i]->GetArgType();
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
  if (!function_ || return_type_ == UNDEFINED) {
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
    if (!Arguments_[i]->SetArgumentIfValid(target)) {
      SOPLOGLN("SetArgumentIfValid: insucfficient arguments");
      *success = -7;  // insucfficient arguments
      return;
    }
  }
  VoidArgumentFunction executer = (VoidArgumentFunction)function_;
  SOPLOGLN(F("[DEBUG]: Execute function"));
  executer((void*)this);
  *success = 0;
}

uint16_t Function::set_id_1003(uint16_t id_1003) {
  return (id_1003_ = id_1003);
}

uint16_t Function::id_1003() { return id_1003_; }

uint16_t Function::set_id_2004(uint16_t id_2004) {
  return (id_2004_ = id_2004);
}

uint16_t Function::id_2004() { return id_2004_; }

SoPType Function::getReturnType() { return return_type_; }

void Function::GetInformation(char* buffer) {
  int i, len;
  switch (return_type_) {
    case INTEGER:
    case DOUBLE:
    case BOOL:
    case VOID:
      snprintf(buffer, MAX_BUFFER_SIZE, "%s#%s#%d", name_, "void",
               num_argument_);
      break;
    default:
      SOPLOGLN(F("[ERROR] Function::GetInformation -> ERROR!"));
      break;
  }
  // SOPLOGLN(F("Get Function Information!"));
}