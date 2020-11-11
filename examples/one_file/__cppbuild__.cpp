#include <build_project.hpp>

// The purpose of this file is to compile `main.cpp`
// The produced "build" program must be launched from the directory this file is.

using namespace build_project;
using namespace std::literals;

int main(int argc, char** argv) {
  auto [native, host] = parse_args_get_tools(argc, argv, "Example of a project using one source file.");

  host->create(program("build/myprogram"), {"main.cpp"});
  return 0;
}