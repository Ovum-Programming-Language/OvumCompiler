#include "SourceId.hpp"

#include <filesystem>

namespace ovum::compiler::parser {

std::string_view SourceId::Path() const noexcept {
  return path_;
}

std::string SourceId::Basename() const {
  if (path_.empty()) {
    return {};
  }

  std::filesystem::path p(path_);
  return p.filename().string();
}

bool SourceId::IsValid() const noexcept {
  return !path_.empty();
}

bool SourceId::operator==(const SourceId& other) const noexcept {
  return path_ == other.path_;
}

bool SourceId::operator!=(const SourceId& other) const noexcept {
  return !(*this == other);
}

} // namespace ovum::compiler::parser
