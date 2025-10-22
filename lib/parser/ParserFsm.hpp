#ifndef PARSERFSM_HPP_
#define PARSERFSM_HPP_

#include <memory>

#include "IParser.hpp"

class ParserFsm : public IParser {
public:
  ParserFsm(std::unique_ptr<class IExpressionParser> expr,
            std::unique_ptr<class ITypeParser> typep,
            std::unique_ptr<class IAstFactory> factory);

  ~ParserFsm() override = default;

  std::unique_ptr<Module> Parse(ITokenStream& ts, IDiagnosticSink& diags) override;

private:
  std::unique_ptr<IExpressionParser> expr_parser_;
  std::unique_ptr<ITypeParser> type_parser_;
  std::unique_ptr<IAstFactory> factory_;
};

#endif // PARSERFSM_HPP_
