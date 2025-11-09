#ifndef PARSER_SOURCEID_HPP_
#define PARSER_SOURCEID_HPP_

#include <cstdint>
#include <string>
#include <string_view>

namespace ovum::compiler::parser {

class SourceId {
public:
  SourceId() = default;
  explicit SourceId(std::string path) : path_(std::move(path)) {
  }

  [[nodiscard]] std::string_view Path() const noexcept;

  [[nodiscard]] std::string Basename() const;

  [[nodiscard]] bool IsValid() const noexcept;
  [[nodiscard]] bool operator==(const SourceId& other) const noexcept;
  [[nodiscard]] bool operator!=(const SourceId& other) const noexcept;

private:
  std::string path_;
};

} // namespace ovum::compiler::parser

#endif // PARSER_SOURCEID_HPP_
