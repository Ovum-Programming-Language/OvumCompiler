#ifndef PARSER_MATCHLEXEME_HPP_
#define PARSER_MATCHLEXEME_HPP_

#include <string_view>

#include <tokens/Token.hpp>

#include "ITokenMatcher.hpp"

namespace ovum::compiler::parser {

class MatchLexeme : public ITokenMatcher {
public:
  explicit MatchLexeme(std::string_view lexeme);

  bool Match(const Token& tok) const override;

private:
  std::string_view lexeme_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_MATCHLEXEME_HPP_
