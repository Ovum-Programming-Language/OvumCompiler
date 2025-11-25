#include "PostfixSpec.hpp"

namespace ovum::compiler::parser {

PostfixSpec::PostfixSpec(std::function<bool(const Token&)> match, int bp, bool keyword) :
    match_(std::move(match)), bp_(bp), keyword_(keyword) {
}

bool PostfixSpec::TryMatch(const Token& token) const {
  return match_ ? match_(token) : false;
}

int PostfixSpec::BindingPower() const noexcept {
  return bp_;
}
bool PostfixSpec::IsKeyword() const noexcept {
  return keyword_;
}

void PostfixSpec::SetBindingPower(int v) noexcept {
  bp_ = v;
}
void PostfixSpec::SetKeyword(bool v) noexcept {
  keyword_ = v;
}

} // namespace ovum::compiler::parser
