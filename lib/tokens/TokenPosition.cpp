#include "TokenPosition.hpp"

namespace ovum {

TokenPosition::TokenPosition(int32_t line, int32_t column) : line_(line), column_(column) {
}

int32_t TokenPosition::GetLine() const noexcept {
  return line_;
}

int32_t TokenPosition::GetColumn() const noexcept {
  return column_;
}

} // namespace ovum
