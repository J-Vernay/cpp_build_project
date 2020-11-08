#include <build_project.hpp>

// The purpose of this file is to compile `main.cpp`
// The produced "build" program must be launched from the directory this file is.

int main(int argc, char** argv) {
  auto [native, host] = build_project::parse_args_get_tools(argc, argv, "Example of a project using one source file.");
  return 0;
}