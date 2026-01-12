#include "lib/parser/diagnostics/severity/Severity.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace ovum::compiler::parser {

namespace {
class SimpleSeverity : public ISeverity {
public:
  SimpleSeverity(std::string name, const int level) : name_(std::move(name)), level_(level) {
  }

  [[nodiscard]] std::string_view Name() const override {
    return name_;
  }

  [[nodiscard]] int Level() const override {
    return level_;
  }

private:
  std::string name_;
  int level_;
};
} // namespace

std::shared_ptr<const ISeverity> Severity::Note() {
  static const auto kInst = std::make_shared<const SimpleSeverity>("note", 10);
  return kInst;
}

std::shared_ptr<const ISeverity> Severity::Warning() {
  static const auto kInst = std::make_shared<const SimpleSeverity>("warning", 20);
  return kInst;
}

std::shared_ptr<const ISeverity> Severity::Error() {
  static const auto kInst = std::make_shared<const SimpleSeverity>("error", 30);
  return kInst;
}

std::shared_ptr<const ISeverity> Severity::Custom(const std::string_view name, int level) {
  return std::make_shared<const SimpleSeverity>(std::string{name}, level);
}

} // namespace ovum::compiler::parser
