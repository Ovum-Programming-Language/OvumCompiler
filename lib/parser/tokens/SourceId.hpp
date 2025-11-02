#ifndef SOURCEID_HPP_
#define SOURCEID_HPP_

#include <cstdint>
#include <string>
#include <string_view>

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

#endif // SOURCEID_HPP_
