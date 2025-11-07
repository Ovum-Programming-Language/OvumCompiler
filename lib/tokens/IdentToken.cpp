#include "IdentToken.hpp"

#include <sstream>

namespace ovum {

IdentToken::IdentToken(std::string lex, const TokenPosition& position) : lexeme_(std::move(lex)), position_(position) {
}

std::string IdentToken::GetStringType() const noexcept {
  return "IDENT";
}

std::string IdentToken::GetLexeme() const noexcept {
  return lexeme_;
}

TokenPtr IdentToken::Clone() const {
  return std::make_shared<IdentToken>(*this);
}

void IdentToken::Accept(TokenVisitor& visitor) const {
  visitor.Visit(*this);
}

std::string IdentToken::ToString() const {
  std::ostringstream os;
  os << "Token(IDENT, '" << lexeme_ << "', @" << position_.GetLine() << ":" << position_.GetColumn() << ")";
  return os.str();
}

const TokenPosition& IdentToken::GetPosition() const noexcept {
  return position_;
}

const std::string& IdentToken::GetName() const noexcept {
  return lexeme_;
}

} // namespace ovum
