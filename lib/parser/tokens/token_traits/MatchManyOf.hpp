#ifndef PARSER_MATCHMANYOF_HPP_
#define PARSER_MATCHMANYOF_HPP_

#include <memory>
#include <vector>

#include "ITokenMatcher.hpp"
#include "tokens/Token.hpp"

namespace ovum::compiler::parser {

class MatchAnyOf : public ITokenMatcher {
public:
  explicit MatchAnyOf(std::vector<std::unique_ptr<ITokenMatcher>> matchers);
  bool TryMatch(const Token& token) const override;

private:
  std::vector<std::unique_ptr<ITokenMatcher>> matchers_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_MATCHMANYOF_HPP_
