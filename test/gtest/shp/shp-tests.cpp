// SPDX-FileCopyrightText: Intel Corporation
//
// SPDX-License-Identifier: BSD-3-Clause

#include "shp-tests.hpp"

// Instantiate SHP-specific configurations for common tests
using Common_Types = ::testing::Types<
    shp::distributed_vector<int, shp::device_allocator<int>>,
    shp::distributed_vector<float, shp::device_allocator<float>>,
    shp::distributed_vector<int, shp::shared_allocator<int>>,
    shp::distributed_vector<float, shp::shared_allocator<float>>>;

INSTANTIATE_TYPED_TEST_SUITE_P(MHP, DistributedVector, Common_Types);
INSTANTIATE_TYPED_TEST_SUITE_P(SHP, Drop, Common_Types);
INSTANTIATE_TYPED_TEST_SUITE_P(SHP, ForEach, Common_Types);
INSTANTIATE_TYPED_TEST_SUITE_P(SHP, Subrange, Common_Types);
INSTANTIATE_TYPED_TEST_SUITE_P(SHP, Take, Common_Types);
INSTANTIATE_TYPED_TEST_SUITE_P(SHP, TransformView, Common_Types);
INSTANTIATE_TYPED_TEST_SUITE_P(SHP, Zip, Common_Types);

// To share tests with MHP
int comm_rank = 0;
int comm_size = 1;

cxxopts::ParseResult options;

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  cxxopts::Options options_spec(argv[0], "DR SHP tests");

  // clang-format off
  options_spec.add_options()
    ("drhelp", "Print help")
    ("d, devicesCount", "number of GPUs to create", cxxopts::value<unsigned int>()->default_value("0"));
  // clang-format on

  try {
    options = options_spec.parse(argc, argv);
  } catch (const cxxopts::OptionParseException &e) {
    fmt::print("{}\n", options_spec.help());
    exit(1);
  }

  if (options.count("drhelp")) {
    fmt::print("{}\n", options_spec.help());
    exit(0);
  }

  const unsigned int dev_num = options["devicesCount"].as<unsigned int>();
  auto devices = shp::get_numa_devices(sycl::default_selector_v);

  if (dev_num > 0) {
    unsigned int i = 0;
    while (devices.size() < dev_num)
      devices.push_back(devices[i++]);
    devices.resize(dev_num); // if too many devices
  }

  shp::init(devices);

  for (auto &device : devices)
    std::cout << "  Device: " << device.get_info<sycl::info::device::name>()
              << "\n";

  return RUN_ALL_TESTS();
}
