#include "SourceCodeWrapper.hpp"

#include "utils.hpp"

SourceCodeWrapper::SourceCodeWrapper(std::string_view src, bool keep_comments) :
    src_(src), keep_comments_(keep_comments) {
}

bool SourceCodeWrapper::IsAtEnd() const noexcept {
  return current_ >= src_.size();
}

char SourceCodeWrapper::Peek(size_t offset) const noexcept {
  size_t idx = current_ + offset;

  if (idx >= src_.size()) {
    return '\0';
  }

  return src_[idx];
}

char SourceCodeWrapper::CurrentChar() const noexcept {
  if (current_ == 0) {
    return '\0';
  }

  return src_[current_ - 1];
}

char SourceCodeWrapper::Advance() {
  if (IsAtEnd()) {
    return '\0';
  }

  char c = src_[current_++];

  if (c == '\n') {
    ++line_;
    col_ = 1;
  } else {
    ++col_;
  }

  return c;
}

void SourceCodeWrapper::RetreatOne() {
  if (current_ == 0) {
    return;
  }

  --current_;
  int l = 1;

  for (size_t i = 0; i < current_; ++i) {
    if (src_[i] == '\n') {
      ++l;
    }
  }

  line_ = l;
  int col = 1;

  for (size_t i = current_; i > 0; --i) {
    if (src_[i - 1] == '\n') {
      col = static_cast<int>(current_ - i + 1);
      break;
    }

    if (i == 1) {
      col = static_cast<int>(current_);
    }
  }

  col_ = col;
}

void SourceCodeWrapper::ConsumeWhile(std::string& out, const std::function<bool(char)>& pred) {
  while (!IsAtEnd() && pred(Peek())) {
    out.push_back(Advance());
  }
}

void SourceCodeWrapper::ResetTokenPosition() {
  start_ = current_;
  token_col_ = col_;
}

std::string SourceCodeWrapper::GetRawLexeme() const {
  if (current_ >= start_) {
    return std::string(src_.substr(start_, current_ - start_));
  }

  return {};
}

int32_t SourceCodeWrapper::GetLine() const noexcept {
  return line_;
}

int32_t SourceCodeWrapper::GetCol() const noexcept {
  return col_;
}

int32_t SourceCodeWrapper::GetTokenCol() const noexcept {
  return token_col_;
}

bool SourceCodeWrapper::IsKeepComments() const noexcept {
  return keep_comments_;
}

bool SourceCodeWrapper::IsKeyword(std::string_view s) const {
  return KeywordSet().contains(std::string(s));
}

bool SourceCodeWrapper::IsMultiop(std::string_view s) const {
  return MultiOpsSet().contains(std::string(s));
}
