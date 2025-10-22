#ifndef IEXPRESSIONPARSER_HPP_
#define IEXPRESSIONPARSER_HPP_

#include <memory>

#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"

class IExpressionParser {
public:
  virtual ~IExpressionParser() = default;

  virtual std::unique_ptr<Expr> Parse(ITokenStream& ts, IDiagnosticSink& diags) = 0;
};

#endif // IEXPRESSIONPARSER_HPP_
