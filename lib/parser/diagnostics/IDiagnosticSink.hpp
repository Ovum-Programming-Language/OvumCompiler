#ifndef IDIAGNOSTICSINK_HPP_
#define IDIAGNOSTICSINK_HPP_

#include <cstddef>
#include <optional>
#include <string_view>

#include "Diagnostic.hpp"
#include "lib/parser/tokens/SourceSpan.hpp"

class IDiagnosticSink {
public:
  virtual ~IDiagnosticSink() = default;

  virtual void Report(Diagnostic d) = 0;

  virtual bool HasErrors() const = 0;
  virtual std::size_t Count() const = 0;
  virtual std::size_t ErrorCount() const = 0;
  virtual std::size_t WarningCount() const = 0;

  virtual void Note(std::string_view code, std::string_view msg, std::optional<SourceSpan> where = std::nullopt) = 0;
  virtual void Warn(std::string_view code, std::string_view msg, std::optional<SourceSpan> where = std::nullopt) = 0;
  virtual void Error(std::string_view code, std::string_view msg, std::optional<SourceSpan> where = std::nullopt) = 0;
};

#endif // IDIAGNOSTICSINK_HPP_
