#include "CommentToken.hpp"

#include <sstream>

CommentToken::CommentToken(std::string txt, const TokenPosition& position) :
    text_(std::move(txt)), position_(position) {
}

std::string CommentToken::GetStringType() const noexcept {
  return "COMMENT";
}

std::string CommentToken::GetLexeme() const noexcept {
  return text_;
}

std::unique_ptr<Token> CommentToken::Clone() const {
  return std::make_unique<CommentToken>(*this);
}

void CommentToken::Accept(TokenVisitor& visitor) const {
  visitor.Visit(*this);
}

std::string CommentToken::ToString() const {
  std::ostringstream os;
  os << "Token(COMMENT, '" << text_ << "', @" << position_.GetLine() << ":" << position_.GetColumn() << ")";
  return os.str();
}

const TokenPosition& CommentToken::GetPosition() const noexcept {
  return position_;
}
