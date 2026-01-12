#include "MatchManyOf.hpp"

namespace ovum::compiler::parser {

MatchAnyOf::MatchAnyOf(std::vector<std::unique_ptr<ITokenMatcher>> matchers) : matchers_(std::move(matchers)) {
}

bool MatchAnyOf::TryMatch(const Token& token) const {
  for (const auto& m : matchers_) {
    if (m && m->TryMatch(token)) {
      return true;
    }
  }
  return false;
}

} // namespace ovum::compiler::parser
