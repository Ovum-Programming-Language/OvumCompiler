#ifndef PRATTEXPRESSIONPARSER_HPP_
#define PRATTEXPRESSIONPARSER_HPP_

#include <memory>
#include <vector>

#include "IExpressionParser.hpp"
#include "IOperatorResolver.hpp"
#include "lib/parser/ast/IAstFactory.hpp"
#include "lib/parser/ast/nodes/base/Expr.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
#include "specifications/InfixSpec.hpp"

class PrattExpressionParser : public IExpressionParser {
public:
  explicit PrattExpressionParser(std::unique_ptr<IOperatorResolver> r, IAstFactory* factory);
  ~PrattExpressionParser() override = default;

  std::unique_ptr<Expr> Parse(ITokenStream& ts, IDiagnosticSink& diags) override;

  std::unique_ptr<Expr> ParseExpr(ITokenStream& ts, IDiagnosticSink& diags, int min_bp);
  std::unique_ptr<Expr> ParsePrefix(ITokenStream& ts, IDiagnosticSink& diags);
  std::unique_ptr<Expr> ParsePostfix(ITokenStream& ts, IDiagnosticSink& diags, std::unique_ptr<Expr> base);

  std::vector<std::unique_ptr<Expr>> ParseArgList(ITokenStream& ts, IDiagnosticSink& diags, char closing);

private:
  std::unique_ptr<Expr> MakeInfix(const InfixSpec& spec, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs);

  std::unique_ptr<IOperatorResolver> r_;
  IAstFactory* factory_;
};

#endif // PRATTEXPRESSIONPARSER_HPP_
