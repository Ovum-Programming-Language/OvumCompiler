#ifndef LEXER_LEXER_HPP_
#define LEXER_LEXER_HPP_

#include <array>
#include <cstddef>
#include <expected>
#include <memory>
#include <string_view>
#include <vector>

#include <tokens/Token.hpp>

#include "LexerError.hpp"
#include "SourceCodeWrapper.hpp"
#include "handlers/Handler.hpp"

namespace ovum::compiler::lexer {

using TokenPtr = ovum::TokenPtr;

constexpr std::size_t kDefaultTokenReserve = 256;

class Lexer {
public:
  explicit Lexer(std::string_view src, bool keep_comments = false);

  std::expected<std::vector<TokenPtr>, LexerError> Tokenize();

  void SetHandler(unsigned char ch, std::unique_ptr<Handler> handler);

  void SetDefaultHandler(std::unique_ptr<Handler> handler);

private:
  static std::array<std::unique_ptr<Handler>, kDefaultTokenReserve> MakeDefaultHandlers();

  static std::unique_ptr<Handler> MakeDefaultHandler();

  SourceCodeWrapper wrapper_;
  std::array<std::unique_ptr<Handler>, kDefaultTokenReserve> handlers_{};
  std::unique_ptr<Handler> default_handler_;
};

} // namespace ovum::compiler::lexer

#endif // LEXER_LEXER_HPP_
