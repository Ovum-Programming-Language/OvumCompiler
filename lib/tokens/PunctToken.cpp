#include <sstream>
#include "PunctToken.hpp"

namespace ovum {

PunctToken::PunctToken(char ch, const TokenPosition& position) : lexeme_(1, ch), position_(position) {
}

PunctToken::PunctToken(std::string punct, const TokenPosition& position) :
    lexeme_(std::move(punct)), position_(position) {
}

std::string PunctToken::GetStringType() const noexcept {
  return "PUNCT";
}

std::string PunctToken::GetLexeme() const noexcept {
  return lexeme_;
}

TokenPtr PunctToken::Clone() const {
  return std::make_shared<PunctToken>(*this);
}

void PunctToken::Accept(TokenVisitor& visitor) const {
  visitor.Visit(*this);
}

std::string PunctToken::ToString() const {
  std::ostringstream os;
  os << "Token(PUNCT, '" << lexeme_ << "', @" << position_.GetLine() << ":" << position_.GetColumn() << ")";
  return os.str();
}

const TokenPosition& PunctToken::GetPosition() const noexcept {
  return position_;
}

} // namespace ovum
