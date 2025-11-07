#include "NewlineToken.hpp"

#include <sstream>

namespace ovum {

NewlineToken::NewlineToken(const TokenPosition& position) : lexeme_("\\n"), position_(position) {
}

std::string NewlineToken::GetStringType() const noexcept {
  return "NEWLINE";
}

std::string NewlineToken::GetLexeme() const noexcept {
  return lexeme_;
}

TokenPtr NewlineToken::Clone() const {
  return std::make_shared<NewlineToken>(*this);
}

void NewlineToken::Accept(TokenVisitor& visitor) const {
  visitor.Visit(*this);
}

std::string NewlineToken::ToString() const {
  std::ostringstream os;
  os << "Token(NEWLINE, '\\n', @" << position_.GetLine() << ":" << position_.GetColumn() << ")";
  return os.str();
}

const TokenPosition& NewlineToken::GetPosition() const noexcept {
  return position_;
}

} // namespace ovum
