#ifndef PARSER_MATCHTYPE_HPP_
#define PARSER_MATCHTYPE_HPP_

#include <string_view>

#include "ITokenMatcher.hpp"
#include "tokens/Token.hpp"

namespace ovum::compiler::parser {

class MatchType : public ITokenMatcher {
public:
  explicit MatchType(std::string_view type_name);
  bool TryMatch(const Token& token) const override;

private:
  std::string_view type_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_MATCHTYPE_HPP_
