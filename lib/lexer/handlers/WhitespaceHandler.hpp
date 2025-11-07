#ifndef LEXER_WHITESPACEHANDLER_HPP_
#define LEXER_WHITESPACEHANDLER_HPP_

#include "Handler.hpp"

namespace ovum::compiler::lexer {

class WhitespaceHandler : public Handler {
public:
  std::expected<OptToken, LexerError> Scan(SourceCodeWrapper& wrapper) override;
};

} // namespace ovum::compiler::lexer

#endif // LEXER_WHITESPACEHANDLER_HPP_
