#ifndef TOKENPROCESSOR_HPP_
#define TOKENPROCESSOR_HPP_

#include <memory>
#include <vector>

#include "lib/lexer/tokens/Token.hpp"

using TokenPtr = std::unique_ptr<Token>;

class TokenProcessor { // NOLINT(cppcoreguidelines-special-member-functions)
public:
  virtual ~TokenProcessor() = default;

  [[nodiscard]] virtual std::vector<TokenPtr> Process(const std::vector<TokenPtr>& tokens) const = 0;
};

#endif // TOKENPROCESSOR_HPP_
