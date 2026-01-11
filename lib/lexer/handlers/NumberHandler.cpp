#include "NumberHandler.hpp"

#include <cctype>
#include <string>

#include <tokens/TokenFactory.hpp>

#include "lib/lexer/LexerError.hpp"

namespace ovum::compiler::lexer {

namespace {
inline constexpr int kHexAlphaOffset = 10;
inline constexpr int kHexNibbleBits = 4;
} // namespace

inline bool IsDec(char c) noexcept {
  return std::isdigit(static_cast<unsigned char>(c)) != 0;
}

inline bool IsHex(char c) noexcept {
  const auto u = static_cast<unsigned char>(c);
  return std::isdigit(u) != 0 || (u >= 'a' && u <= 'f') || (u >= 'A' && u <= 'F');
}

inline bool IsBin(char c) noexcept {
  return c == '0' || c == '1';
}

inline bool IsIdentStart(char c) noexcept {
  const auto u = static_cast<unsigned char>(c);
  return std::isalpha(u) != 0 || u == '_';
}

inline std::expected<bool, LexerError> ParseExponent(SourceCodeWrapper& w, std::string& raw) {
  if (w.Peek() == 'e' || w.Peek() == 'E') {
    raw.push_back(w.Advance());

    if (w.Peek() == '+' || w.Peek() == '-') {
      raw.push_back(w.Advance());
    }

    if (!IsDec(w.Peek())) {
      return std::unexpected(LexerError("Malformed exponent"));
    }

    w.ConsumeWhile(raw, IsDec);
    return true;
  }

  return false;
}

inline std::expected<double, LexerError> ParseDoubleStrict(const std::string& raw) {
  try {
    return std::stod(raw);
  } catch (...) {
    return std::unexpected(LexerError(std::string("Malformed float literal: ") + raw));
  }
}

inline std::expected<long long, LexerError> ParseDecIntStrict(const std::string& raw) {
  try {
    return std::stoll(raw);
  } catch (...) {
    return std::unexpected(LexerError(std::string("Malformed integer literal: ") + raw));
  }
}

inline std::expected<void, LexerError> EnsureNoIdentTail(SourceCodeWrapper& w, const char* ctx) {
  if (IsIdentStart(w.Peek())) {
    return std::unexpected(LexerError(std::string("Unexpected identifier after ") + ctx));
  }
  return {};
}

inline std::expected<void, LexerError> EnsureNoSecondDotWithDigits(SourceCodeWrapper& w) {
  if (w.Peek() == '.' && IsDec(w.Peek(1))) {
    return std::unexpected(LexerError("Malformed float literal: duplicate decimal point"));
  }
  return {};
}

std::expected<OptToken, LexerError> NumberHandler::Scan(SourceCodeWrapper& wrapper) {
  std::string raw;
  const char first = wrapper.CurrentChar();

  if (first == '.') {
    raw.push_back('.');

    if (!IsDec(wrapper.Peek())) {
      return std::nullopt;
    }

    wrapper.ConsumeWhile(raw, IsDec);
    auto exp_result = ParseExponent(wrapper, raw);
    if (!exp_result) {
      return std::unexpected(exp_result.error());
    }
    auto dot_result = EnsureNoSecondDotWithDigits(wrapper);
    if (!dot_result) {
      return std::unexpected(dot_result.error());
    }
    auto ident_result = EnsureNoIdentTail(wrapper, "number");
    if (!ident_result) {
      return std::unexpected(ident_result.error());
    }
    auto v_result = ParseDoubleStrict(raw);
    if (!v_result) {
      return std::unexpected(v_result.error());
    }

    return TokenFactory::MakeFloatLiteral(std::move(raw), v_result.value(), wrapper.GetLine(), wrapper.GetTokenCol());
  }

  wrapper.RetreatOne();

  if (wrapper.Peek() == '0' && (wrapper.Peek(1) == 'x' || wrapper.Peek(1) == 'X')) {
    raw.push_back(wrapper.Advance());
    raw.push_back(wrapper.Advance());

    if (!IsHex(wrapper.Peek())) {
      return std::unexpected(LexerError("Malformed hex literal: expected hex digit after 0x"));
    }

    wrapper.ConsumeWhile(raw, IsHex);

    if (wrapper.Peek() == '.') {
      return std::unexpected(LexerError("Hex literal cannot have decimal point"));
    }

    auto ident_result = EnsureNoIdentTail(wrapper, "hex literal");
    if (!ident_result) {
      return std::unexpected(ident_result.error());
    }

    long long val = 0;

    for (size_t i = 2; i < raw.size(); ++i) {
      const char c = raw[i];
      val <<= kHexNibbleBits;

      if (c >= '0' && c <= '9') {
        val |= static_cast<long long>(c - '0');
      } else if (c >= 'a' && c <= 'f') {
        val |= static_cast<long long>(kHexAlphaOffset + c - 'a');
      } else {
        val |= static_cast<long long>(kHexAlphaOffset + c - 'A');
      }
    }

    return TokenFactory::MakeIntLiteral(std::move(raw), val, wrapper.GetLine(), wrapper.GetTokenCol());
  }

  if (wrapper.Peek() == '0' && (wrapper.Peek(1) == 'b' || wrapper.Peek(1) == 'B')) {
    raw.push_back(wrapper.Advance());
    raw.push_back(wrapper.Advance());

    if (!IsBin(wrapper.Peek())) {
      return std::unexpected(LexerError("Malformed binary literal: expected binary digit after 0b"));
    }

    wrapper.ConsumeWhile(raw, IsBin);

    if (wrapper.Peek() == '.') {
      return std::unexpected(LexerError("Binary literal cannot have decimal point"));
    }

    auto ident_result = EnsureNoIdentTail(wrapper, "binary literal");
    if (!ident_result) {
      return std::unexpected(ident_result.error());
    }
    long long val = 0;

    for (size_t i = 2; i < raw.size(); ++i) {
      val = (val << 1) | (raw[i] - '0');
    }

    return TokenFactory::MakeIntLiteral(std::move(raw), val, wrapper.GetLine(), wrapper.GetTokenCol());
  }

  wrapper.ConsumeWhile(raw, IsDec);

  if (wrapper.Peek() == '.') {
    raw.push_back(wrapper.Advance());

    if (IsDec(wrapper.Peek())) {
      wrapper.ConsumeWhile(raw, IsDec);
    }

    auto exp_result = ParseExponent(wrapper, raw);
    if (!exp_result) {
      return std::unexpected(exp_result.error());
    }
    auto dot_result = EnsureNoSecondDotWithDigits(wrapper);
    if (!dot_result) {
      return std::unexpected(dot_result.error());
    }
    auto ident_result = EnsureNoIdentTail(wrapper, "number");
    if (!ident_result) {
      return std::unexpected(ident_result.error());
    }
    auto v_result = ParseDoubleStrict(raw);
    if (!v_result) {
      return std::unexpected(v_result.error());
    }
    return TokenFactory::MakeFloatLiteral(std::move(raw), v_result.value(), wrapper.GetLine(), wrapper.GetTokenCol());
  }

  auto exp_result = ParseExponent(wrapper, raw);
  if (!exp_result) {
    return std::unexpected(exp_result.error());
  }
  const bool had_exp = exp_result.value();

  if (had_exp) {
    auto ident_result = EnsureNoIdentTail(wrapper, "number");
    if (!ident_result) {
      return std::unexpected(ident_result.error());
    }
    auto v_result = ParseDoubleStrict(raw);
    if (!v_result) {
      return std::unexpected(v_result.error());
    }
    return TokenFactory::MakeFloatLiteral(std::move(raw), v_result.value(), wrapper.GetLine(), wrapper.GetTokenCol());
  }

  if (!raw.empty() && (raw.back() == 'e' || raw.back() == 'E' || raw.back() == '+' || raw.back() == '-')) {
    return std::unexpected(LexerError(std::string("Malformed float literal: ") + raw));
  }

  // Check for byte literal suffix (b or B) before EnsureNoIdentTail
  // Store the current position to check for 'b' suffix
  char next_char = wrapper.Peek();
  if (next_char == 'b' || next_char == 'B') {
    // Parse the number part without the 'b' suffix
    auto vi_result = ParseDecIntStrict(raw);
    if (!vi_result) {
      return std::unexpected(vi_result.error());
    }
    long long value = vi_result.value();
    // Clamp to byte range [0, 255]
    if (value < 0) {
      value = 0;
    }
    if (value > 255) {
      value = 255;
    }
    // Consume the 'b' suffix
    raw.push_back(wrapper.Advance());
    // Create byte literal token using MakeIntLiteral with special handling
    // The parser will check for 'b' suffix in lexeme to determine if it's a byte literal
    return TokenFactory::MakeIntLiteral(std::move(raw), value, wrapper.GetLine(), wrapper.GetTokenCol());
  }

  auto ident_result = EnsureNoIdentTail(wrapper, "integer literal");
  if (!ident_result) {
    return std::unexpected(ident_result.error());
  }
  auto vi_result = ParseDecIntStrict(raw);
  if (!vi_result) {
    return std::unexpected(vi_result.error());
  }

  return TokenFactory::MakeIntLiteral(std::move(raw), vi_result.value(), wrapper.GetLine(), wrapper.GetTokenCol());
}

} // namespace ovum::compiler::lexer
