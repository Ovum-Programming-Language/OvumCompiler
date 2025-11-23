#include "MatchType.hpp"

namespace ovum::compiler::parser {

MatchType::MatchType(std::string_view type_name) : type_(type_name) {
}

bool MatchType::TryMatch(const Token& token) const {
  return token.GetStringType() == type_;
}

} // namespace ovum::compiler::parser
