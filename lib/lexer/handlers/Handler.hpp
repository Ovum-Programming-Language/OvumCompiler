#ifndef HANDLER_HPP_
#define HANDLER_HPP_

#include <optional>

#include "lib/lexer/SourceCodeWrapper.hpp"
#include "lib/lexer/tokens/Token.hpp"

using OptToken = std::optional<TokenPtr>;

class Handler { // NOLINT(cppcoreguidelines-special-member-functions)
public:
  virtual ~Handler() = default;

  virtual OptToken Scan(SourceCodeWrapper& wrapper) = 0;
};

#endif // HANDLER_HPP_
