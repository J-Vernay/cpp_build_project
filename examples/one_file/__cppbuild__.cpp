#include <build_project.hpp>

// The purpose of this file is to compile `main.cpp`
// The produced "build" program must be launched from the directory this file is.

#include <fstream>

int main() {
  std::ofstream out{"test.txt"};
  out << build_project::get_greeting() << '\n';
  return 0;
}