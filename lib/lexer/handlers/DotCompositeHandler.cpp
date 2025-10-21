#include "DotCompositeHandler.hpp"

DotCompositeHandler::DotCompositeHandler() :
    num_(std::make_unique<NumberHandler>()), op_(std::make_unique<OperatorHandler>()),
    punct_(std::make_unique<PunctHandler>()) {
}

OptToken DotCompositeHandler::Scan(SourceCodeWrapper& w) {
  if (std::isdigit(static_cast<unsigned char>(w.Peek()))) {
    return num_->Scan(w);
  }

  if (auto t = op_->Scan(w)) {
    return t;
  }

  if (punct_) {
    return punct_->Scan(w);
  }

  return std::nullopt;
}
