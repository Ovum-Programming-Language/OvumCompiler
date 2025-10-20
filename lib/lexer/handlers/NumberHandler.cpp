#include "NumberHandler.hpp"

#include <cctype>
#include <string>

#include "lib/lexer/LexerError.hpp"
#include "tokens/TokenFactory.hpp"

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

inline bool ParseExponent(SourceCodeWrapper& w, std::string& raw) {
  if (w.Peek() == 'e' || w.Peek() == 'E') {
    raw.push_back(w.Advance());

    if (w.Peek() == '+' || w.Peek() == '-') {
      raw.push_back(w.Advance());
    }

    if (!IsDec(w.Peek())) {
      throw LexerError("Malformed exponent");
    }

    w.ConsumeWhile(raw, IsDec);
    return true;
  }

  return false;
}

inline double ParseDoubleStrict(const std::string& raw) {
  try {
    return std::stod(raw);
  } catch (...) {
    throw LexerError(std::string("Malformed float literal: ") + raw);
  }
}

inline long long ParseDecIntStrict(const std::string& raw) {
  try {
    return std::stoll(raw);
  } catch (...) {
    throw LexerError(std::string("Malformed integer literal: ") + raw);
  }
}

inline void EnsureNoIdentTail(SourceCodeWrapper& w, const char* ctx) {
  if (IsIdentStart(w.Peek())) {
    throw LexerError(std::string("Unexpected identifier after ") + ctx);
  }
}

inline void EnsureNoSecondDotWithDigits(SourceCodeWrapper& w) {
  if (w.Peek() == '.' && IsDec(w.Peek(1))) {
    throw LexerError("Malformed float literal: duplicate decimal point");
  }
}

OptToken NumberHandler::Scan(SourceCodeWrapper& wrapper) {
  std::string raw;
  const char first = wrapper.CurrentChar();

  if (first == '.') {
    raw.push_back('.');

    if (!IsDec(wrapper.Peek())) {
      return std::nullopt;
    }

    wrapper.ConsumeWhile(raw, IsDec);
    ParseExponent(wrapper, raw);
    EnsureNoSecondDotWithDigits(wrapper);
    EnsureNoIdentTail(wrapper, "number");
    const double v = ParseDoubleStrict(raw);

    return TokenFactory::MakeFloatLiteral(std::move(raw), v, wrapper.GetLine(), wrapper.GetTokenCol());
  }

  wrapper.RetreatOne();

  if (wrapper.Peek() == '0' && (wrapper.Peek(1) == 'x' || wrapper.Peek(1) == 'X')) {
    raw.push_back(wrapper.Advance());
    raw.push_back(wrapper.Advance());

    if (!IsHex(wrapper.Peek())) {
      throw LexerError("Malformed hex literal: expected hex digit after 0x");
    }

    wrapper.ConsumeWhile(raw, IsHex);

    if (wrapper.Peek() == '.') {
      throw LexerError("Hex literal cannot have decimal point");
    }

    EnsureNoIdentTail(wrapper, "hex literal");

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
      throw LexerError("Malformed binary literal: expected binary digit after 0b");
    }

    wrapper.ConsumeWhile(raw, IsBin);

    if (wrapper.Peek() == '.') {
      throw LexerError("Binary literal cannot have decimal point");
    }

    EnsureNoIdentTail(wrapper, "binary literal");
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

    ParseExponent(wrapper, raw);
    EnsureNoSecondDotWithDigits(wrapper);
    EnsureNoIdentTail(wrapper, "number");
    const double v = ParseDoubleStrict(raw);
    return TokenFactory::MakeFloatLiteral(std::move(raw), v, wrapper.GetLine(), wrapper.GetTokenCol());
  }

  const bool had_exp = ParseExponent(wrapper, raw);

  if (had_exp) {
    EnsureNoIdentTail(wrapper, "number");
    const double v = ParseDoubleStrict(raw);
    return TokenFactory::MakeFloatLiteral(std::move(raw), v, wrapper.GetLine(), wrapper.GetTokenCol());
  }

  if (!raw.empty() && (raw.back() == 'e' || raw.back() == 'E' || raw.back() == '+' || raw.back() == '-')) {
    throw LexerError(std::string("Malformed float literal: ") + raw);
  }

  EnsureNoIdentTail(wrapper, "integer literal");
  const long long vi = ParseDecIntStrict(raw);

  return TokenFactory::MakeIntLiteral(std::move(raw), vi, wrapper.GetLine(), wrapper.GetTokenCol());
}
