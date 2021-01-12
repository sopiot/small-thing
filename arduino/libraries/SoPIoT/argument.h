#ifndef SMALL_THING_ARGUMENT_H_
#define SMALL_THING_ARGUMENT_H_

#include "common.h"

class Argument {
 public:
  /**
   * constructor for int or char array or bool argument
   * @param name means the name of the Argument
   */
  Argument(const char *name, int min, int max, CapType arg_type);
  /**
   * constructor for double-argument function
   * @param name means the name of the function
   */
  Argument(const char *name, double min, double max, CapType arg_type);

  /** @} */
  /**
   * A destructor
   */
  ~Argument();

  /**
   * set name of the function
   */
  void set_name(const char *name);
  /**
   * get name's pointer of the function
   */
  char *name();
  void *value();

  /**
   * @}
   * @name set minmax
   * set the minimum value or maximum of the value
   * @{
   */

  /**
   * get the order of the argument
   */
  int get_order();
  /**
   * set the order of the argument
   */
  void set_order(const int order);

  /**
   * get type status of the function, it also not be used in user level
   */
  CapType arg_type(void);

  void GetInformation(char *buf);

  bool SetArgumentIfValid(char *val);

 private:
  /**
   * set the minimum value of the argument of integer function or double
   * function
   */
  void set_min(const int min);
  /**
   * set the minimum value of the argument of the double function
   */
  void set_min(const double min);
  /**
   * set the maximum value of the argument of integer function or double
   * function
   */
  void set_max(const int max);
  /**
   * set the maximum value of the argument of double function
   */
  void set_max(const double max);

  /** @}
   *
   * initailizing function
   */
  void Initialize();
  char *name_; /** < A name of the argument */
  void *value_;
  void *min_;
  void *max_;
  CapType arg_type_; /** < represents type of the argument */
  int order_;        /** < represents order of argument */
};

#endif  // SMALL_THING_ARGUMENT_H_