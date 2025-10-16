#ifndef TOKENFACTORY_HPP_
#define TOKENFACTORY_HPP_

#include <memory>
#include <string>

#include "CommentToken.hpp"
#include "EofToken.hpp"
#include "IdentToken.hpp"
#include "KeywordToken.hpp"
#include "LiteralToken.hpp"
#include "NewlineToken.hpp"
#include "OperatorToken.hpp"
#include "PunctToken.hpp"
#include "Token.hpp"
#include "lib/lexer/values/BoolValue.hpp"
#include "lib/lexer/values/CharValue.hpp"
#include "lib/lexer/values/FloatValue.hpp"
#include "lib/lexer/values/IntValue.hpp"
#include "lib/lexer/values/StringValue.hpp"

class TokenFactory {
public:
  static std::unique_ptr<Token> MakeIdent(std::string lex, int32_t line, int32_t col) {
    return std::make_unique<IdentToken>(std::move(lex), line, col);
  }

  static std::unique_ptr<Token> MakeKeyword(std::string lex, int32_t line, int32_t col) {
    return std::make_unique<KeywordToken>(std::move(lex), line, col);
  }

  static std::unique_ptr<Token> MakeOperator(std::string lex, int32_t line, int32_t col) {
    return std::make_unique<OperatorToken>(std::move(lex), line, col);
  }

  static std::unique_ptr<Token> MakePunct(char ch, int32_t line, int32_t col) {
    return std::make_unique<PunctToken>(ch, line, col);
  }

  static std::unique_ptr<Token> MakePunct(std::string lex, int32_t line, int32_t col) {
    return std::make_unique<PunctToken>(std::move(lex), line, col);
  }

  static std::unique_ptr<Token> MakeNewline(int32_t line, int32_t col) {
    return std::make_unique<NewlineToken>(line, col);
  }

  static std::unique_ptr<Token> MakeComment(std::string text, int32_t line, int32_t col) {
    return std::make_unique<CommentToken>(std::move(text), line, col);
  }

  static std::unique_ptr<Token> MakeEof(int32_t line, int32_t col) {
    return std::make_unique<EofToken>(line, col);
  }

  static std::unique_ptr<Token> MakeIntLiteral(std::string raw, int64_t v, int32_t line, int32_t col) {
    return std::make_unique<LiteralToken>(TokenType::kInt, std::move(raw), std::make_unique<IntValue>(v), line, col);
  }

  static std::unique_ptr<Token> MakeFloatLiteral(std::string raw, long double v, int32_t line, int32_t col) {
    return std::make_unique<LiteralToken>(
        TokenType::kFloat, std::move(raw), std::make_unique<FloatValue>(v), line, col);
  }

  static std::unique_ptr<Token> MakeStringLiteral(std::string raw, std::string s, int32_t line, int32_t col) {
    return std::make_unique<LiteralToken>(
        TokenType::kString, std::move(raw), std::make_unique<StringValue>(std::move(s)), line, col);
  }

  static std::unique_ptr<Token> MakeCharLiteral(std::string raw, char c, int32_t line, int32_t col) {
    return std::make_unique<LiteralToken>(TokenType::kChar, std::move(raw), std::make_unique<CharValue>(c), line, col);
  }

  static std::unique_ptr<Token> MakeBoolLiteral(std::string raw, bool b, int32_t line, int32_t col) {
    return std::make_unique<LiteralToken>(TokenType::kBool, std::move(raw), std::make_unique<BoolValue>(b), line, col);
  }
};

#endif // TOKENFACTORY_HPP_
