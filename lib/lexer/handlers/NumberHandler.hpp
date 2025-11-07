#ifndef LEXER_NUMBERHANDLER_HPP_
#define LEXER_NUMBERHANDLER_HPP_

#include "Handler.hpp"

namespace ovum::compiler::lexer {

class NumberHandler : public Handler {
public:
  OptToken Scan(SourceCodeWrapper& wrapper) override;
};

} // namespace ovum::compiler::lexer

#endif // LEXER_NUMBERHANDLER_HPP_
