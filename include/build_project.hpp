#ifndef CPP_BUILD_PROJECT
#define CPP_BUILD_PROJECT

#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <string_view>

namespace build_project {

// Declaration of classes defined in this header.
class preprocessor_args;
class flags;
class compiler_interface;

// Common using declarations
using std::initializer_list;
using std::string_view;
using std::unique_ptr;

struct error_not_supported : std::runtime_error {
  using std::runtime_error::runtime_error; // inheriting constructors
};

struct tools {
  unique_ptr<compiler_interface> native; //< For native platform, i.e. code generators
  unique_ptr<compiler_interface> host;   //< For final-user platform, i.e. the actual program
  // TODO: Consider adding "target"? maybe useless?
  // TODO: Add dependency manager?
};

/// Parse the command-line arguments from `main(argc,argv)` and provide required tools for the asked build.
/// May call `exit()` if needed (i.e. after printing help or if detecting invalid argument).
auto parse_args_get_tools(int argc, char const* const* argv, string_view info_message = "") -> tools;

class compiler_interface {};

} // namespace build_project

#endif