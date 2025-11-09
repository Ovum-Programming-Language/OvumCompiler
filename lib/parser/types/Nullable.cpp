#include "Nullable.hpp"

Nullable::Nullable(bool on) noexcept : on_(on) {
}

bool Nullable::IsOn() const noexcept {
  return on_;
}

void Nullable::Set(bool on) noexcept {
  on_ = on;
}

void Nullable::Enable() noexcept {
  on_ = true;
}

void Nullable::Disable() noexcept {
  on_ = false;
}
