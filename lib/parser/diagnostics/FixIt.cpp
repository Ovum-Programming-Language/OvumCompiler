#include "lib/parser/diagnostics/FixIt.hpp"

#include <utility>

namespace ovum::compiler::parser {

FixIt::FixIt(SourceSpan source_span, std::string replacement) :
    where_(std::move(source_span)), replacement_(std::move(replacement)) {
}

} // namespace ovum::compiler::parser
