#include "LiteralToken.hpp"

#include <memory>
#include <sstream>

namespace ovum {

LiteralToken::LiteralToken(std::string rawLexeme, std::unique_ptr<Value> value, const TokenPosition& position) :
    lexeme_(std::move(rawLexeme)), value_(std::move(value)), position_(position) {
}

std::string LiteralToken::GetStringType() const noexcept {
  return "LITERAL:" + value_->GetTypeName();
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

TokenPtr LiteralToken::Clone() const {
  std::unique_ptr<Value> vcopy = value_ ? value_->Clone() : nullptr;
  return std::make_shared<LiteralToken>(lexeme_, std::move(vcopy), position_);
}

void LiteralToken::Accept(TokenVisitor& visitor) const {
  visitor.Visit(*this);
}

std::string LiteralToken::ToString() const {
  std::ostringstream os;
  os << "Token(" << GetStringType() << ", '" << lexeme_ << "'";

  if (value_) {
    os << ", " << value_->ToString();
  }

  os << ", @" << position_.GetLine() << ":" << position_.GetColumn() << ")";
  return os.str();
}

const TokenPosition& LiteralToken::GetPosition() const noexcept {
  return position_;
}

} // namespace ovum
