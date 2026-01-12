#ifndef PARSER_POSTFIXSPEC_HPP_
#define PARSER_POSTFIXSPEC_HPP_

#include <functional>

#include "tokens/Token.hpp"

namespace ovum::compiler::parser {

class PostfixSpec {
public:
  PostfixSpec(std::function<bool(const Token&)> match, int bp, bool keyword);

  [[nodiscard]] bool TryMatch(const Token& token) const;

  [[nodiscard]] int BindingPower() const noexcept;
  [[nodiscard]] bool IsKeyword() const noexcept;

  void SetBindingPower(int v) noexcept;
  void SetKeyword(bool v) noexcept;

private:
  std::function<bool(const Token&)> match_;
  int bp_ = 0;
  bool keyword_ = false;
};

} // namespace ovum::compiler::parser

#endif // PARSER_POSTFIXSPEC_HPP_
