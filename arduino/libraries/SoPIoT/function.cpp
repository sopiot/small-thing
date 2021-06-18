#include "function.h"
#include "utils.h"

void Function::Initialize() {
  name_ = NULL;
  function_ = NULL;
  nmaxArguments_ = 0;
  ncurArguments_ = 0;
  nmaxFunctionTags_ = 0;
  ncurFunctionTags_ = 0;
  ptsArguments_ = NULL;
  function_classifier_ = UNDEFINED;
}

Function::Function(const char* name, VoidFunction func, int nArguments,
                   int nFunctionTags) {
  Initialize();
  set_name(name);
  set_function(func);
  nmaxArguments_ = nArguments;
  if (nArguments > 0) {
    ptsArguments_ = (Argument**)malloc(sizeof(Argument*) * nArguments);
    MEM_ALLOC_CHECK(ptsArguments_);
  }

  nmaxFunctionTags_ = nFunctionTags;
  if (nFunctionTags > 0) {
    ptsFunctionTags_ =
        (Tag**)malloc(sizeof(Tag*) * nFunctionTags);
    MEM_ALLOC_CHECK(ptsFunctionTags_);
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

  for (int i = 0; i < nmaxFunctionTags_; i++) {
    if (ptsFunctionTags_[i] != NULL) {
      free(ptsFunctionTags_[i]);
      ptsFunctionTags_[i] = NULL;
    }
  }
  free(ptsFunctionTags_);
}

void Function::AddArgument(Argument& argument) {
  // set order of argument to distinguish when receiving function request from
  // middleware
  if (ncurArguments_ >= nmaxArguments_) {
    SOPLOGLN(F("the number of arguments is exceeded"));
    return;
  }
  ptsArguments_[ncurArguments_] = &argument;
  argument.set_order(ncurArguments_++);
}

void Function::AddTag(const char *tag_name) {
  Tag *p_tag = new Tag(tag_name);
  ptsFunctionTags_[ncurFunctionTags_] = p_tag;
  ncurFunctionTags_++;
}

void Function::AddTag(Tag& function_tag) {
  ptsFunctionTags_[ncurFunctionTags_] = &function_tag;
  ncurFunctionTags_++;
}

char* Function::name() { return (char*)name_; }

void Function::set_name(const char* name) {
  name_ = strdup(name);
  MEM_ALLOC_CHECK(name_);
}

void Function::set_function(VoidFunction func) {
  function_ = (void*)func;
  function_classifier_ = VOID;
}

void Function::Execute(char* args, int* success) const {
  int i;
  //	char *pTokPtr = NULL;
  char* target = NULL;
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

SoPType Function::function_classifier() { return function_classifier_; }

void Function::GetInformation(char* buffer) {
  int i, len;
  switch (function_classifier_) {
    case INTEGER:
    case DOUBLE:
    case BOOL:
    case VOID:
      snprintf(buffer, MAX_BUFFER_SIZE, "%s\t%d\t%d", name_, ncurArguments_, ncurFunctionTags_);
      break;
    default:
      SOPLOGLN(F("[ERROR] Function::GetInformation -> ERROR!"));
      break;
  }
  // SOPLOGLN(F("Get Function Information!"));
}