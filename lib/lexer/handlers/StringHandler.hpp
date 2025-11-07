#ifndef LEXER_STRINGHANDLER_HPP_
#define LEXER_STRINGHANDLER_HPP_

#include "Handler.hpp"

namespace ovum::compiler::lexer {

class StringHandler : public Handler {
public:
  OptToken Scan(SourceCodeWrapper& wrapper) override;
};

} // namespace ovum::compiler::lexer

#endif // LEXER_STRINGHANDLER_HPP_
