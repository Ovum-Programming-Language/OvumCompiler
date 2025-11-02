#ifndef IASSIGNOPTAG_HPP_
#define IASSIGNOPTAG_HPP_

class IAssignOpTag {
public:
  virtual ~IAssignOpTag() = default;

  virtual std::string_view Name() const = 0;
};

#endif // IASSIGNOPTAG_HPP_
