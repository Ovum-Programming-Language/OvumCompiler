#ifndef PARSER_INFIXSPEC_HPP_
#define PARSER_INFIXSPEC_HPP_

#include <functional>

#include "lib/parser/ast/nodes/exprs/tags/IBinaryOpTag.hpp"
#include "tokens/Token.hpp"

namespace ovum::compiler::parser {

class InfixSpec {
public:
  InfixSpec(int left_bp, int right_bp, bool right_associative, const IBinaryOpTag* tag, bool is_elvis);

  void SetMatcher(std::function<bool(const Token&)> matcher);
  [[nodiscard]] bool TryMatch(const Token& token) const;

  [[nodiscard]] int Lbp() const noexcept;
  [[nodiscard]] int Rbp() const noexcept;
  [[nodiscard]] bool IsRightAssociative() const noexcept;
  [[nodiscard]] const IBinaryOpTag* Tag() const noexcept;
  [[nodiscard]] bool IsElvis() const noexcept;

  void SetLbp(int v) noexcept;
  void SetRbp(int v) noexcept;
  void SetRightAssociative(bool v) noexcept;
  void SetTag(const IBinaryOpTag* t) noexcept;
  void SetElvis(bool v) noexcept;

private:
  std::function<bool(const Token&)> match_;
  int lbp_ = 0;
  int rbp_ = 0;
  bool right_associative_ = false;
  const IBinaryOpTag* tag_ = nullptr;
  bool is_elvis_ = false;
};

} // namespace ovum::compiler::parser

#endif // PARSER_INFIXSPEC_HPP_
