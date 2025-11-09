#ifndef PARSER_RELATEDINFO_HPP_
#define PARSER_RELATEDINFO_HPP_

#include <optional>
#include <string>

#include "lib/parser/tokens/SourceSpan.hpp"

namespace ovum::compiler::parser {

class RelatedInfo {
public:
  RelatedInfo(std::string&& message, SourceSpan&& span);

private:
  std::string message_;
  std::optional<SourceSpan> where_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_RELATEDINFO_HPP_
