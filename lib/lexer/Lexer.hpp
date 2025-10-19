#ifndef LEXER_HPP_
#define LEXER_HPP_

#include <array>
#include <cstddef>
#include <memory>
#include <string_view>
#include <vector>

#include "SourceCodeWrapper.hpp"
#include "handlers/Handler.hpp"

constexpr std::size_t kDefaultTokenReserve = 256;

class Lexer {
public:
  explicit Lexer(std::string_view src, bool keep_comments = false);

  std::vector<TokenPtr> Tokenize();

  void SetHandler(unsigned char ch, std::unique_ptr<Handler> handler);

  void SetDefaultHandler(std::unique_ptr<Handler> handler);

private:
  static std::array<std::unique_ptr<Handler>, kDefaultTokenReserve> MakeDefaultHandlers();

  static std::unique_ptr<Handler> MakeDefaultHandler();

  SourceCodeWrapper wrapper_;
  std::array<std::unique_ptr<Handler>, kDefaultTokenReserve> handlers_{};
  std::unique_ptr<Handler> default_handler_;
};

#endif // LEXER_HPP_
