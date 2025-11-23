#ifndef PARSER_MATCHLITERAL_HPP_
#define PARSER_MATCHLITERAL_HPP_

#include "tokens/Token.hpp"
#include "ITokenMatcher.hpp"

namespace ovum::compiler::parser {

class MatchLiteral : public ITokenMatcher {
public:
  bool TryMatch(const Token& token) const override;
};

} // namespace ovum::compiler::parser

#endif // PARSER_MATCHLITERAL_HPP_
