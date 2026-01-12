#ifndef PARSER_FIXIT_HPP_
#define PARSER_FIXIT_HPP_

#include <string>

#include "lib/parser/tokens/SourceSpan.hpp"

namespace ovum::compiler::parser {

class FixIt {
public:
  FixIt(SourceSpan source_span, std::string replacement);

private:
  SourceSpan where_;
  std::string replacement_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_FIXIT_HPP_
