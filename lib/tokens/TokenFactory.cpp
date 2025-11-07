#include "TokenFactory.hpp"

#include "CommentToken.hpp"
#include "EofToken.hpp"
#include "IdentToken.hpp"
#include "KeywordToken.hpp"
#include "LiteralToken.hpp"
#include "NewlineToken.hpp"
#include "OperatorToken.hpp"
#include "PunctToken.hpp"
#include "values/BoolValue.hpp"
#include "values/CharValue.hpp"
#include "values/FloatValue.hpp"
#include "values/IntValue.hpp"
#include "values/StringValue.hpp"

namespace ovum {

namespace TokenFactory {

TokenPtr MakeIdent(std::string lex, int32_t line, int32_t col) {
  return std::make_shared<IdentToken>(std::move(lex), TokenPosition(line, col));
}

TokenPtr MakeKeyword(std::string lex, int32_t line, int32_t col) {
  return std::make_shared<KeywordToken>(std::move(lex), TokenPosition(line, col));
}

TokenPtr MakeOperator(std::string lex, int32_t line, int32_t col) {
  return std::make_shared<OperatorToken>(std::move(lex), TokenPosition(line, col));
}

TokenPtr MakePunct(char ch, int32_t line, int32_t col) {
  return std::make_shared<PunctToken>(ch, TokenPosition(line, col));
}

TokenPtr MakePunct(std::string lex, int32_t line, int32_t col) {
  return std::make_shared<PunctToken>(std::move(lex), TokenPosition(line, col));
}

TokenPtr MakeNewline(int32_t line, int32_t col) {
  return std::make_shared<NewlineToken>(TokenPosition(line, col));
}

TokenPtr MakeComment(std::string text, int32_t line, int32_t col) {
  return std::make_shared<CommentToken>(std::move(text), TokenPosition(line, col));
}

TokenPtr MakeEof(int32_t line, int32_t col) {
  return std::make_shared<EofToken>(TokenPosition(line, col));
}

TokenPtr MakeIntLiteral(std::string raw, int64_t v, int32_t line, int32_t col) {
  return std::make_shared<LiteralToken>(std::move(raw), std::make_unique<IntValue>(v), TokenPosition(line, col));
}

TokenPtr MakeFloatLiteral(std::string raw, double v, int32_t line, int32_t col) {
  return std::make_shared<LiteralToken>(std::move(raw), std::make_unique<FloatValue>(v), TokenPosition(line, col));
}

TokenPtr MakeStringLiteral(std::string raw, std::string s, int32_t line, int32_t col) {
  return std::make_shared<LiteralToken>(
      std::move(raw), std::make_unique<StringValue>(std::move(s)), TokenPosition(line, col));
}

TokenPtr MakeCharLiteral(std::string raw, char c, int32_t line, int32_t col) {
  return std::make_shared<LiteralToken>(std::move(raw), std::make_unique<CharValue>(c), TokenPosition(line, col));
}

TokenPtr MakeBoolLiteral(std::string raw, bool b, int32_t line, int32_t col) {
  return std::make_shared<LiteralToken>(std::move(raw), std::make_unique<BoolValue>(b), TokenPosition(line, col));
}

} // namespace TokenFactory

} // namespace ovum
