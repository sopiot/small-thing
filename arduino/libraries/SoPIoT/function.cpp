#include "function.h"

#include "utils.h"

void Function::Initialize() {
  name_ = NULL;
  function_ = NULL;
  nmaxArguments_ = 0;
  ncurArguments_ = 0;
  nMaxFunctionAttributes_ = 0;
  ncurFunctionAttributes_ = 0;
  ptsArguments_ = NULL;
  function_classifier_ = UNDEFINED;
}

Function::Function(const char *name, VoidFunction func, int nArguments,
                   int nFunctionAttributes) {
  Initialize();
  set_name(name);
  set_function(func);
  nmaxArguments_ = nArguments;
  if (nArguments > 0) {
    ptsArguments_ = (Argument **)malloc(sizeof(Argument *) * nArguments);
    MEM_ALLOC_CHECK(ptsArguments_);
  }

  nMaxFunctionAttributes_ = nFunctionAttributes;
  if (nFunctionAttributes > 0) {
    ptsFunctionAttributes_ =
        (Attribute **)malloc(sizeof(Attribute *) * nFunctionAttributes);
    MEM_ALLOC_CHECK(ptsFunctionAttributes_);
  }
}

Function::~Function() {
  if (name_) free(name_);

  for (int i = 0; i < nmaxArguments_; i++) {
    if (ptsArguments_[i] != NULL) {
      free(ptsArguments_[i]);
      ptsArguments_[i] = NULL;
    }
  }
  free(ptsArguments_);

  for (int i = 0; i < nMaxFunctionAttributes_; i++) {
    if (ptsFunctionAttributes_[i] != NULL) {
      free(ptsFunctionAttributes_[i]);
      ptsFunctionAttributes_[i] = NULL;
    }
  }
  free(ptsFunctionAttributes_);
}

void Function::AddArgument(Argument &argument) {
  // set order of argument to distinguish when receiving function request from
  // middleware
  if (ncurArguments_ >= nmaxArguments_) {
    SOPLOGLN(F("the number of arguments is exceeded"));
    return;
  }
  ptsArguments_[ncurArguments_] = &argument;
  argument.set_order(ncurArguments_++);
}

void Function::AddFunctionAttribute(Attribute &function_attribute) {
  ptsFunctionAttributes_[ncurFunctionAttributes_] = &function_attribute;
  ncurFunctionAttributes_++;
}

char *Function::name() { return (char *)name_; }

void Function::set_name(const char *name) {
  name_ = strdup(name);
  MEM_ALLOC_CHECK(name_);
}

void Function::set_function(VoidFunction func) {
  function_ = (void *)func;
  function_classifier_ = VOID;
}

void Function::Execute(char *args, int *success) const {
  int i;
  //	char *pTokPtr = NULL;
  char *target = NULL;
  if (!function_ || function_classifier_ != VOID) {
    *success = -1;
    return;
  }
  for (i = 0; i < ncurArguments_; i++) {
    target = strsep(&args, "\t");
    if (target == NULL) {
      *success = -7;  // insucfficient arguments
      return;
    }
    if (!ptsArguments_[i]->SetArgumentIfValid(target)) {
      *success = -7;  // insucfficient arguments
      return;
    }
  }
  VoidFunction executer = (VoidFunction)function_;
  SOPLOGLN(F("[DEBUG]: Execute function"));
  executer((void *)this);
  *success = 0;
}

uint16_t Function::set_id_1003(uint16_t id_1003) {
  return (id_1003_ = id_1003);
}

uint16_t Function::id_1003() { return id_1003_; }

uint16_t Function::set_id_2004_deprecated(uint16_t id_2004) {
  return (id_2004_deprecated_ = id_2004);
}

uint16_t Function::id_2004_deprecated() { return id_2004_deprecated_; }

uint16_t Function::set_id_2004(uint16_t id_2004) {
  return (id_2004_ = id_2004);
}

uint16_t Function::id_2004() { return id_2004_; }

CapType Function::function_classifier() { return function_classifier_; }

void Function::GetInformation(char *buffer) {
  int i, len;
  switch (function_classifier_) {
    case INTEGER:
    case DOUBLE:
    case BOOL:
    case VOID:
      // index =
      snprintf(buffer, MAX_BUFFER_SIZE, "%s\t%d", name_, ncurArguments_);
      break;
    default:  // error!
      SOPLOGLN(F("[ERROR] Function::GetInformation -> ERROR!"));
      break;
  }
  // SOPLOGLN(F("Get Function Information!"));
}