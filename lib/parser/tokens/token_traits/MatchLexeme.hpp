#ifndef PARSER_MATCHLEXEME_HPP_
#define PARSER_MATCHLEXEME_HPP_

#include <string_view>

#include "ITokenMatcher.hpp"
#include "tokens/Token.hpp"

namespace ovum::compiler::parser {

class MatchLexeme : public ITokenMatcher {
public:
  explicit MatchLexeme(std::string_view lexeme);
  bool TryMatch(const Token& token) const override;

private:
  std::string_view lexeme_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_MATCHLEXEME_HPP_
