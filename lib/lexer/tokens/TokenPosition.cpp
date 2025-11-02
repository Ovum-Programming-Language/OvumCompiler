#include "TokenPosition.hpp"

TokenPosition::TokenPosition() : line_(0), column_(0) {
}

TokenPosition::TokenPosition(int32_t line, int32_t column) : line_(line), column_(column) {
}

int32_t TokenPosition::GetLine() const noexcept {
  return line_;
}

int32_t TokenPosition::GetColumn() const noexcept {
  return column_;
}

bool TokenPosition::IsValid() const noexcept {
  return (line_ > 0) && (column_ > 0);
}

TokenPosition TokenPosition::Invalid() noexcept {
  return {-1, -1};
}

bool TokenPosition::operator<(const TokenPosition& rhs) const noexcept {
  return (line_ < rhs.line_) || (line_ == rhs.line_ && column_ < rhs.column_);
}

bool TokenPosition::operator==(const TokenPosition& rhs) const noexcept {
  return line_ == rhs.line_ && column_ == rhs.column_;
}
