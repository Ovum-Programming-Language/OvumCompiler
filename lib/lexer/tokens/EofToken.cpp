#include "EofToken.hpp"

#include <sstream>

EofToken::EofToken(const TokenPosition& position) : position_(position) {
} 

std::string EofToken::GetStringType() const noexcept {
  return "EOF";
}

std::string EofToken::GetLexeme() const noexcept {
  return "";
}

std::unique_ptr<Token> EofToken::Clone() const {
  return std::make_unique<EofToken>(*this);
}

void EofToken::Accept(TokenVisitor& visitor) const {
  visitor.Visit(*this);
}

std::string EofToken::ToString() const {
  std::ostringstream os;
  os << "Token(EOF, @" << position_.GetLine() << ":" << position_.GetColumn() << ")";
  return os.str();
}

const TokenPosition& EofToken::GetPosition() const noexcept {
  return position_;
}