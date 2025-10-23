#ifndef FIXIT_HPP_
#define FIXIT_HPP_

#include <string>

#include "lib/parser/tokens/SourceSpan.hpp"

class FixIt {
public:
  FixIt(SourceSpan source_span, std::string replacement);
  // TODO: make methods
private:
  SourceSpan where_;
  std::string replacement_;
};

#endif // FIXIT_HPP_
