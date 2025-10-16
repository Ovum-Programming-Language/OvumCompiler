#include "IdentToken.hpp"

#include <sstream>

IdentToken::IdentToken(std::string lex, int32_t line, int32_t col) : Token(line, col), lexeme_(std::move(lex)) {
}

TokenType IdentToken::GetType() const noexcept {
  return TokenType::kIdent;
}

std::string IdentToken::GetLexeme() const noexcept {
  return lexeme_;
}

std::unique_ptr<Token> IdentToken::Clone() const {
  return std::make_unique<IdentToken>(*this);
}

void IdentToken::Accept(TokenVisitor& visitor) const {
  visitor.Visit(*this);
}

std::string IdentToken::ToString() const {
  std::ostringstream os;
  os << "Token(IDENT, '" << lexeme_ << "', @" << this->GetLine() << ":" << this->GetColumn() << ")";
  return os.str();
}

const std::string& IdentToken::GetName() const noexcept {
  return lexeme_;
}
