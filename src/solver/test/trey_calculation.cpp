#include "common.hpp"
#include "solver/trey.hpp"

namespace teknavo {
namespace test {
namespace {

struct TestData {
  std::int64_t a;
  std::int64_t b;
  std::int64_t c;
  double x1;
  double x2;
  double extreme;
};

class TreyTest : public testing::TestWithParam<TestData> {};

TEST_P(TreyTest, CalculatesRegularRoots) {
  const auto& p = GetParam();
  Trey t{p.a, p.b, p.c};

  t.CalculateRoots();

  ASSERT_TRUE(t.GetFirstRoot());
  EXPECT_NEAR(*t.GetFirstRoot(), p.x1, 0.001);
  ASSERT_TRUE(t.GetSecondRoot());
  EXPECT_NEAR(*t.GetSecondRoot(), p.x2, 0.001);
}

TEST_P(TreyTest, CalculatesExtreme) {
  const auto& p = GetParam();
  Trey t{p.a, p.b, p.c};

  t.CalculateExtreme();

  ASSERT_TRUE(t.GetExtreme());
  ASSERT_NEAR(*t.GetExtreme(), p.extreme, 0.001);
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(Trey, TreyTest, testing::Values(
    TestData{4, -5, -12, 2.466, -1.216, 0.625},
    TestData{1, -5, 6, 3, 2, 2.5},
    TestData{3, -5, 2, 1, 0.667, 0.833},
    TestData{1, -2, -3, 3, -1, 1},
    TestData{2, 10, 10, -3.618, -1.382, -2.5},
    TestData{-5, 5, 5, 1.618, -0.618, 0.5},
    TestData{6, 26, 8, -4, -0.333, -2.167}
));
// clang-format on

class ComplexRoots : public testing::TestWithParam<TestData> {};

TEST_P(ComplexRoots, DoesntCalculateComplexRoots) {
  const auto& p = GetParam();
  Trey t{p.a, p.b, p.c};

  t.CalculateRoots();

  ASSERT_TRUE(t.GetFirstRoot());
  ASSERT_TRUE(*t.GetFirstRoot() != *t.GetFirstRoot());
  ASSERT_TRUE(t.GetSecondRoot());
  ASSERT_TRUE(*t.GetSecondRoot() != *t.GetSecondRoot());
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(Trey, ComplexRoots, testing::Values(
    TestData{6, 10, 8, 0, 0, 0},
    TestData{3, 4, 3, 0, 0, -0.833}
));
// clang-format on

}  // namespace
}  // namespace test
}  // namespace teknavo
