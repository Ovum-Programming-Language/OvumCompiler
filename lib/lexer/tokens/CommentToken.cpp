#include "CommentToken.hpp"

#include <sstream>

CommentToken::CommentToken(std::string txt, int32_t line, int32_t col) : Token(line, col), text_(std::move(txt)) {
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
  os << "Token(COMMENT, '" << text_ << "', @" << this->GetLine() << ":" << this->GetColumn() << ")";
  return os.str();
}
