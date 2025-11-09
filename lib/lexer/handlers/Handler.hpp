#ifndef LEXER_HANDLER_HPP_
#define LEXER_HANDLER_HPP_

#include <expected>
#include <optional>

#include <tokens/Token.hpp>

#include "lib/lexer/LexerError.hpp"
#include "lib/lexer/SourceCodeWrapper.hpp"

namespace ovum::compiler::lexer {

using TokenPtr = ovum::TokenPtr;
using OptToken = std::optional<TokenPtr>;

class Handler { // NOLINT(cppcoreguidelines-special-member-functions)
public:
  virtual ~Handler() = default;

  virtual std::expected<OptToken, LexerError> Scan(SourceCodeWrapper& wrapper) = 0;
};

} // namespace ovum::compiler::lexer

#endif // LEXER_HANDLER_HPP_
