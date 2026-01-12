#include "InfixSpec.hpp"

namespace ovum::compiler::parser {

InfixSpec::InfixSpec(int left_bp, int right_bp, bool right_associative, const IBinaryOpTag* tag, bool is_elvis) :
    lbp_(left_bp), rbp_(right_bp), right_associative_(right_associative), tag_(tag), is_elvis_(is_elvis) {
}

void InfixSpec::SetMatcher(std::function<bool(const Token&)> matcher) {
  match_ = std::move(matcher);
}

bool InfixSpec::TryMatch(const Token& token) const {
  return match_ ? match_(token) : false;
}

int InfixSpec::Lbp() const noexcept {
  return lbp_;
}
int InfixSpec::Rbp() const noexcept {
  return rbp_;
}
bool InfixSpec::IsRightAssociative() const noexcept {
  return right_associative_;
}
const IBinaryOpTag* InfixSpec::Tag() const noexcept {
  return tag_;
}
bool InfixSpec::IsElvis() const noexcept {
  return is_elvis_;
}

void InfixSpec::SetLbp(int v) noexcept {
  lbp_ = v;
}
void InfixSpec::SetRbp(int v) noexcept {
  rbp_ = v;
}
void InfixSpec::SetRightAssociative(bool v) noexcept {
  right_associative_ = v;
}
void InfixSpec::SetTag(const IBinaryOpTag* t) noexcept {
  tag_ = t;
}
void InfixSpec::SetElvis(bool v) noexcept {
  is_elvis_ = v;
}

} // namespace ovum::compiler::parser
