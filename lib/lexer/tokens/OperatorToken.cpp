#include "OperatorToken.hpp"

#include <sstream>

OperatorToken::OperatorToken(std::string op, int32_t line, int32_t col) : Token(line, col), lexeme_(std::move(op)) {
}

std::string OperatorToken::GetStringType() const noexcept {
  return "OPERATOR";
}

std::string OperatorToken::GetLexeme() const noexcept {
  return lexeme_;
}

std::unique_ptr<Token> OperatorToken::Clone() const {
  return std::make_unique<OperatorToken>(*this);
}

void OperatorToken::Accept(TokenVisitor& visitor) const {
  visitor.Visit(*this);
}

std::string OperatorToken::ToString() const {
  std::ostringstream os;
  os << "Token(OPERATOR, '" << lexeme_ << "', @" << this->GetLine() << ":" << this->GetColumn() << ")";
  return os.str();
}
