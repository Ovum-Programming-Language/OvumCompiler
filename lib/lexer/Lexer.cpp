#include "Lexer.hpp"
#include "utils.hpp"

#include "handlers/CharHandler.hpp"
#include "handlers/DefaultHandler.hpp"
#include "handlers/Handler.hpp"
#include "handlers/IdentifierHandler.hpp"
#include "handlers/NewlineHandler.hpp"
#include "handlers/NumberHandler.hpp"
#include "handlers/OperatorHandler.hpp"
#include "handlers/PunctHandler.hpp"
#include "handlers/SlashHandler.hpp"
#include "handlers/StringHandler.hpp"
#include "handlers/WhitespaceHandler.hpp"

#include "tokens/TokenFactory.hpp"

Lexer::Lexer(std::string_view src, bool keep_comments) : src_(src), keep_comments_(keep_comments) {
  RegisterDefaults();
}

bool Lexer::IsAtEnd() const noexcept {
  return current_ >= src_.size();
}

char Lexer::Peek(size_t offset) const noexcept {
  size_t idx = current_ + offset;

  if (idx >= src_.size()) {
    return '\0';
  }

  return src_[idx];
}

char Lexer::CurrentChar() const noexcept {
  if (current_ == 0) {
    return '\0';
  }

  return src_[current_ - 1];
}

char Lexer::Advance() {
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

void Lexer::RetreatOne() {
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

void Lexer::ConsumeWhile(std::string &out, const std::function<bool(char)> &pred) {
  while (!IsAtEnd() && pred(Peek())) {
    out.push_back(Advance());
  }
}

std::string Lexer::GetRawLexeme() const {
  if (current_ >= start_) {
    return std::string(src_.substr(start_, current_ - start_));
  }

  return {};
}

bool Lexer::IsKeyword(std::string_view s) const {
  return KeywordSet().contains(std::string(s));
}

bool Lexer::IsMultiop(std::string_view s) const {
  return MultiOpsSet().contains(std::string(s));
}

std::vector<TokenPtr> Lexer::Tokenize() {
  std::vector<TokenPtr> tokens;

  while (!IsAtEnd()) {
    start_ = current_;
    token_col_ = col_;
    char c = Advance();
    Handler *h = handlers_.at(static_cast<unsigned char>(c)).get();

    if (!h) {
      h = default_handler_.get();
    }

    OptToken maybe = h->Scan(*this);

    if (maybe && *maybe) {
      tokens.push_back(std::move(*maybe));
    }
  }

  tokens.push_back(TokenFactory::make_eof(line_, col_));
  return tokens;
}

void Lexer::RegisterDefaults() {
  for (auto &p : handlers_) {
    p.reset();
  }

  default_handler_.reset();

  SetHandler(' ', std::make_unique<WhitespaceHandler>());
  SetHandler('\t', std::make_unique<WhitespaceHandler>());
  SetHandler('\r', std::make_unique<WhitespaceHandler>());

  SetHandler('\n', std::make_unique<NewlineHandler>());

  for (unsigned char c = 'a'; c <= 'z'; ++c) {
    SetHandler(c, std::make_unique<IdentifierHandler>());
  }

  for (unsigned char c = 'A'; c <= 'Z'; ++c) {
    SetHandler(c, std::make_unique<IdentifierHandler>());
  }

  SetHandler((unsigned char) '_', std::make_unique<IdentifierHandler>());

  for (unsigned char d = '0'; d <= '9'; ++d) {
    SetHandler(d, std::make_unique<NumberHandler>());
  }

  SetHandler((unsigned char) '.', std::make_unique<NumberHandler>());

  SetHandler((unsigned char) '"', std::make_unique<StringHandler>());
  SetHandler((unsigned char) '\'', std::make_unique<CharHandler>());

  SetHandler((unsigned char) '/', std::make_unique<SlashHandler>());

  const std::string opchars = "+-*/%<>=!&|^~?:.";

  for (unsigned char c : opchars) {
    SetHandler(c, std::make_unique<OperatorHandler>());
  }

  const std::string puncts = ",;:(){}[]";

  for (unsigned char c : puncts) {
    SetHandler(c, std::make_unique<PunctHandler>());
  }

  SetDefaultHandler(std::make_unique<DefaultHandler>());
}
