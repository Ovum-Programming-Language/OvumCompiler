#include "NumberHandler.hpp"

#include <cmath>

#include "lib/lexer/LexerError.hpp"
#include "tokens/TokenFactory.hpp"

static bool IsDigitChar(char c) {
  return std::isdigit(static_cast<unsigned char>(c));
}

OptToken NumberHandler::Scan(SourceCodeWrapper& wrapper) {
  std::string raw;
  char first = wrapper.CurrentChar();

  if (first == '.') {
    raw.push_back('.');
    wrapper.ConsumeWhile(raw, [](char c) { return std::isdigit(static_cast<unsigned char>(c)); });

    if (wrapper.Peek() == 'e' || wrapper.Peek() == 'E') {
      raw.push_back(wrapper.Advance());

      if (wrapper.Peek() == '+' || wrapper.Peek() == '-') {
        raw.push_back(wrapper.Advance());
      }

      if (!IsDigitChar(wrapper.Peek())) {
        throw LexerError("Malformed exponent");
      }

      wrapper.ConsumeWhile(raw, [](char ch) { return std::isdigit(static_cast<unsigned char>(ch)); });
    }

    long double v = NAN;

    try {
      v = std::stold(raw);
    } catch (...) {
      throw LexerError(std::string("Malformed float literal: ") + raw);
    }

    return std::make_optional(
        TokenFactory::MakeFloatLiteral(std::move(raw), v, wrapper.GetLine(), wrapper.GetTokenCol()));
  }

  wrapper.RetreatOne();
  wrapper.ConsumeWhile(raw, [](char c) { return std::isdigit(static_cast<unsigned char>(c)); });

  if (wrapper.Peek() == '.' && std::isdigit(static_cast<unsigned char>(wrapper.Peek(1)))) {
    raw.push_back(wrapper.Advance());
    wrapper.ConsumeWhile(raw, [](char c) { return std::isdigit(static_cast<unsigned char>(c)); });

    if (wrapper.Peek() == 'e' || wrapper.Peek() == 'E') {
      raw.push_back(wrapper.Advance());

      if (wrapper.Peek() == '+' || wrapper.Peek() == '-') {
        raw.push_back(wrapper.Advance());
      }

      if (!IsDigitChar(wrapper.Peek())) {
        throw LexerError("Malformed exponent");
      }

      wrapper.ConsumeWhile(raw, [](char ch) { return std::isdigit(static_cast<unsigned char>(ch)); });
    }

    long double v = NAN;

    try {
      v = std::stold(raw);
    } catch (...) {
      throw LexerError(std::string("Malformed float literal: ") + raw);
    }

    return std::make_optional(
        TokenFactory::MakeFloatLiteral(std::move(raw), v, wrapper.GetLine(), wrapper.GetTokenCol()));
  }

  if (wrapper.Peek() == 'e' || wrapper.Peek() == 'E') {
    raw.push_back(wrapper.Advance());

    if (wrapper.Peek() == '+' || wrapper.Peek() == '-') {
      raw.push_back(wrapper.Advance());
    }

    if (!IsDigitChar(wrapper.Peek())) {
      throw LexerError("Malformed exponent");
    }

    wrapper.ConsumeWhile(raw, [](char ch) { return std::isdigit(static_cast<unsigned char>(ch)); });
    long double v = NAN;

    try {
      v = std::stold(raw);
    } catch (...) {
      throw LexerError(std::string("Malformed float literal: ") + raw);
    }

    return std::make_optional(
        TokenFactory::MakeFloatLiteral(std::move(raw), v, wrapper.GetLine(), wrapper.GetTokenCol()));
  }

  int64_t vi = 0;

  try {
    vi = std::stoll(raw);
  } catch (...) {
    throw LexerError(std::string("Malformed integer literal: ") + raw);
  }

  return std::make_optional(TokenFactory::MakeIntLiteral(std::move(raw), vi, wrapper.GetLine(), wrapper.GetTokenCol()));
}
