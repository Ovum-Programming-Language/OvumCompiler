#ifndef PARSER_ITOKENMATCHER_HPP_
#define PARSER_ITOKENMATCHER_HPP_

#include "tokens/Token.hpp"

namespace ovum::compiler::parser {

class ITokenMatcher {
public:
  virtual ~ITokenMatcher() = default;
  virtual bool TryMatch(const Token& token) const = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_ITOKENMATCHER_HPP_
