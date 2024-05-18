#pragma once
#include <cstdint>
#include <iosfwd>
#include <memory>
#include <optional>
#include <tuple>

namespace teknavo {

class Trey {
 public:
  using Ptr = std::unique_ptr<Trey>;

 public:
  Trey(std::int64_t a, std::int64_t b, std::int64_t c) : a_{a}, b_{b}, c_{c} {}

  void CalculateRoots() noexcept;
  void CalculateExtreme() noexcept;

  std::int64_t GetA() const noexcept { return a_; }
  std::int64_t GetB() const noexcept { return b_; }
  std::int64_t GetC() const noexcept { return c_; }

  const std::optional<double>& GetExtreme() const noexcept { return extreme_; }
  const std::optional<double>& GetFirstRoot() const noexcept {
    return first_root_;
  }
  const std::optional<double>& GetSecondRoot() const noexcept {
    return second_root_;
  }

 private:
  std::int64_t a_;
  std::int64_t b_;
  std::int64_t c_;
  std::optional<double> first_root_;
  std::optional<double> second_root_;
  std::optional<double> extreme_;
};

}  // namespace teknavo

std::ostream& operator<<(std::ostream& os, const teknavo::Trey& t);
