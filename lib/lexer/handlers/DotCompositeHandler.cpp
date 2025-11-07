#include "DotCompositeHandler.hpp"

namespace ovum::compiler::lexer {

DotCompositeHandler::DotCompositeHandler() :
    num_(std::make_unique<NumberHandler>()), op_(std::make_unique<OperatorHandler>()),
    punct_(std::make_unique<PunctHandler>()) {
}

std::expected<OptToken, LexerError> DotCompositeHandler::Scan(SourceCodeWrapper& w) {
  if (std::isdigit(static_cast<unsigned char>(w.Peek()))) {
    return num_->Scan(w);
  }

  auto op_result = op_->Scan(w);
  if (!op_result) {
    return op_result;
  }
  if (op_result.value()) {
    return op_result;
  }

  return punct_->Scan(w);
}

} // namespace ovum::compiler::lexer
