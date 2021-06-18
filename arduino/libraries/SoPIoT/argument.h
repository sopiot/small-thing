#ifndef SMALL_THING_ARGUMENT_H_
#define SMALL_THING_ARGUMENT_H_

#include "common.h"

class Argument {
 public:
  Argument(const char* name, int min, int max, SoPType arg_type);
  Argument(const char* name, double min, double max, SoPType arg_type);

  ~Argument();

  void set_name(const char* name);

  char* name();
  void* value();

  int get_order();
  void set_order(const int order);

  SoPType arg_type(void);

  void GetInformation(char* buf);

  bool SetArgumentIfValid(char* val);

 private:
  void set_min(const int min);
  void set_min(const double min);

  void set_max(const int max);
  void set_max(const double max);

  void Initialize();
  char* name_; /** < A name of the argument */
  void* value_;
  void* min_;
  void* max_;
  SoPType arg_type_; /** < represents type of the argument */
  int order_;        /** < represents order of argument */
};

#endif  // SMALL_THING_ARGUMENT_H_