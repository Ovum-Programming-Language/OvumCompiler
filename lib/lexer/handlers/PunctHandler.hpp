#ifndef LEXER_PUNCTHANDLER_HPP_
#define LEXER_PUNCTHANDLER_HPP_

#include "Handler.hpp"

namespace ovum::compiler::lexer {

class PunctHandler : public Handler {
public:
  std::expected<OptToken, LexerError> Scan(SourceCodeWrapper& wrapper) override;
};

} // namespace ovum::compiler::lexer

#endif // LEXER_PUNCTHANDLER_HPP_
