// SPDX-FileCopyrightText: Intel Corporation
//
// SPDX-License-Identifier: BSD-3-Clause

#include "xhp-tests.hpp"

TEST(Itt, Basic) {
  itt_log itt;
  itt.begin(itt_log::task_id::init);
  itt.end();
}
