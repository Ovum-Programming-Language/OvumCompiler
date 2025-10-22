#ifndef ISEVERITY_HPP_
#define ISEVERITY_HPP_

#include <string_view>

class ISeverity {
public:
  virtual ~ISeverity() = default;
  virtual std::string_view Name() const = 0;
  virtual int Level() const = 0;
};

#endif // ISEVERITY_HPP_
