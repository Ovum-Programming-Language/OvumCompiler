#ifndef LEXER_COLONHANDLER_HPP_
#define LEXER_COLONHANDLER_HPP_
#include "Handler.hpp"

namespace ovum::compiler::lexer {

class ColonHandler : public Handler {
public:
  std::expected<OptToken, LexerError> Scan(SourceCodeWrapper& wrapper) override;
};

} // namespace ovum::compiler::lexer

#endif // LEXER_COLONHANDLER_HPP_
