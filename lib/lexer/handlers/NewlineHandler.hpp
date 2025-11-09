#ifndef LEXER_NEWLINEHANDLER_HPP_
#define LEXER_NEWLINEHANDLER_HPP_

#include "Handler.hpp"

namespace ovum::compiler::lexer {

class NewlineHandler : public Handler {
public:
  std::expected<OptToken, LexerError> Scan(SourceCodeWrapper& wrapper) override;
};

} // namespace ovum::compiler::lexer

#endif // LEXER_NEWLINEHANDLER_HPP_
