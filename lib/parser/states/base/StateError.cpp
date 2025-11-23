#include "StateError.hpp"

namespace ovum::compiler::parser {

StateError::StateError() = default;

StateError::StateError(std::string message) : message_(std::move(message)) {
}

StateError::StateError(std::string_view message) : message_(message) {
}

const std::string& StateError::Message() const noexcept {
  return message_;
}

} // namespace ovum::compiler::parser
