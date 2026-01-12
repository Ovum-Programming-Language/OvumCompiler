#ifndef PARSER_ITYPEPARSER_HPP_
#define PARSER_ITYPEPARSER_HPP_

#include <memory>

#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
#include "lib/parser/types/TypeReference.hpp"

namespace ovum::compiler::parser {

class ITypeParser { // NOLINT(cppcoreguidelines-special-member-functions)
public:
  virtual ~ITypeParser() = default;
  virtual std::unique_ptr<TypeReference> ParseType(ITokenStream& ts, IDiagnosticSink& diags) = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_ITYPEPARSER_HPP_
