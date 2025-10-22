#ifndef IPARSER_HPP_
#define IPARSER_HPP_

#include <memory>

#include "ast/nodes/decls/Module.hpp"
#include "diagnostics/IDiagnosticSink.hpp"
#include "tokens/token_streams/ITokenStream.hpp"

class IParser {
public:
  virtual ~IParser() = default;
  virtual std::unique_ptr<Module> Parse(ITokenStream& ts, IDiagnosticSink& diags) = 0;
};

#endif // IPARSER_HPP_
