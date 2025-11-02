#ifndef TOKENPOSITION_HPP_
#define TOKENPOSITION_HPP_

#include <cstdint>

class TokenPosition {
public:
  TokenPosition();
  TokenPosition(int32_t line, int32_t column);

  [[nodiscard]] int32_t GetLine() const noexcept;
  [[nodiscard]] int32_t GetColumn() const noexcept;

  [[nodiscard]] bool IsValid() const noexcept;
  [[nodiscard]] static TokenPosition Invalid() noexcept;

  [[nodiscard]] bool operator<(const TokenPosition& rhs) const noexcept;
  [[nodiscard]] bool operator==(const TokenPosition& rhs) const noexcept;

private:
  int32_t line_;
  int32_t column_;
};

#endif // TOKENPOSITION_HPP_
