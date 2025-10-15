#include "LiteralToken.hpp"

#include <sstream>

LiteralToken::LiteralToken(
    TokenType typ, std::string rawLexeme, std::unique_ptr<Value> value, int32_t line, int32_t col) :
    Token(line, col), typ_(typ), lexeme_(std::move(rawLexeme)), value_(std::move(value)) {
}

TokenType LiteralToken::GetType() const noexcept {
  return typ_;
}

std::string LiteralToken::GetLexeme() const noexcept {
  return lexeme_;
}

const Value* LiteralToken::GetValue() const noexcept {
  return value_.get();
}

Value* LiteralToken::GetValue() noexcept {
  return value_.get();
}

std::unique_ptr<Token> LiteralToken::Clone() const {
  std::unique_ptr<Value> vcopy = value_ ? value_->Clone() : nullptr;
  return std::make_unique<LiteralToken>(typ_, lexeme_, std::move(vcopy), this->GetLine(), this->GetColumn());
}

void LiteralToken::Accept(TokenVisitor& visitor) const {
  visitor.Visit(*this);
}

std::string LiteralToken::ToString() const {
  std::ostringstream os;
  os << "Token(" << to_string_view(typ_) << ", '" << lexeme_ << "'";
  if (value_) {
    os << ", " << value_->ToString();
  }

  os << ", @" << this->GetLine() << ":" << this->GetColumn() << ")";
  return os.str();
}
