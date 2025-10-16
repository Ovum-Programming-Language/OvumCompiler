#include "EofToken.hpp"

#include <sstream>

EofToken::EofToken(int32_t line, int32_t col) : Token(line, col), lexeme_("") {
}

TokenType EofToken::GetType() const noexcept {
  return TokenType::kEofT;
}

std::string EofToken::GetLexeme() const noexcept {
  return lexeme_;
}

std::unique_ptr<Token> EofToken::Clone() const {
  return std::make_unique<EofToken>(*this);
}

void EofToken::Accept(TokenVisitor& visitor) const {
  visitor.Visit(*this);
}

std::string EofToken::ToString() const {
  std::ostringstream os;
  os << "Token(EOF, @" << this->GetLine() << ":" << this->GetColumn() << ")";
  return os.str();
}
