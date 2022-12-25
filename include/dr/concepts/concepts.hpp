// SPDX-FileCopyrightText: Intel Corporation
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "../details/ranges.hpp"
#include <ranges>

namespace lib {

template <typename I>
concept remote_iterator =
    std::forward_iterator<I> && requires(I &iter) { lib::ranges::rank(iter); };

template <typename R>
concept remote_range =
    std::ranges::forward_range<R> && requires(R &r) { lib::ranges::rank(r); };

template <typename R>
concept distributed_range =
    std::ranges::forward_range<R> &&
    requires(R &r) {
      { lib::ranges::segments(r) } -> std::ranges::forward_range;
    } &&
    remote_range<std::ranges::range_value_t<decltype(lib::ranges::segments(
        std::declval<R>()))>>;

template <typename I>
concept remote_contiguous_iterator =
    std::random_access_iterator<I> && requires(I &iter) {
                                        lib::ranges::rank(iter);
                                        {
                                          lib::ranges::local(iter)
                                          } -> std::contiguous_iterator;
                                      };
template <typename R>
concept remote_contiguous_range =
    remote_range<R> && std::ranges::random_access_range<R> &&
    requires(R &r) {
      lib::ranges::rank(r);
      { lib::ranges::local(r) } -> std::ranges::contiguous_range;
    };

template <typename R>
concept distributed_contiguous_range =
    distributed_range<R> && std::ranges::random_access_range<R> &&
    requires(R &r) {
      { lib::ranges::segments(r) } -> std::ranges::random_access_range;
    } &&
    remote_contiguous_range<std::ranges::range_value_t<
        decltype(lib::ranges::segments(std::declval<R>()))>>;

template <typename I>
concept mpi_distributed_iterator =
    std::forward_iterator<I> && requires(I &iter) {
                                  { iter.container() };
                                };

template <typename I>
concept mpi_distributed_contiguous_iterator =
    std::random_access_iterator<I> && mpi_distributed_iterator<I>;

template <typename R>
concept mpi_distributed_range =
    std::ranges::forward_range<R> && requires(R &r) {
                                       {
                                         r.begin()
                                         } -> mpi_distributed_iterator;
                                       { r.end() } -> mpi_distributed_iterator;
                                     };

template <typename I>
concept sycl_mpi_distributed_contiguous_iterator =
    mpi_distributed_contiguous_iterator<I> &&
    requires(I &iter) {
      { iter.container().allocator().policy() };
    };

template <typename R>
concept mpi_distributed_contiguous_range =
    mpi_distributed_range<R> && std::ranges::random_access_range<R>;

template <typename ZR>
concept distributed_range_zip = requires(ZR &zr) {
                                  { std::get<0>(*zr.begin()) };
                                };

} // namespace lib
