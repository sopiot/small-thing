#ifndef SMALL_THING_TAG_H_
#define SMALL_THING_TAG_H_

#include "common.h"

class Tag {
 public:
  Tag();
  Tag(const char* name);
  ~Tag();

  void SetName(const char* name);
  char* GetName();

  /**
   * get json_object* to register to the middleware, it will not be used in user
   * level
   */
  void GetInformation(char* buffer);

 private:
  void Initialize();
  char* name_;
};

#endif  // SMALL_THING_TAG_H_