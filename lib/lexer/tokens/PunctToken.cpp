#include <sstream>
#include "PunctToken.hpp"

PunctToken::PunctToken(char ch, int32_t line, int32_t col) :
  Token(line, col),
  lexeme_(1, ch) {
}

PunctToken::PunctToken(std::string punct,
                       int32_t line,
                       int32_t col) :
  Token(line, col),
  lexeme_(std::move(punct)) {
}

TokenType PunctToken::GetType() const noexcept {
  return TokenType::kPunct;
}

std::string PunctToken::GetLexeme() const noexcept {
  return lexeme_;
}

std::unique_ptr<Token> PunctToken::Clone() const {
  return std::make_unique<PunctToken>(*this);
}

void PunctToken::Accept(TokenVisitor& visitor) const {
  visitor.Visit(*this);
}

std::string PunctToken::ToString() const {
  std::ostringstream os;
  os << "Token(PUNCT, '" << lexeme_ << "', @" << this->GetLine() << ":" << this
      ->GetColumn() << ")";
  return os.str();
}
