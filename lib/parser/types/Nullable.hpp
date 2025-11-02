#ifndef NULLABLE_HPP_
#define NULLABLE_HPP_

class Nullable {
public:
  Nullable() noexcept = default;

  explicit Nullable(bool on) noexcept;

  bool IsOn() const noexcept;

  void Set(bool on) noexcept;

  void Enable() noexcept;

  void Disable() noexcept;

private:
  bool on_ = false;
};

#endif // NULLABLE_HPP_
