#include "lib/parser/diagnostics/FixIt.hpp"

#include <utility>

FixIt::FixIt(SourceSpan source_span, std::string replacement) :
    where_(source_span), replacement_(std::move(replacement)) {
}
