#ifndef IBINARYOPTAG_HPP_
#define IBINARYOPTAG_HPP_

class IBinaryOpTag {
public:
  virtual ~IBinaryOpTag() = default;

  virtual const char* Name() const = 0;
};

#endif // IBINARYOPTAG_HPP_
