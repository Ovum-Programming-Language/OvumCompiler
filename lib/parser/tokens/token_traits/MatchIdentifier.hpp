#ifndef PARSER_MATCHIDENTIFIER_HPP_
#define PARSER_MATCHIDENTIFIER_HPP_

#include "tokens/Token.hpp"
#include "ITokenMatcher.hpp"

namespace ovum::compiler::parser {

class MatchIdentifier : public ITokenMatcher {
public:
  bool TryMatch(const Token& token) const override;
};

} // namespace ovum::compiler::parser

#endif // PARSER_MATCHIDENTIFIER_HPP_
