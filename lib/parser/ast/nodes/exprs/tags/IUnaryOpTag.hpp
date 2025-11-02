#ifndef IUNARYOPTAG_HPP_
#define IUNARYOPTAG_HPP_

#include <string>

class IUnaryOpTag {
public:
  virtual ~IUnaryOpTag() = default;

  virtual std::string_view Name() const = 0;
};

#endif // IUNARYOPTAG_HPP_
