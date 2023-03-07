// SPDX-FileCopyrightText: Intel Corporation
//
// SPDX-License-Identifier: BSD-3-Clause

TYPED_TEST_P(CommonTests, ZipView) {
  using DV = typename TypeParam::DV;
  using V = typename TypeParam::V;

  std::size_t n = 10;

  DV dv_a(n), dv_b(n), dv_c(n);
  TypeParam::iota(dv_a, 100);
  TypeParam::iota(dv_b, 200);
  TypeParam::iota(dv_c, 300);

  // DISABLE 2 zip
  // auto d_z2 = zhp::zip(dv_a, dv_b);
  // EXPECT_TRUE(check_segments(d_z2));

  auto d_z3 = zhp::zip(dv_a, dv_b, dv_c);
  EXPECT_TRUE(check_segments(d_z3));
  barrier();

  if (comm_rank == 0) {
    V a(n), b(n), c(n);
    rng::iota(a, 100);
    rng::iota(b, 200);
    rng::iota(c, 300);

    // DISABLE 2 zip
    // auto z2 = rng::views::zip(a,  b);
    // EXPECT_TRUE(equal(z2, d_z2));

    auto z3 = rng::views::zip(a, b, c);
    EXPECT_TRUE(equal(z3, d_z3));
  }
}