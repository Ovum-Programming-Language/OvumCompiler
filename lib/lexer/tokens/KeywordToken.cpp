#include "KeywordToken.hpp"

#include <sstream>

KeywordToken::KeywordToken(std::string lex, int32_t line, int32_t col) : Token(line, col), lexeme_(std::move(lex)) {
}

std::string KeywordToken::GetStringType() const noexcept {
  return "KEYWORD";
}

std::string KeywordToken::GetLexeme() const noexcept {
  return lexeme_;
}

std::unique_ptr<Token> KeywordToken::Clone() const {
  return std::make_unique<KeywordToken>(*this);
}

void KeywordToken::Accept(TokenVisitor& visitor) const {
  visitor.Visit(*this);
}

std::string KeywordToken::ToString() const {
  std::ostringstream os;
  os << "Token(KEYWORD, '" << lexeme_ << "', @" << this->GetLine() << ":" << this->GetColumn() << ")";
  return os.str();
}
