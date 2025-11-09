#ifndef PARSER_IEXPRESSIONPARSER_HPP_
#define PARSER_IEXPRESSIONPARSER_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

namespace ovum::compiler::parser {

class IExpressionParser {
public:
  virtual ~IExpressionParser() = default;

  virtual std::unique_ptr<Expr> Parse(ITokenStream& ts, IDiagnosticSink& diags) = 0;
};

} // namespace ovum::compiler::parser

#endif // PARSER_IEXPRESSIONPARSER_HPP_
