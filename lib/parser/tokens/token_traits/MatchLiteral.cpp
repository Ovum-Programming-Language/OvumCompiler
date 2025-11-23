#include "MatchLiteral.hpp"

#include <string>

namespace ovum::compiler::parser {

bool MatchLiteral::TryMatch(const Token& token) const {
  const std::string type = token.GetStringType();
  // LiteralToken::GetStringType() -> "LITERAL:<TypeName>"
  return type.rfind("LITERAL:", 0) == 0;
}

} // namespace ovum::compiler::parser
