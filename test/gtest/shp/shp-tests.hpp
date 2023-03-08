// SPDX-FileCopyrightText: Intel Corporation
//
// SPDX-License-Identifier: BSD-3-Clause

#include <sycl/sycl.hpp>

#include "dr/shp/shp.hpp"

extern int comm_rank;
extern int comm_size;

// Namespace aliases and wrapper functions to make the tests uniform
namespace zhp = shp::views;
namespace xhp = shp;

inline void barrier() {}
inline void fence() {}

// Need shp::iota
// Why doesn't rng::iota work?
auto iota(auto &&r, auto val) { return std::iota(r.begin(), r.end(), val); }

template <typename T> auto default_policy_of_allocator() {
  return shp::par_unseq;
};

#include "common-tests.hpp"
