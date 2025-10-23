#ifndef IASSIGNOPTAG_HPP_
#define IASSIGNOPTAG_HPP_

class IAssignOpTag {
public:
  virtual ~IAssignOpTag() = default;

  virtual const char* Name() const = 0;
};

#endif // IASSIGNOPTAG_HPP_
