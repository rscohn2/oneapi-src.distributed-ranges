// SPDX-FileCopyrightText: Intel Corporation
//
// SPDX-License-Identifier: BSD-3-Clause

// Fixture
template <typename T> class Drop : public testing::Test {
public:
};

TYPED_TEST_SUITE_P(Drop);

TYPED_TEST_P(Drop, Basic) {
  using DV = typename TypeParam::DV;
  using V = typename TypeParam::V;

  std::size_t n = 10;

  auto neg = [](auto &v) { v = -v; };
  DV dv_a(n);
  TypeParam::iota(dv_a, 100);
  auto d = rng::views::drop(dv_a, 2);
  EXPECT_TRUE(check_segments(d));
  barrier();
  xhp::for_each(TypeParam::policy(), d, neg);

  if (comm_rank == 0) {
    V a(n), a_in(n);
    rng::iota(a, 100);
    rng::iota(a_in, 100);
    rng::for_each(rng::views::drop(a, 2), neg);
    EXPECT_TRUE(unary_check(a_in, a, dv_a));
  }
}

REGISTER_TYPED_TEST_SUITE_P(Drop, Basic);