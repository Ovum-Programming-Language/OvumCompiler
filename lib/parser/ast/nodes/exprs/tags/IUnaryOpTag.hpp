#ifndef IUNARYOPTAG_HPP_
#define IUNARYOPTAG_HPP_

class IUnaryOpTag {
public:
  virtual ~IUnaryOpTag() = default;

  virtual const char* Name() const = 0;
};

#endif // IUNARYOPTAG_HPP_
