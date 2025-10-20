#include "SourceCodeWrapper.hpp"

#include <algorithm>

const std::unordered_set<std::string_view> SourceCodeWrapper::kKeywords = {
    "fun",        "class",   "interface", "var",    "override", "pure",    "if",     "else",      "continue",
    "break",      "for",     "while",     "return", "unsafe",   "val",     "static", "public",    "private",
    "implements", "as",      "is",        "in",     "null",     "true",    "false",  "typealias", "destructor",
    "call",       "#import", "#define",   "#undef", "#ifdef",   "#ifndef", "#else",  "#endif"};

const std::unordered_set<std::string_view> SourceCodeWrapper::kMultiOps = {
    "*=", "+=", "-=", "/=", "==", "!=", "<=", ">=", "&&", "||", "?:", "?.", "::", ":="};

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

  const char was = src_[current_ - 1];
  --current_;

  if (was == '\n') {
    line_ = std::max<int32_t>(1, line_ - 1);
    size_t i = current_;
    int32_t col = 1;

    while (i > 0 && src_[i - 1] != '\n') {
      --i;
      ++col;
    }

    col_ = col;
    return;
  }

  col_ = std::max<int32_t>(1, col_ - 1);
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

bool SourceCodeWrapper::IsKeyword(std::string_view s) {
  return kKeywords.contains(s);
}

bool SourceCodeWrapper::IsMultiOp(std::string_view s) {
  return kMultiOps.contains(s);
}
