#include "NumberHandler.hpp"

#include <cmath>

#include "lib/lexer/LexerError.hpp"
#include "tokens/TokenFactory.hpp"

static bool IsDigitChar(char c) {
  return std::isdigit(static_cast<unsigned char>(c));
}

OptToken NumberHandler::Scan(Lexer& lx) {
  std::string raw;
  char first = lx.CurrentChar();

  if (first == '.') {
    raw.push_back('.');
    lx.ConsumeWhile(raw, [](char c) { return std::isdigit(static_cast<unsigned char>(c)); });

    if (lx.Peek() == 'e' || lx.Peek() == 'E') {
      raw.push_back(lx.Advance());

      if (lx.Peek() == '+' || lx.Peek() == '-') {
        raw.push_back(lx.Advance());
      }

      if (!IsDigitChar(lx.Peek())) {
        throw LexerError("Malformed exponent");
      }

      lx.ConsumeWhile(raw, [](char ch) { return std::isdigit(static_cast<unsigned char>(ch)); });
    }

    long double v = NAN;

    try {
      v = std::stold(raw);
    } catch (...) {
      throw LexerError(std::string("Malformed float literal: ") + raw);
    }

    return std::make_optional(TokenFactory::MakeFloatLiteral(std::move(raw), v, lx.GetLine(), lx.GetTokenCol()));
  }

  lx.RetreatOne();
  lx.ConsumeWhile(raw, [](char c) { return std::isdigit(static_cast<unsigned char>(c)); });

  if (lx.Peek() == '.' && std::isdigit(static_cast<unsigned char>(lx.Peek(1)))) {
    raw.push_back(lx.Advance());
    lx.ConsumeWhile(raw, [](char c) { return std::isdigit(static_cast<unsigned char>(c)); });

    if (lx.Peek() == 'e' || lx.Peek() == 'E') {
      raw.push_back(lx.Advance());

      if (lx.Peek() == '+' || lx.Peek() == '-') {
        raw.push_back(lx.Advance());
      }

      if (!IsDigitChar(lx.Peek())) {
        throw LexerError("Malformed exponent");
      }

      lx.ConsumeWhile(raw, [](char ch) { return std::isdigit(static_cast<unsigned char>(ch)); });
    }

    long double v = NAN;

    try {
      v = std::stold(raw);
    } catch (...) {
      throw LexerError(std::string("Malformed float literal: ") + raw);
    }

    return std::make_optional(TokenFactory::MakeFloatLiteral(std::move(raw), v, lx.GetLine(), lx.GetTokenCol()));
  }

  if (lx.Peek() == 'e' || lx.Peek() == 'E') {
    raw.push_back(lx.Advance());

    if (lx.Peek() == '+' || lx.Peek() == '-') {
      raw.push_back(lx.Advance());
    }

    if (!IsDigitChar(lx.Peek())) {
      throw LexerError("Malformed exponent");
    }

    lx.ConsumeWhile(raw, [](char ch) { return std::isdigit(static_cast<unsigned char>(ch)); });
    long double v = NAN;

    try {
      v = std::stold(raw);
    } catch (...) {
      throw LexerError(std::string("Malformed float literal: ") + raw);
    }

    return std::make_optional(TokenFactory::MakeFloatLiteral(std::move(raw), v, lx.GetLine(), lx.GetTokenCol()));
  }

  int64_t vi = 0;

  try {
    vi = std::stoll(raw);
  } catch (...) {
    throw LexerError(std::string("Malformed integer literal: ") + raw);
  }

  return std::make_optional(TokenFactory::MakeIntLiteral(std::move(raw), vi, lx.GetLine(), lx.GetTokenCol()));
}
