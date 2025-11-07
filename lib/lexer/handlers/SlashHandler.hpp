#ifndef LEXER_SLASHHANDLER_HPP_
#define LEXER_SLASHHANDLER_HPP_

#include "Handler.hpp"

namespace ovum::compiler::lexer {

class SlashHandler : public Handler {
public:
  std::expected<OptToken, LexerError> Scan(SourceCodeWrapper& wrapper) override;
};

} // namespace ovum::compiler::lexer

#endif // LEXER_SLASHHANDLER_HPP_
