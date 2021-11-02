#ifndef SMALL_THING_ARGUMENT_H_
#define SMALL_THING_ARGUMENT_H_

#include "common.h"

#pragma pack(push, 1)
class Argument {
 public:
  Argument();
  Argument(const char* name, int min, int max, SoPType argument_type);
  Argument(const char* name, double min, double max, SoPType argument_type);

  ~Argument();

  void SetName(const char* name);

  char* GetName();
  void* GetValue();

  int GetOrder();
  void* GetMin();
  void* GetMax();
  void SetOrder(const int order);

  SoPType GetArgumentType(void);
  void GetRegisterPublishData(char* buf);

  bool SetArgumentIfValid(char* val);

 private:
  void SetMin(const int min);
  void SetMin(const double min);

  void SetMax(const int max);
  void SetMax(const double max);

  void Initialize();
  void* value_;
  void* min_;
  void* max_;
  SoPType argument_type_; /** < represents type of the argument */
  int order_;             /** < represents order of argument */
  char* name_;            /** < A name of the argument */
};
#pragma pack(pop)

#endif  // SMALL_THING_ARGUMENT_H_