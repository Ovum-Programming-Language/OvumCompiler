#ifndef IBINARYOPTAG_HPP_
#define IBINARYOPTAG_HPP_

class IBinaryOpTag {
public:
  virtual ~IBinaryOpTag() = default;

  virtual std::string_view Name() const = 0;
};

#endif // IBINARYOPTAG_HPP_
