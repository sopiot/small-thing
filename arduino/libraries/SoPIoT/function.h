#ifndef SMALL_THING_FUNCTION_H_
#define SMALL_THING_FUNCTION_H_

#include "argument.h"
#include "attribute.h"
#include "common.h"

/**
 * @brief A class for handling various "function" types.
 * @details A programmer can add Functions to the Supervisor by using this
 * class. void f(void | int | double | bool) - only 4 types can be handled
 */
class Function {
 public:
  /**
   * @name incomplete Constructors
   * @brief Name and function must be set.
   *        Also, if the argument type of the function is integer, double, or
   * const char*, min and max must be set. (In the case of const char*, min and
   * max indicates minimum length and maximum length of the array.)
   * @param (*function) means the function pointer
   * @param min means the minimum argument value of the function
   * @param max means the maximum argument value of the function
   * @{
   */

  /* only one type of constructor is allowed*/
  Function(const char *name, VoidFunction func, int nArguments,
           int nFunctionAttributes);

  /** @} */
  /**
   * A destructor
   */
  ~Function();

  /**
   * add a function-dependent value
   */

  void Add_argument(Argument &argument);

  void Add_functionattribute(Attribute &function_attribute);

  /**
   * set name of the function
   */
  void set_name(const char *name);
  /**
   * get name's pointer of the function
   */
  char *name();

  /**
   * @name set function
   * set function pointer
   * @{
   */
  int ncurArguments() { return ncurArguments_; }
  Argument *getIdxArgument(int idx) { return ptsArguments_[idx]; }

  // Dowhan added
  Attribute *getIdxFunctionAttribute(int idx) {
    return ptsFunctionAttributes_[idx];
  }
  // End
  int ncurFunctionAttributes() { return ncurFunctionAttributes_; }

  void GetInformation(char *buffer);
  void Execute(char *args, int *success) const;
  /**
   * set a registered id of topic name from MT1003 (returns id_1003)
   */
  uint16_t set_id_1003(uint16_t id_1003);
  /**
   * get a registerd id of topic name from MT1003
   */
  uint16_t id_1003();
  /**
   * set a registered id of topic name from MT2004 (returns id_2004)
   */
  uint16_t set_id_2004_deprecated(uint16_t id_2004);
  uint16_t set_id_2004(uint16_t id_2004);
  /**
   * get a saved id_2004
   */
  uint16_t id_2004_deprecated();
  uint16_t id_2004();

  CapType function_classifier(void);  // for function return type

 private:
  void set_function(VoidFunction func);

  void Initialize();

  char *name_;
  void *function_;
  int nmaxArguments_;
  int ncurArguments_;
  Argument **ptsArguments_;
  // Dowhan added
  Attribute **ptsFunctionAttributes_;
  int ncurFunctionAttributes_;
  // End

  uint16_t id_1003_;
  uint16_t id_2004_deprecated_;
  uint16_t id_2004_;

  CapType function_classifier_;
};

#endif  // SMALL_THING_FUNCTION_H_
