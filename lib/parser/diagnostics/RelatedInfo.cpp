#include "lib/parser/diagnostics/RelatedInfo.hpp"

#include <utility>

namespace ovum::compiler::parser {

RelatedInfo::RelatedInfo(std::string&& message, SourceSpan&& span) :
    message_(std::move(message)), where_(std::move(span)) {
}

} // namespace ovum::compiler::parser
