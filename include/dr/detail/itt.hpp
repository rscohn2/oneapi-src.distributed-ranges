// SPDX-FileCopyrightText: Intel Corporation
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once


#ifdef DR_ITT

#include <ittnotify.h>


class itt_log {
public:
  enum class task_id {
    init,
    sentinel
  };

  itt_log() {
    domain_ = __itt_domain_create("DR");
    for (std::size_t i = 0; i < num_ids_; i++) {
      handles_[i] = __itt_string_handle_create(names_[i]);
    }
  }

  void begin(task_id id) {
    fmt::print("XXX calling begin\n");
    __itt_task_begin(domain_, __itt_null, __itt_null, handles_[std::size_t(id)]);
  }

  void end() {
    __itt_task_end(domain_);
  }
    
private:
  static constexpr std::size_t num_ids_ = std::size_t(task_id::sentinel);
  __itt_domain *domain_;
  __itt_string_handle *handles_[num_ids_];
  const char *names_[num_ids_] = {"DR_Init"};
};



#else
#endif
