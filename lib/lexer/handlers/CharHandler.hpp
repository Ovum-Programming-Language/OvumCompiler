#ifndef LEXER_CHARHANDLER_HPP_
#define LEXER_CHARHANDLER_HPP_

#include "Handler.hpp"

namespace ovum::compiler::lexer {

class CharHandler : public Handler {
public:
  OptToken Scan(SourceCodeWrapper& wrapper) override;
};

} // namespace ovum::compiler::lexer

#endif // LEXER_CHARHANDLER_HPP_
