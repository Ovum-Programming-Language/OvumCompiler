#ifndef LEXER_SOURCECODEWRAPPER_HPP_
#define LEXER_SOURCECODEWRAPPER_HPP_

#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_set>

namespace ovum::compiler::lexer {

class SourceCodeWrapper {
public:
  SourceCodeWrapper(std::string_view src, bool keep_comments = false);

  [[nodiscard]] bool IsAtEnd() const noexcept;

  [[nodiscard]] char Peek(size_t offset = 0) const noexcept;

  [[nodiscard]] char CurrentChar() const noexcept;

  char Advance();

  void RetreatOne();

  void ConsumeWhile(std::string& out, const std::function<bool(char)>& pred);

  void ResetTokenPosition();

  [[nodiscard]] std::string GetRawLexeme() const;

  [[nodiscard]] int32_t GetLine() const noexcept;

  [[nodiscard]] int32_t GetCol() const noexcept;

  [[nodiscard]] int32_t GetTokenCol() const noexcept;

  [[nodiscard]] bool IsKeepComments() const noexcept;

  [[nodiscard]] static bool IsKeyword(std::string_view s);

  [[nodiscard]] static bool IsMultiOp(std::string_view s);

private:
  std::string_view src_;

  bool keep_comments_;
  size_t start_{0};
  size_t current_{0};
  int32_t line_{1};
  int32_t col_{1};
  int32_t token_col_{1};

  static const std::unordered_set<std::string_view> kKeywords;
  static const std::unordered_set<std::string_view> kMultiOps;
};

} // namespace ovum::compiler::lexer

#endif // LEXER_SOURCECODEWRAPPER_HPP_
