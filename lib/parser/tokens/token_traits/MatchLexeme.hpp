#ifndef MATCHLEXEME_HPP_
#define MATCHLEXEME_HPP_

#include <string_view>

#include "ITokenMatcher.hpp"
#include "lib/lexer/tokens/Token.hpp"

class MatchLexeme : public ITokenMatcher {
public:
  explicit MatchLexeme(std::string_view lexeme);

  bool Match(const Token& tok) const override;

private:
  std::string_view lexeme_;
};

#endif // MATCHLEXEME_HPP_
