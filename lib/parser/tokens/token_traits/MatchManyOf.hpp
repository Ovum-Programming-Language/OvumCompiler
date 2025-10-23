#ifndef MATCHMANYOF_HPP_
#define MATCHMANYOF_HPP_

#include <memory>
#include <vector>

#include "ITokenMatcher.hpp"
#include "lib/lexer/tokens/Token.hpp"

class MatchAnyOf : public ITokenMatcher {
public:
  explicit MatchAnyOf(std::vector<std::unique_ptr<ITokenMatcher>> ms);

  bool Match(const Token& tok) const override;

private:
  std::vector<std::unique_ptr<ITokenMatcher>> ms_;
};

#endif // MATCHMANYOF_HPP_
