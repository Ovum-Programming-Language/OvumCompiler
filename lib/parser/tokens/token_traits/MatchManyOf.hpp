#ifndef PARSER_MATCHMANYOF_HPP_
#define PARSER_MATCHMANYOF_HPP_

#include <memory>
#include <vector>

#include <tokens/Token.hpp>

#include "ITokenMatcher.hpp"

namespace ovum::compiler::parser {

class MatchAnyOf : public ITokenMatcher {
public:
  explicit MatchAnyOf(std::vector<std::unique_ptr<ITokenMatcher>> ms);

  bool Match(const Token& tok) const override;

private:
  std::vector<std::unique_ptr<ITokenMatcher>> ms_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_MATCHMANYOF_HPP_
