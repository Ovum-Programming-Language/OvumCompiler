#ifndef LEXER_HPP_
#define LEXER_HPP_

#include <array>
#include <cstddef>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

#include "handlers/Handler.hpp"

constexpr std::size_t kNumAsciiChars = 256;

class Lexer {
public:
  explicit Lexer(std::string_view src, bool keep_comments = false);

  std::vector<TokenPtr> Tokenize();

  [[nodiscard]] bool IsAtEnd() const noexcept;

  [[nodiscard]] char Peek(size_t offset = 0) const noexcept;

  [[nodiscard]] char CurrentChar() const noexcept;

  char Advance();

  void RetreatOne();

  void ConsumeWhile(std::string& out, const std::function<bool(char)>& pred);

  [[nodiscard]] std::string GetRawLexeme() const;

  [[nodiscard]] int GetLine() const noexcept {
    return line_;
  }

  [[nodiscard]] int GetTokenCol() const noexcept {
    return token_col_;
  }

  [[nodiscard]] bool IsKeepComments() const noexcept {
    return keep_comments_;
  }

  [[nodiscard]] bool IsKeyword(std::string_view s) const;

  [[nodiscard]] bool IsMultiop(std::string_view s) const;

  void SetHandler(unsigned char c, std::unique_ptr<Handler> handler) {
    handlers_.at(c) = std::move(handler);
  }

  void SetDefaultHandler(std::unique_ptr<Handler> handler) {
    default_handler_ = std::move(handler);
  }

private:
  void RegisterDefaults();

  std::string_view src_;
  bool keep_comments_;

  size_t start_{0};
  size_t current_{0};
  int line_{1};
  int col_{1};
  int token_col_{1};

  std::array<std::unique_ptr<Handler>, kNumAsciiChars> handlers_;

  std::unique_ptr<Handler> default_handler_;
};

#endif // LEXER_HPP_
