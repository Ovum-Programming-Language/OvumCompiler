#ifndef LEXER_HPP_
#define LEXER_HPP_

#include <array>
#include <cstddef>
#include <string_view>
#include <vector>

#include "SourceCodeWrapper.hpp"
#include "handlers/Handler.hpp"

constexpr std::size_t kNumAsciiChars = 256;

class Lexer {
public:
  explicit Lexer(std::string_view src, bool keep_comments = false);

  std::vector<TokenPtr> Tokenize();

  void SetHandler(unsigned char c, std::unique_ptr<Handler> handler) {
    handlers_.at(c) = std::move(handler);
  }

  void SetDefaultHandler(std::unique_ptr<Handler> handler) {
    default_handler_ = std::move(handler);
  }

private:
  void RegisterDefaults();

  SourceCodeWrapper wrapper_;
  std::array<std::unique_ptr<Handler>, kNumAsciiChars> handlers_;
  std::unique_ptr<Handler> default_handler_;
};

#endif // LEXER_HPP_
