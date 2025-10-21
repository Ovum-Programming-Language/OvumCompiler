#ifndef TOKENPOSITION_HPP_
#define TOKENPOSITION_HPP_

#include <cstdint>

class TokenPosition {
public:
  TokenPosition(int32_t line, int32_t column);

  [[nodiscard]] int32_t GetLine() const noexcept;

  [[nodiscard]] int32_t GetColumn() const noexcept;

private:
  int32_t line_;
  int32_t column_;
};

#endif // TOKENPOSITION_HPP_
