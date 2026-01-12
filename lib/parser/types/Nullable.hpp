#ifndef PARSER_NULLABLE_HPP_
#define PARSER_NULLABLE_HPP_

namespace ovum::compiler::parser {

class Nullable {
public:
  Nullable() noexcept = default;

  explicit Nullable(bool on) noexcept;

  [[nodiscard]] bool IsOn() const noexcept;

  void Set(bool on) noexcept;

  void Enable() noexcept;

  void Disable() noexcept;

private:
  bool on_ = false;
};

} // namespace ovum::compiler::parser

#endif // PARSER_NULLABLE_HPP_
