#include "tag.h"
#include "utils.h"

void Tag::Initialize() {
  name_ = NULL;
}

Tag::Tag(const char* name) {
  Initialize();
  set_name(name);
}

Tag::~Tag() {
  if (name_) free(name_);
}

char* Tag::name() { return name_; }

void Tag::set_name(const char* name) {
  name_ = strdup(name);
  MEM_ALLOC_CHECK(name_);
}

void Tag::GetInformation(char* buffer) {
  snprintf(buffer, MAX_BUFFER_SIZE, "%s", name_);
}