#ifndef LEXER_DOTCOMPOSITEHANDLER_HPP_
#define LEXER_DOTCOMPOSITEHANDLER_HPP_

#include "Handler.hpp"
#include "NumberHandler.hpp"
#include "OperatorHandler.hpp"
#include "PunctHandler.hpp"

namespace ovum::compiler::lexer {

class DotCompositeHandler : public Handler {
public:
  DotCompositeHandler();

  std::expected<OptToken, LexerError> Scan(SourceCodeWrapper& wrapper) override;

private:
  std::unique_ptr<NumberHandler> num_;
  std::unique_ptr<OperatorHandler> op_;
  std::unique_ptr<PunctHandler> punct_;
};

} // namespace ovum::compiler::lexer

#endif // LEXER_DOTCOMPOSITEHANDLER_HPP_
