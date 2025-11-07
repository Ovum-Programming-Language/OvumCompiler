#include "OperatorToken.hpp"

#include <sstream>

namespace ovum {

OperatorToken::OperatorToken(std::string op, const TokenPosition& position) :
    lexeme_(std::move(op)), position_(position) {
}

std::string OperatorToken::GetStringType() const noexcept {
  return "OPERATOR";
}

std::string OperatorToken::GetLexeme() const noexcept {
  return lexeme_;
}

TokenPtr OperatorToken::Clone() const {
  return std::make_shared<OperatorToken>(*this);
}

void OperatorToken::Accept(TokenVisitor& visitor) const {
  visitor.Visit(*this);
}

std::string OperatorToken::ToString() const {
  std::ostringstream os;
  os << "Token(OPERATOR, '" << lexeme_ << "', @" << position_.GetLine() << ":" << position_.GetColumn() << ")";
  return os.str();
}

const TokenPosition& OperatorToken::GetPosition() const noexcept {
  return position_;
}

} // namespace ovum
