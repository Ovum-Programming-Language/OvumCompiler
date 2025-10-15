#ifndef TOKENTYPE_HPP_
#define TOKENTYPE_HPP_

#include <cstdint>
#include <string_view>

enum TokenType : std::uint8_t {
  kIdent,
  kKeyword,
  kInt,
  kFloat,
  kString,
  kChar,
  kBool,
  kOperator,
  kPunct,
  kNewline,
  kComment,
  kEofT
};

inline std::string_view to_string_view(TokenType token_type) {
  switch (token_type) {
    case TokenType::kIdent:
      return "IDENT";
    case TokenType::kKeyword:
      return "KEYWORD";
    case TokenType::kInt:
      return "INT";
    case TokenType::kFloat:
      return "FLOAT";
    case TokenType::kString:
      return "STRING";
    case TokenType::kChar:
      return "CHAR";
    case TokenType::kBool:
      return "BOOL";
    case TokenType::kOperator:
      return "OPERATOR";
    case TokenType::kPunct:
      return "PUNCT";
    case TokenType::kNewline:
      return "NEWLINE";
    case TokenType::kComment:
      return "COMMENT";
    case TokenType::kEofT:
      return "EOF";
    default:
      return "UNKNOWN";
  }
}

#endif // TOKENTYPE_HPP_
