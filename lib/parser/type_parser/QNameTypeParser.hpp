#ifndef QNAMETYPEPARSER_HPP_
#define QNAMETYPEPARSER_HPP_

#include <memory>

#include "ITypeParser.hpp"
#include "lib/parser/ast/IAstFactory.hpp"
#include "lib/parser/diagnostics/IDiagnosticSink.hpp"
#include "lib/parser/tokens/token_streams/ITokenStream.hpp"
#include "lib/parser/types/TypeReference.hpp"

class QNameTypeParser final : public ITypeParser {
public:
  explicit QNameTypeParser(IAstFactory& factory) noexcept;
  ~QNameTypeParser() override = default;

  [[nodiscard]] std::unique_ptr<TypeReference> ParseType(ITokenStream& ts, IDiagnosticSink& diags) override;

  QNameTypeParser(const QNameTypeParser&) = delete;
  QNameTypeParser& operator=(const QNameTypeParser&) = delete;

  QNameTypeParser(QNameTypeParser&&) noexcept = default;
  QNameTypeParser& operator=(QNameTypeParser&&) noexcept = delete;

private:
  std::shared_ptr<IAstFactory> factory_;
};

#endif // QNAMETYPEPARSER_HPP_
