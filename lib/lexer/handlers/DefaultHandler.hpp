#ifndef LEXER_DEFAULTHANDLER_HPP_
#define LEXER_DEFAULTHANDLER_HPP_

#include "Handler.hpp"

namespace ovum::compiler::lexer {

class DefaultHandler : public Handler {
public:
  std::expected<OptToken, LexerError> Scan(SourceCodeWrapper& wrapper) override;
};

} // namespace ovum::compiler::lexer

#endif // LEXER_DEFAULTHANDLER_HPP_
