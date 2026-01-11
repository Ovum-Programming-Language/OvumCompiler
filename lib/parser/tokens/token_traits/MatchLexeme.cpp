#include "MatchLexeme.hpp"

namespace ovum::compiler::parser {

MatchLexeme::MatchLexeme(const std::string_view lexeme) : lexeme_(lexeme) {
}

bool MatchLexeme::TryMatch(const Token& token) const {
  return token.GetLexeme() == lexeme_;
}

} // namespace ovum::compiler::parser
