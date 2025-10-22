#ifndef ITYPEPARSER_HPP_
#define ITYPEPARSER_HPP_

#include <memory>

#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
#include "lib/parser/types/TypeReference.hpp"

class ITypeParser {
public:
  virtual ~ITypeParser() = default;
  virtual std::unique_ptr<TypeReference> ParseType(ITokenStream& ts, IDiagnosticSink& diags) = 0;
};

#endif // ITYPEPARSER_HPP_
