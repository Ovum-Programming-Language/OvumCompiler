#include "TokenFactory.hpp"

#include "CommentToken.hpp"
#include "EofToken.hpp"
#include "IdentToken.hpp"
#include "KeywordToken.hpp"
#include "LiteralToken.hpp"
#include "NewlineToken.hpp"
#include "OperatorToken.hpp"
#include "PunctToken.hpp"
#include "lib/lexer/values/BoolValue.hpp"
#include "lib/lexer/values/CharValue.hpp"
#include "lib/lexer/values/FloatValue.hpp"
#include "lib/lexer/values/IntValue.hpp"
#include "lib/lexer/values/StringValue.hpp"

namespace TokenFactory {

std::unique_ptr<Token> MakeIdent(std::string lex, int32_t line, int32_t col) {
  return std::make_unique<IdentToken>(std::move(lex), TokenPosition(line, col));
}

std::unique_ptr<Token> MakeKeyword(std::string lex, int32_t line, int32_t col) {
  return std::make_unique<KeywordToken>(std::move(lex), TokenPosition(line, col));
}

std::unique_ptr<Token> MakeOperator(std::string lex, int32_t line, int32_t col) {
  return std::make_unique<OperatorToken>(std::move(lex), TokenPosition(line, col));
}

std::unique_ptr<Token> MakePunct(char ch, int32_t line, int32_t col) {
  return std::make_unique<PunctToken>(ch, TokenPosition(line, col));
}

std::unique_ptr<Token> MakePunct(std::string lex, int32_t line, int32_t col) {
  return std::make_unique<PunctToken>(std::move(lex), TokenPosition(line, col));
}

std::unique_ptr<Token> MakeNewline(int32_t line, int32_t col) {
  return std::make_unique<NewlineToken>(TokenPosition(line, col));
}

std::unique_ptr<Token> MakeComment(std::string text, int32_t line, int32_t col) {
  return std::make_unique<CommentToken>(std::move(text), TokenPosition(line, col));
}

std::unique_ptr<Token> MakeEof(int32_t line, int32_t col) {
  return std::make_unique<EofToken>(TokenPosition(line, col));
}

std::unique_ptr<Token> MakeIntLiteral(std::string raw, int64_t v, int32_t line, int32_t col) {
  return std::make_unique<LiteralToken>(std::move(raw), std::make_unique<IntValue>(v), TokenPosition(line, col));
}

std::unique_ptr<Token> MakeFloatLiteral(std::string raw, long double v, int32_t line, int32_t col) {
  return std::make_unique<LiteralToken>(std::move(raw), std::make_unique<FloatValue>(v), TokenPosition(line, col));
}

std::unique_ptr<Token> MakeStringLiteral(std::string raw, std::string s, int32_t line, int32_t col) {
  return std::make_unique<LiteralToken>(
      std::move(raw), std::make_unique<StringValue>(std::move(s)), TokenPosition(line, col));
}

std::unique_ptr<Token> MakeCharLiteral(std::string raw, char c, int32_t line, int32_t col) {
  return std::make_unique<LiteralToken>(std::move(raw), std::make_unique<CharValue>(c), TokenPosition(line, col));
}

std::unique_ptr<Token> MakeBoolLiteral(std::string raw, bool b, int32_t line, int32_t col) {
  return std::make_unique<LiteralToken>(std::move(raw), std::make_unique<BoolValue>(b), TokenPosition(line, col));
}

}; // namespace TokenFactory
