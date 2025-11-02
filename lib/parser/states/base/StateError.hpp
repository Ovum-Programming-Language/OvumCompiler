#ifndef STATEERROR_HPP_
#define STATEERROR_HPP_

#include <string>

class StateError {
public:
  StateError();
  explicit StateError(std::string message);
  const std::string& Message() const noexcept;

private:
  std::string message_;
};

#endif // STATEERROR_HPP_
