#include "MatchIdentifier.hpp"

namespace ovum::compiler::parser {

bool MatchIdentifier::TryMatch(const Token& token) const {
  const std::string type = token.GetStringType();
  return type == "IDENT";
}

} // namespace ovum::compiler::parser
