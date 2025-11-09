#include "lib/parser/diagnostics/RelatedInfo.hpp"

#include <utility>

RelatedInfo::RelatedInfo(std::string&& message, SourceSpan&& span) :
    message_(std::move(message)), where_(std::move(span)) {
}
