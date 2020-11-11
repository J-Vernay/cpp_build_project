
#include <build_project.hpp>

#include <filesystem>
#include <iomanip>
#include <iostream>

using namespace std;
using namespace filesystem;

namespace build_project {

void compiler_interface::create([[maybe_unused]] project_file const& dst,
                                [[maybe_unused]] const project_file* inputs_data, [[maybe_unused]] size_t inputs_size,
                                [[maybe_unused]] language lang,
                                [[maybe_unused]] preprocessor_args const* pp_args,
                                [[maybe_unused]] flags const* flags) {
  throw error_not_supported("compiler_interface is not implemented yet.");
}

} // namespace build_project
