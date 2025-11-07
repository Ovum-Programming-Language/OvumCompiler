#ifndef LEXER_HANDLER_HPP_
#define LEXER_HANDLER_HPP_

#include <optional>

#include "lib/lexer/SourceCodeWrapper.hpp"
#include "lib/tokens/Token.hpp"

namespace ovum::compiler::lexer {

using TokenPtr = ovum::TokenPtr;
using OptToken = std::optional<TokenPtr>;

class Handler { // NOLINT(cppcoreguidelines-special-member-functions)
public:
  virtual ~Handler() = default;

  virtual OptToken Scan(SourceCodeWrapper& wrapper) = 0;
};

} // namespace ovum::compiler::lexer

#endif // LEXER_HANDLER_HPP_
