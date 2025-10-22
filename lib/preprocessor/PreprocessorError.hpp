#ifndef PREPROCESSORERROR_HPP_
#define PREPROCESSORERROR_HPP_

#include <stdexcept>

class PreprocessorError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

#endif // PREPROCESSORERROR_HPP_
