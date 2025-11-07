#ifndef TOKENS_TOKENPOSITION_HPP_
#define TOKENS_TOKENPOSITION_HPP_

#include <cstdint>

namespace ovum {

class TokenPosition {
public:
  TokenPosition(int32_t line, int32_t column);

  [[nodiscard]] int32_t GetLine() const noexcept;

  [[nodiscard]] int32_t GetColumn() const noexcept;

private:
  int32_t line_;
  int32_t column_;
};

} // namespace ovum

#endif // TOKENS_TOKENPOSITION_HPP_
