#include "KeywordToken.hpp"

#include <sstream>

KeywordToken::KeywordToken(std::string lex, const TokenPosition& position) : lexeme_(std::move(lex)), position_(position) {
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
  os << "Token(KEYWORD, '" << lexeme_ << "', @" << position_.GetLine() << ":" << position_.GetColumn() << ")";
  return os.str();
}

const TokenPosition& KeywordToken::GetPosition() const noexcept {
  return position_;
}