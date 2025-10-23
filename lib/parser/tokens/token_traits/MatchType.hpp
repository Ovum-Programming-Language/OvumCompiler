#ifndef MATCHTYPE_HPP_
#define MATCHTYPE_HPP_

#include <string_view>

#include "ITokenMatcher.hpp"
#include "lib/lexer/tokens/Token.hpp"

class MatchType : public ITokenMatcher {
public:
  explicit MatchType(std::string_view type_name);

  bool Match(const Token& tok) const override;

private:
  std::string_view type_;
};

#endif // MATCHTYPE_HPP_
