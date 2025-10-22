#ifndef QNAMETYPEPARSER_HPP_
#define QNAMETYPEPARSER_HPP_

#include <memory>

#include "ITypeParser.hpp"
#include "lib/parser/ast/IAstFactory.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
#include "lib/parser/types/TypeReference.hpp"

class QNameTypeParser : public ITypeParser {
public:
  explicit QNameTypeParser(IAstFactory* factory);
  ~QNameTypeParser() override = default;

  std::unique_ptr<TypeReference> ParseType(ITokenStream& ts, IDiagnosticSink& diags) override;

private:
  IAstFactory* factory_;
};

#endif // QNAMETYPEPARSER_HPP_
