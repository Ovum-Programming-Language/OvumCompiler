#ifndef PARSER_IPARSER_HPP_
#define PARSER_IPARSER_HPP_

#include <memory>

#include "ast/nodes/decls/Module.hpp"
#include "diagnostics/IDiagnosticSink.hpp"
#include "tokens/token_streams/ITokenStream.hpp"

namespace ovum::compiler::parser {

class IParser { // NOLINT(cppcoreguidelines-special-member-functions)
public:
  virtual ~IParser() = default;
  virtual std::unique_ptr<Module> Parse(ITokenStream& ts, IDiagnosticSink& diags) = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_IPARSER_HPP_
