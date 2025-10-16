#ifndef TOKENFACTORY_HPP_
#define TOKENFACTORY_HPP_

#include <memory>
#include <string>

#include "Token.hpp"

namespace TokenFactory {

std::unique_ptr<Token> MakeIdent(std::string lex, int32_t line, int32_t col);

std::unique_ptr<Token> MakeKeyword(std::string lex, int32_t line, int32_t col);

std::unique_ptr<Token> MakeOperator(std::string lex, int32_t line, int32_t col);

std::unique_ptr<Token> MakePunct(char ch, int32_t line, int32_t col);

std::unique_ptr<Token> MakePunct(std::string lex, int32_t line, int32_t col);

std::unique_ptr<Token> MakeNewline(int32_t line, int32_t col);

std::unique_ptr<Token> MakeComment(std::string text, int32_t line, int32_t col);

std::unique_ptr<Token> MakeEof(int32_t line, int32_t col);

std::unique_ptr<Token> MakeIntLiteral(std::string raw, int64_t v, int32_t line, int32_t col);

std::unique_ptr<Token> MakeFloatLiteral(std::string raw, long double v, int32_t line, int32_t col);

std::unique_ptr<Token> MakeStringLiteral(std::string raw, std::string s, int32_t line, int32_t col);

std::unique_ptr<Token> MakeCharLiteral(std::string raw, char c, int32_t line, int32_t col);

std::unique_ptr<Token> MakeBoolLiteral(std::string raw, bool b, int32_t line, int32_t col);

}; // namespace TokenFactory

#endif // TOKENFACTORY_HPP_
