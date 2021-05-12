#ifndef SMALL_THING_ATTRIBUTE_H_
#define SMALL_THING_ATTRIBUTE_H_

#include "common.h"

class Attribute {
 public:
  /**
   * @name complete Constructors
   * @brief while using 3 Constructors below, it can be added to the Client
   * without any function calls
   * @param name means the name of the attribute
   * @{
   */

  /**
   * constructor with name and real value.
   */
  Attribute(const char* name, double attribute_value, CapType attribute_type);
  /**
   * constructor with name and string value.
   */
  Attribute(const char* name, char* attribute_value, CapType attribute_type);

  /** @} */
  /**
   * A destructor
   */
  ~Attribute();

  /**
   * set name of the attribute
   */
  void set_name(const char* name);
  /**
   * get name's pointer of the attribute
   */
  char* name();

  /**
   * set string or binary value of the attribute
   */
  void set_attribute_value(const char* attribute_value);

  /**
   * set real value of the attribute
   */
  void set_attribute_value(double attribute_value);

  /**
   * get json_object* to register to the middleware, it will not be used in user
   * level
   */
  void GetInformation(char* buffer);

  /**
   * get type status of the attribute, it also not be used in user level
   */
  CapType attribute_type(void);

 private:
  /**
   * initailizing function
   */
  void Initialize();
  double real_value_;
  char* string_value_;
  char* name_;             /** < A name of the value*/
  CapType attribute_type_; /** < represents type of the value */
};

#endif  // SMALL_THING_ATTRIBUTE_H_