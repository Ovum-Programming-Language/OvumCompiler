#ifndef TOKENPOSITION_HPP_
#define TOKENPOSITION_HPP_

#include <cstdint>

class TokenPosition {
public:
  TokenPosition(int32_t line, int32_t column) : line_(line), column_(column) {
  }

  [[nodiscard]] int32_t GetLine() const noexcept {
    return line_;
  }

  [[nodiscard]] int32_t GetColumn() const noexcept {
    return column_;
  }

private:
  int32_t line_;
  int32_t column_;
};

#endif // TOKENPOSITION_HPP_
