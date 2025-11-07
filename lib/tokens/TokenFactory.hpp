#ifndef TOKENS_TOKENFACTORY_HPP_
#define TOKENS_TOKENFACTORY_HPP_

#include <string>

#include "Token.hpp"

namespace ovum::TokenFactory { // NOLINT(readability-identifier-naming)

TokenPtr MakeIdent(std::string lex, int32_t line, int32_t col);

TokenPtr MakeKeyword(std::string lex, int32_t line, int32_t col);

TokenPtr MakeOperator(std::string lex, int32_t line, int32_t col);

TokenPtr MakePunct(char ch, int32_t line, int32_t col);

TokenPtr MakePunct(std::string lex, int32_t line, int32_t col);

TokenPtr MakeNewline(int32_t line, int32_t col);

TokenPtr MakeComment(std::string text, int32_t line, int32_t col);

TokenPtr MakeEof(int32_t line, int32_t col);

TokenPtr MakeIntLiteral(std::string raw, int64_t v, int32_t line, int32_t col);

TokenPtr MakeFloatLiteral(std::string raw, double v, int32_t line, int32_t col);

TokenPtr MakeStringLiteral(std::string raw, std::string s, int32_t line, int32_t col);

TokenPtr MakeCharLiteral(std::string raw, char c, int32_t line, int32_t col);

TokenPtr MakeBoolLiteral(std::string raw, bool b, int32_t line, int32_t col);

} // namespace ovum::TokenFactory

#endif // TOKENS_TOKENFACTORY_HPP_
