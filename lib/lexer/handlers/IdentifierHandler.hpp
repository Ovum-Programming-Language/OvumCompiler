#ifndef LEXER_IDENTIFIERHANDLER_HPP_
#define LEXER_IDENTIFIERHANDLER_HPP_

#include "Handler.hpp"

namespace ovum::compiler::lexer {

class IdentifierHandler : public Handler {
public:
  std::expected<OptToken, LexerError> Scan(SourceCodeWrapper& wrapper) override;
};

} // namespace ovum::compiler::lexer

#endif // LEXER_IDENTIFIERHANDLER_HPP_
