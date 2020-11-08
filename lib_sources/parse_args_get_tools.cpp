
#include <build_project.hpp>

#include <iomanip>
#include <iostream>

using namespace std;

namespace build_project {

static void print_help(ostream& out, char const* program_name) {
  out << quoted(program_name) << " was generated using <build_project.hpp>.\n"
      << "[General]\n"
      << "\t " << program_name << " help\n"
      << "\t\t Display this message\n"
      << "[Building]\n"
      << "\t " << program_name << " [TARGET] [FLAGS]\n"
      << "\t\t Build the project. If TARGET is not specified, use the native\n"
      << "\t\t compiler (compiler used for building this program) and target\n"
      << "\t\t the native platform (your machine and OS).\n"
      << "\t\t You can specify a target by replacing [TARGET] with:\n"
      << "\t\t\t host=COMPILER:PLATFORM\n"
      << "\t\t\t\t Target PLATFORM using COMPILER\n"
      << "\t\t\t host=COMPILER\n"
      << "\t\t\t\t Target native platform (your machine and OS) using COMPILER\n"
      << "\t\t\t host=:PLATFORM\n"
      << "\t\t\t\t Target PLATFORM using native compiler\n"
      << "\t\t\t host=:\n"
      << "\t\t\t\t Target native platform using native compiler (same as default)\n"
      << "[Installation]\n"
      << "NOTE: Eventually, this part will be implemented by your compiler manufacturer.\n"
      << "TODO: Specify installation commands.\n";
}

auto parse_args_get_tools(int argc, char const* const* argv, string_view info_message)
    -> tools {
  auto const program_name = argv[0];

  if (argc == 1) { // by default, build natively
    char const* args[] = {program_name, "host=:", nullptr};
    return parse_args_get_tools(2, args, info_message);
  }
  // argc > 1
  string_view const first_arg = argv[1];
  if (first_arg == "help") {
    if (not info_message.empty())
      cout << info_message << '\n';
    print_help(cout, program_name);
    exit(EXIT_SUCCESS); // success: we have printed the help
  }

  throw error_not_supported("Currently, only `help` as first argument is supported.");
}

} // namespace build_project
