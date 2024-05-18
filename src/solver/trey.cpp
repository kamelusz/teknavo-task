#include "solver/trey.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

template <typename T>
inline static int Sign(T v) {
  return v < 0 ? -1 : 1;
}

namespace teknavo {

void Trey::CalculateRoots() noexcept {
  const auto temp = -0.5 * (b_ + Sign(b_) * std::sqrt(b_ * b_ - 4 * a_ * c_));

  first_root_ = a_ != 0 ? std::make_optional(temp / a_) : std::nullopt;
  second_root_ = temp != 0 ? std::make_optional(c_ / temp) : std::nullopt;
}

void Trey::CalculateExtreme() noexcept {
  extreme_ = a_ != 0 ? std::make_optional(-(b_ / (static_cast<double>(2) * a_)))
                     : std::nullopt;
}

}  // namespace teknavo

std::ostream& operator<<(std::ostream& os, const teknavo::Trey& t) {
  os << "(" << t.GetA() << " " << t.GetB() << " " << t.GetC() << ") => ";

  const auto& x1 = t.GetFirstRoot();
  const auto& x2 = t.GetSecondRoot();
  const auto x1_has_value = x1.has_value();
  const auto x2_has_value = x2.has_value();
  if (!x1_has_value && !x2_has_value) {
    os << "no roots";
  } else {
    os << "(";
    if (x1_has_value) {
      const auto v = x1.value();
      if (v != v) {
        os << "complex root, ";
      } else {
        os << v << ", ";
      }
    }
    if (x2_has_value) {
      const auto v = x2.value();
      if (v != v) {
        os << "complex root";
      } else {
        os << v;
      }
    }
    os << ") ";
  }

  const auto& extreme = t.GetExtreme();
  if (!extreme.has_value()) {
    os << "no Xmin";
  } else {
    os << "Xmin=" << extreme.value();
  }

  return os;
}