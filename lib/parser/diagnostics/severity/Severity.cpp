#include "lib/parser/diagnostics/severity/Severity.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace {
class SimpleSeverity : public ISeverity {
public:
  SimpleSeverity(std::string name, int level) : name_(std::move(name)), level_(level) {
  }

  std::string_view Name() const override {
    return name_;
  }

  int Level() const override {
    return level_;
  }

private:
  std::string name_;
  int level_;
};
} // namespace

const std::shared_ptr<const ISeverity>& Severity::Note() {
  static const auto inst = std::make_shared<const SimpleSeverity>("note", 10);
  return inst;
}

const std::shared_ptr<const ISeverity>& Severity::Warning() {
  static const auto inst = std::make_shared<const SimpleSeverity>("warning", 20);
  return inst;
}

const std::shared_ptr<const ISeverity>& Severity::Error() {
  static const auto inst = std::make_shared<const SimpleSeverity>("error", 30);
  return inst;
}

std::shared_ptr<const ISeverity> Severity::Custom(std::string_view name, int level) {
  return std::make_shared<const SimpleSeverity>(std::string{name}, level);
}
