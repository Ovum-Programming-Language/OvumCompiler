#ifndef LEXER_OPERATORHANDLER_HPP_
#define LEXER_OPERATORHANDLER_HPP_

#include "Handler.hpp"

namespace ovum::compiler::lexer {

class OperatorHandler : public Handler {
public:
  std::expected<OptToken, LexerError> Scan(SourceCodeWrapper& wrapper) override;
};

} // namespace ovum::compiler::lexer

#endif // LEXER_OPERATORHANDLER_HPP_
