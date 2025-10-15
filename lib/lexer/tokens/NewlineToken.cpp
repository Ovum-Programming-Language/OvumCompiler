#include "NewlineToken.hpp"

#include <sstream>

NewlineToken::NewlineToken(int32_t line, int32_t col) : Token(line, col), lexeme_("\\n") {
}

TokenType NewlineToken::GetType() const noexcept {
  return TokenType::NEWLINE;
}

std::string NewlineToken::GetLexeme() const noexcept {
  return lexeme_;
}

std::unique_ptr<Token> NewlineToken::Clone() const {
  return std::make_unique<NewlineToken>(*this);
}

void NewlineToken::Accept(TokenVisitor& visitor) const {
  visitor.Visit(*this);
}

std::string NewlineToken::ToString() const {
  std::ostringstream os;
  os << "Token(NEWLINE, '\\n', @" << this->GetLine() << ":" << this->GetColumn() << ")";
  return os.str();
}
